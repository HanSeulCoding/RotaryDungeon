#pragma once

class Quad : public Transform
{
protected:
	Material* material;
	Mesh* mesh;

	vector<VertexA> vertices;
	vector<UINT> indices;

	Float2 size;
public:
	Quad(wstring textureFile);	
	Quad(Float2 size);
	Quad(wstring textureFile, Float2 size);
	~Quad();
		
	void Render();

	Material* GetMaterial() { return material; }
private:
	void CreateMesh();
};
