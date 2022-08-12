#pragma once

class Cube : public Transform
{
private:
	typedef VertexUVNormal VertexType;

	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	Vector3 size;
public:
	Cube(Vector3 size = Vector3(1, 1, 1));
	~Cube();

	void Render();

	Material* GetMaterial() { return material; }
private:
	void Create();
	void CreateNormal();
};