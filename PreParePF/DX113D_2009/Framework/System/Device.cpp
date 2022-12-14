#include "Framework.h"

Device::Device()
{
	CreateDeviceAndSwapchain();
	CreateBackBuffer();
}

Device::~Device()
{
	device->Release();
	deviceContext->Release();

	swapChain->Release();

	renderTartetView->Release();
}

void Device::CreateDeviceAndSwapchain()
{
	UINT width = WIN_WIDTH;
	UINT height = WIN_HEIGHT;

	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hWnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = true;

	V(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		&swapChain,
		&device,
		nullptr,
		&deviceContext
	));	
}

void Device::CreateBackBuffer()
{
	ID3D11Texture2D* backBuffer;

	V(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer));
	V(device->CreateRenderTargetView(backBuffer, nullptr, &renderTartetView));
	backBuffer->Release();

	ID3D11Texture2D* depthBuffer;

	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = WIN_WIDTH;
		desc.Height = WIN_HEIGHT;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		V(device->CreateTexture2D(&desc, nullptr, &depthBuffer));
	}

	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		V(device->CreateDepthStencilView(depthBuffer, &desc, &depthStencilView));
	}
}

void Device::SetRenderTarget()
{
	deviceContext->OMSetRenderTargets(1, &renderTartetView, depthStencilView);
}

void Device::Clear(Float4 color)
{
	deviceContext->ClearRenderTargetView(renderTartetView, (float*)&color);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Device::Present()
{
	swapChain->Present(0, 0);
}
