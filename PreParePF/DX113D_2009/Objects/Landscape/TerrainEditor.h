#pragma once

class TerrainEditor : public Transform
{
private:
	typedef VertexUVNormalTangentAlpha VertexType;

	const float MAX_HEIGHT = 100.0f;

	enum SaveType
	{
		HEIGHTMAP,
		ALPHAMAP,
		DATA
	}saveType;

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

	string projectPath;

	ComputeShader* computeShader;
	RayBuffer* rayBuffer;
	StructuredBuffer* structuredBuffer;
	InputDesc* input;
	OutputDesc* output;

	UINT size;

	BrushBuffer* brushBuffer;

	bool isRaise;
	float adjustValue;

	bool isPainting;
	float paintValue;

	int selectMap;
public:
	TerrainEditor(UINT width = 300, UINT height = 300);
	~TerrainEditor();

	void Update();
	void Render();
	void PostRener();

	bool Picking(OUT Vector3* position);

	void AdjustY();
	void PaintBrush();

	Material* GetMaterial() { return material; }
private:
	void CreateMesh();
	void CreateNormal();
	void CreateTangent();

	void CreateCompute();

	void SelectMap();

	void Save();
	void Load();

	void SaveHeightMap(wstring heightFile);
	void LoadHeightMap(wstring heightFile);

	void SaveAlphaMap(wstring alphaFile);
	void LoadAlphaMap(wstring alphaFile);

	void SaveData(wstring dataFile);
	void LoadData(wstring dataFile);
};