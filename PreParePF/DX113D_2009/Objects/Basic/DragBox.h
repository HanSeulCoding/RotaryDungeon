#pragma once

class DragBox : public Transform
{
private:
	Material* material;
	Mesh* mesh;

	Matrix view;
	Matrix orthographic;

	MatrixBuffer* viewBuffer;
	MatrixBuffer* projectionBuffer;

public:
	DragBox();
	~DragBox();

	void Render();

private:
	void CreateMesh();
	void CreateViewProjection();
};