#pragma once

class Shader
{
private:
	static map<wstring, Shader*> totalShader;

protected:
	ID3DBlob* blob;
	
	virtual ~Shader() = default;
public:
	static class VertexShader* AddVS(wstring file, string entry = "VS");
	static class PixelShader* AddPS(wstring file, string entry = "PS");
	static class ComputeShader* AddCS(wstring file, string entry = "CS");
	static class HullShader* AddHS(wstring file, string entry = "HS");
	static class DomainShader* AddDS(wstring file, string entry = "DS");
	static class GeometryShader* AddGS(wstring file, string entry = "GS");

	static void Delete();

	virtual void Set() = 0;
};