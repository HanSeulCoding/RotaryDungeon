#pragma once
class Grass
{
	Terrain* terrain;
	//vector<Billboard*> trees;

	Material* material;
	GeometryShader* geometryShader;

	VertexBuffer* vertexBuffer;
	UINT count;

	BlendState* blendState[2];
	DepthStencilState* depthState[2];
public:
	Grass(Terrain* terrain);
	~Grass();

	void Update();
	void Render();
	void PreRender();

	void SetTerrain(Terrain* value) { terrain = value; }


};