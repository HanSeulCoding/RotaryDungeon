#include "Framework.h"

Model::Model(string file) : ModelReader(file)
{
	boneBuffer = new BoneBuffer();

	MakeTransform();
	SetBoneTransforms();	
}

Model::~Model()
{
	delete boneBuffer;
}

void Model::Render()
{
	SetWorldBuffer();
	boneBuffer->SetVSBuffer(4);

	MeshRender();
}

void Model::MakeTransform()
{
	nodeTransforms = new Matrix[nodes.size()];
	UINT nodeIndex = 0;

	for (NodeData* node : nodes)
	{
		Matrix parent;
		int parentIndex = node->parent;

		if (parentIndex < 0)
			parent = XMMatrixIdentity();
		else
			parent = nodeTransforms[parentIndex];

		nodeTransforms[nodeIndex] = XMLoadFloat4x4(&node->transform) * parent;

		if (boneMap.count(node->name) > 0)
		{
			int boneIndex = boneMap[node->name];

			Matrix offset = XMLoadFloat4x4(&bones[boneIndex]->offset);

			boneTransforms[boneIndex] = offset * nodeTransforms[nodeIndex];
		}

		nodeIndex++;
	}
}

void Model::SetBoneTransforms()
{
	for (auto bone : boneTransforms)
		boneBuffer->Add(bone.second, bone.first);
}

Matrix Model::GetNodeTransform(string name)
{
	return nodeTransforms[GetNodeByName(name)];
}
