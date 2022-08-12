#pragma once

class Terrain : public Transform
{
private:
	typedef VertexUVNormalTangent VertexType;

	struct InputDesc
	{
		UINT index;
		Float3 v0, v1, v2;
	};

	struct OutputDesc
	{
		int picked;
		float u, v, distance;
	};

	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT width, height;

	Texture* heightMap;
	Texture* alphaMap;

	Texture* secondMap;
	Texture* thirdMap;

	RasterizerState* fillMode[2];

	ComputeShader* computeShader;
	RayBuffer* rayBuffer;
	StructuredBuffer* structuredBuffer;
	InputDesc* input;
	OutputDesc* output;

	UINT size;
public:
	Terrain(wstring shaderFile, wstring dataFile);
	~Terrain();

	void Update();	
	void Render();	

	bool Picking(OUT Vector3* position);
	bool ComputePicking(OUT Vector3* position);
	float GetHeight(Vector3 position);

	Material* GetMaterial() { return material; }

	Float2 GetSize() { return Float2(width, height); }
private:
	void CreateMesh();
	void CreateNormal();
	void CreateTangent();

	void LoadData(wstring dataFile);
};