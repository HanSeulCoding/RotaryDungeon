#include "Framework.h"

Camera::Camera()
	: moveSpeed(20.0f), rotSpeed(5.0f),
	distance(73), height(37), offset(0, 5, 0), moveDamping(5), rotDamping(0),
	destRot(0), rotY(0), target(nullptr)
{
	viewBuffer = new MatrixBuffer();
	viewPosBuffer = new VectorBuffer();
	oldPos = MOUSEPOS;

	Load();
}

Camera::~Camera()
{
	delete viewBuffer;
	delete viewPosBuffer;
}

void Camera::Update()
{
	if (target)
		FollowMode();
	else
		FreeMode();

	Shake();
}

void Camera::PostRender()
{
	ImGui::Text("Main Camera");
	ImGui::Text("CamPos : %.2f, %.2f, %.2f", localPosition.x, localPosition.y, localPosition.z);
	ImGui::Text("CamRot : %.2f, %.2f, %.2f", localRotation.x, localRotation.y, localRotation.z);

	if (target)
	{
		ImGui::SliderFloat("CamDistance", &distance, -10.0f, 100.0f);
		ImGui::SliderFloat("CamHeight", &height, -10.0f, 100.0f);
		ImGui::SliderFloat("CamMoveDamping", &moveDamping, 0.0f, 30.0f);
		ImGui::SliderFloat("CamRotDamping", &rotDamping, 0.0f, 30.0f);
		ImGui::SliderFloat3("CamOffset", (float*)&offset, -20.f, 20.0f);
	}
	else
	{
		ImGui::SliderFloat("CamMoveSpeed", &moveSpeed, 0.0f, 50.0f);
		ImGui::SliderFloat("CamRotSpeed", &rotSpeed, 0.0f, 10.0f);
	}

	if (ImGui::Button("Save"))
		Save();

	if (ImGui::Button("Load"))
		Load();
}

void Camera::FreeMode()
{
	FreeMove();
	FreeRotation();
	SetView();
}

void Camera::FollowMode()
{
	if (target->localRotation.y != destRot)
		destRot = LERP(destRot, target->localRotation.y + XM_PI, rotDamping * DELTA);

	rotMatrix = XMMatrixRotationY(destRot);

	Vector3 forward = XMVector3TransformNormal(kForward, rotMatrix);
	destPos = forward * -distance;

	destPos += target->GlobalPosition();
	destPos.y += height;

	localPosition = LERP(localPosition, destPos, moveDamping * DELTA);

	Vector3 tempOffset = XMVector3TransformCoord(offset.data, rotMatrix);

	view = XMMatrixLookAtLH(localPosition.data, (target->GlobalPosition() + tempOffset).data, kUp);
	viewBuffer->Set(view);

	SetViewPos();
}

void Camera::FreeMove()
{
	if (KEY_PRESS(VK_RBUTTON))
	{
		if (KEY_PRESS('W'))
			localPosition += Forward() * moveSpeed * DELTA;
		if (KEY_PRESS('S'))
			localPosition -= Forward() * moveSpeed * DELTA;
		if (KEY_PRESS('A'))
			localPosition -= Right() * moveSpeed * DELTA;
		if (KEY_PRESS('D'))
			localPosition += Right() * moveSpeed * DELTA;
		if (KEY_PRESS('Q'))
			localPosition -= Up() * moveSpeed * DELTA;
		if (KEY_PRESS('E'))
			localPosition += Up() * moveSpeed * DELTA;
	}
}

void Camera::FreeRotation()
{
	if (KEY_PRESS(VK_RBUTTON))
	{
		Vector3 value = MOUSEPOS - oldPos;

		localRotation.x += value.y * rotSpeed * DELTA;
		localRotation.y += value.x * rotSpeed * DELTA;
	}

	oldPos = MOUSEPOS;
}

void Camera::ShakeStart(float magnitude, float duration, float reduceDamping)
{
	this->magnitude = magnitude;
	this->duration = duration;
	this->reduceDamping = reduceDamping;

	originPosition = localPosition;
}

void Camera::Save()
{
	BinaryWriter* w = new BinaryWriter("TextData/CameraData.data");
	w->Float(localPosition.x);
	w->Float(localPosition.y);
	w->Float(localPosition.z);
	w->Float(localRotation.x);
	w->Float(localRotation.y);
	w->Float(localRotation.z);

	w->Float(moveSpeed);
	w->Float(rotSpeed);

	w->Float(distance);
	w->Float(height);
	w->Float(moveDamping);
	w->Float(rotDamping);
	w->Float(offset.x);
	w->Float(offset.y);
	w->Float(offset.z);
	
	delete w;
}

void Camera::Load()
{
	BinaryReader* r = new BinaryReader("TextData/CameraData.data");
	localPosition.x = r->Float();
	localPosition.y = r->Float();
	localPosition.z = r->Float();
	localRotation.x = r->Float();
	localRotation.y = r->Float();
	localRotation.z = r->Float();
	
	moveSpeed = r->Float();
	rotSpeed = r->Float();
	
	distance = r->Float();
	height = r->Float();
	moveDamping = r->Float();
	rotDamping = r->Float();
	offset.x = r->Float();
	offset.y = r->Float();
	offset.z = r->Float();

	delete r;
}

void Camera::Shake()
{
	if (duration <= 0.0f) return;

	duration -= DELTA;
	magnitude -= reduceDamping * DELTA;

	if (magnitude < 0.0f)
	{
		magnitude = 0.0f;
		duration = 0.0f;
	}

	Vector3 temp;
	temp.x = Random(-magnitude, magnitude);
	temp.y = Random(-magnitude, magnitude);
	temp.z = 0;

	localPosition = originPosition + temp;

	if (duration <= 0.0f)
		localPosition = originPosition;
}

void Camera::SetView()
{
	SetViewPos();

	Vector3 focus = localPosition + Forward();
	view = XMMatrixLookAtLH(localPosition.data, focus.data, Up().data);

	viewBuffer->Set(view);
}

void Camera::SetViewPos()
{
	UpdateWorld();

	viewPosBuffer->data.vector.x = localPosition.x;
	viewPosBuffer->data.vector.y = localPosition.y;
	viewPosBuffer->data.vector.z = localPosition.z;
}

void Camera::SetBuffer()
{
	viewBuffer->SetVSBuffer(1);
	viewPosBuffer->SetVSBuffer(3);

	viewBuffer->SetDSBuffer(1);
	viewPosBuffer->SetHSBuffer(3);

	viewBuffer->SetGSBuffer(1);
	viewPosBuffer->SetGSBuffer(3);
}

Ray Camera::ScreenPointToRay(Vector3 screenPos)
{
	Float2 screenSize(WIN_WIDTH, WIN_HEIGHT);

	Float2 point;
	point.x = ((2 * screenPos.x) / screenSize.x) - 1.0f;
	point.y = (((2 * screenPos.y) / screenSize.y) - 1.0f) * -1.0f;

	Matrix projection = Environment::Get()->GetProjection();

	Float4x4 temp;
	XMStoreFloat4x4(&temp, projection);

	point.x /= temp._11;
	point.y /= temp._22;

	Ray ray;
	ray.pos = localPosition;

	Matrix invView = XMMatrixInverse(nullptr, view);

	Vector3 tempPos(point.x, point.y, 1.0f);

	ray.dir = XMVector3TransformNormal(tempPos.data, invView);
	ray.dir.Normalize();

	return ray;
}
