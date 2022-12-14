#include "Framework.h"

map<wstring, Texture*> Texture::totalTexture;

Texture::Texture(ID3D11ShaderResourceView* srv, ScratchImage& image)
    : srv(srv), image(move(image))
{    
}

Texture::~Texture()
{
    srv->Release();
}

Texture* Texture::Add(wstring file)
{
    if (totalTexture.count(file) > 0)
        return totalTexture[file];

    ScratchImage image;

    wstring extension = GetExtension(file);

    HRESULT result;

    if (extension == L"tga")
        result = LoadFromTGAFile(file.c_str(), nullptr, image);
    else if (extension == L"dds")
        result = LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);
    else
        result = LoadFromWICFile(file.c_str(), WIC_FLAGS_FORCE_RGB, nullptr, image);

    if (FAILED(result))
        return nullptr;

    ID3D11ShaderResourceView* srv;

    V(CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &srv));

    totalTexture[file] = new Texture(srv, image);
    totalTexture[file]->file = file;

    return totalTexture[file];
}

Texture* Texture::Load(wstring file)
{
    if (totalTexture.count(file) > 0)
        delete totalTexture[file];

    ScratchImage image;

    wstring extension = GetExtension(file);

    HRESULT result;

    if (extension == L"tga")
        result = LoadFromTGAFile(file.c_str(), nullptr, image);
    else if (extension == L"dds")
        result = LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);
    else
        result = LoadFromWICFile(file.c_str(), WIC_FLAGS_FORCE_RGB, nullptr, image);

    if (FAILED(result))
        return nullptr;

    ID3D11ShaderResourceView* srv;

    V(CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &srv));

    totalTexture[file] = new Texture(srv, image);
    totalTexture[file]->file = file;

    return totalTexture[file];
}

void Texture::Delete()
{
    for (auto texture : totalTexture)
        delete texture.second;
}

void Texture::PSSet(UINT slot)
{
    DC->PSSetShaderResources(slot, 1, &srv);
}

void Texture::DSSet(UINT slot)
{
    DC->DSSetShaderResources(slot, 1, &srv);
}

vector<Float4> Texture::ReadPixels()
{
    uint8_t* colors = image.GetPixels();
    UINT size = image.GetPixelsSize();

    float scale = 1.0f / 255.0f;

    vector<Float4> result(size / 4);

    for (size_t i = 0; i < result.size(); i++)
    {
        result[i].x = colors[i * 4 + 0] * scale;
        result[i].y = colors[i * 4 + 1] * scale;
        result[i].z = colors[i * 4 + 2] * scale;
        result[i].w = colors[i * 4 + 3] * scale;
    }

    return result;
}


