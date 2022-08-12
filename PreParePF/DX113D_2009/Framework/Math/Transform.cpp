#include "Framework.h"

Transform::Transform(string tag)
	: tag(tag), localScale(1, 1, 1), parent(nullptr), isActive(true)
{
	worldBuffer = new MatrixBuffer();
	UpdateWorld();
}

Transform::~Transform()
{
	delete worldBuffer;
}

void Transform::UpdateWorld()
{
	world = XMMatrixTransformation(pivot.data, XMQuaternionIdentity(), localScale.data, pivot.data,
		XMQuaternionRotationRollPitchYaw(localRotation.x, localRotation.y, localRotation.z),
		localPosition.data);

	if (parent != nullptr)
		world *= *parent;

	XMMatrixDecompose(&globalScale.data, &globalRotation.data, &globalPosition.data, world);

	worldBuffer->Set(world);
}

void Transform::PostRender()
{
	string str = tag + " Transform";
	ImGui::Text(str.c_str());
	str = tag + " Position";
	ImGui::InputFloat3(str.c_str(), (float*)&localPosition);
	str = tag + " Rotation";
	ImGui::SliderFloat3(str.c_str(), (float*)&localRotation, 0, XM_2PI);
	str = tag + " Scale";
	ImGui::InputFloat3(str.c_str(), (float*)&localScale);
}

void Transform::SetWorldBuffer(UINT slot)
{
	UpdateWorld();
	worldBuffer->SetVSBuffer(slot);
}

Vector3 Transform::Forward()
{
	return XMVector3Normalize(XMVector3TransformNormal(kForward, world));
}

Vector3 Transform::Up()
{
	return XMVector3Normalize(XMVector3TransformNormal(kUp, world));
}

Vector3 Transform::Right()
{
	return XMVector3Normalize(XMVector3TransformNormal(kRight, world));
}
