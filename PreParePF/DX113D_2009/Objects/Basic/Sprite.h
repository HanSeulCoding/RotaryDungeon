#pragma once

class Sprite : public Transform
{
protected:
	
	Mesh* mesh;

	ID3D11ShaderResourceView* srv;

	Matrix view;
	Matrix orthographic;

	MatrixBuffer* viewBuffer;
	MatrixBuffer* projectionBuffer;
	BlackWhiteBuffer* blackWhiteBuffer;

	BlendState* blendState[2];
	DepthStencilState* depthState[2];

public:
	Material* material;
	Sprite(wstring shaderFile);
	virtual ~Sprite();

	virtual void Update();
	virtual void Render();

	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }
	Material* SetMaterial() { return material; }
	ID3D11ShaderResourceView* GetSRV() { return srv; }
private:
	void CreateMesh();
	void CreateViewProjection();
};