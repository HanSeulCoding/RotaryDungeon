#include "Framework.h"

DepthStencil::DepthStencil(UINT width, UINT height, bool isStencil)
{
	{//DSV Texture
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = isStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;

		V(DEVICE->CreateTexture2D(&desc, nullptr, &dsvTexture));
	}

	{//RTV
		D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
		desc.Format = isStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		V(DEVICE->CreateDepthStencilView(dsvTexture, &desc, &dsv));
	}

	{//SRV
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = isStencil ? DXGI_FORMAT_R24_UNORM_X8_TYPELESS : DXGI_FORMAT_R32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;

		V(DEVICE->CreateShaderResourceView(dsvTexture, &desc, &srv));
	}
}

DepthStencil::~DepthStencil()
{
	dsvTexture->Release();

	dsv->Release();
	srv->Release();
}

void DepthStencil::Clear()
{
	DC->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
