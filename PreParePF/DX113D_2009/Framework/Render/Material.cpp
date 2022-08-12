#include "Framework.h"

Material::Material()
	: vertexShader(nullptr), pixelShader(nullptr),
	diffuseMap(nullptr), specularMap(nullptr), normalMap(nullptr)
{
	buffer = new MaterialBuffer();
}

Material::Material(wstring file)
	: diffuseMap(nullptr), specularMap(nullptr), normalMap(nullptr)
{
	SetShader(file);

	buffer = new MaterialBuffer();
}

Material::~Material()
{
	delete buffer;
}

void Material::Set()
{
	buffer->SetPSBuffer(1);

	if (diffuseMap != nullptr)
		diffuseMap->PSSet(0);

	if (specularMap != nullptr)
		specularMap->PSSet(1);

	if (normalMap != nullptr)
		normalMap->PSSet(2);

	vertexShader->Set();
	pixelShader->Set();
}

void Material::SetShader(wstring file)
{
	SetVertexShader(file);
	SetPixelShader(file);
}

void Material::SetVertexShader(wstring file)
{
	vertexShader = Shader::AddVS(file);
}

void Material::SetPixelShader(wstring file)
{
	pixelShader = Shader::AddPS(file);
}

void Material::SetDiffuseMap(wstring file)
{
	diffuseMap = Texture::Add(file);
	if (diffuseMap != nullptr)
		buffer->data.hasDiffuseMap = 1;
	else
		buffer->data.hasDiffuseMap = 0;
}

void Material::SetSpecularMap(wstring file)
{
	specularMap = Texture::Add(file);
	if (specularMap != nullptr)
		buffer->data.hasSpecularMap = 1;
	else
		buffer->data.hasSpecularMap = 0;
}

void Material::SetNormalMap(wstring file)
{
	normalMap = Texture::Add(file);
	if (normalMap != nullptr)
		buffer->data.hasNormalMap = 1;
	else
		buffer->data.hasNormalMap = 0;
}

void Material::SetTexture(wstring textureFile)
{
	mainTexture = Texture::Add(textureFile);
}
