#pragma once

class Material
{
public:
	string name;

private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	Texture* diffuseMap;
	Texture* specularMap;
	Texture* normalMap;

	MaterialBuffer* buffer;
	Texture* mainTexture;
public:
	Material();
	Material(wstring file);
	~Material();

	void Set();

	void SetShader(wstring file);
	void SetVertexShader(wstring file);
	void SetPixelShader(wstring file);

	void SetDiffuseMap(wstring file);
	void SetSpecularMap(wstring file);
	void SetNormalMap(wstring file);
	void SetTexture(wstring textureFile);

	Texture* GetTexture() { return mainTexture; }
	Texture* GetDiffuseMap() { return diffuseMap; }
	Texture* GetSpecularMap() { return specularMap; }
	Texture* GetNormalMap() { return normalMap; }

	MaterialBuffer* GetBuffer() { return buffer; }
};