#pragma once

class Model : public ModelReader, public Transform
{
private:
	BoneBuffer* boneBuffer;

	map<int, Matrix> boneTransforms;
	Matrix* nodeTransforms;
public:
	Model(string file);
	virtual ~Model();

	void Render();

	void MakeTransform();
	void SetBoneTransforms();

	Matrix GetNodeTransform(string name);
};