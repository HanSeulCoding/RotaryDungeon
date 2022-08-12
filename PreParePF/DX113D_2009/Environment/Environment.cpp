#include "Framework.h"

Environment::Environment()
{
	CreatePerspective();

	lightBuffer = new LightBuffer();
	lightBuffer->Add();

	samplerState = new SamplerState();
	samplerState->SetState();

	mainCamera = new Camera();	
}

Environment::~Environment()
{	
	delete projectionBuffer;
	delete lightBuffer;

	delete samplerState;

	delete mainCamera;
}

void Environment::PostRender()
{
	mainCamera->PostRender();
	ImGui::Text("LightInfo");
	ImGui::ColorEdit4("LightAmbient", (float*)&lightBuffer->data.ambient);
	ImGui::ColorEdit4("LightAmbientCeil", (float*)&lightBuffer->data.ambientCeil);

	if (ImGui::Button("AddLight"))
		lightBuffer->Add();

	for (int i = 0; i < lightBuffer->data.lightCount; i++)
	{
		string name = "Light " + to_string(i);
		if (ImGui::BeginMenu(name.c_str()))
		{
			ImGui::Checkbox("Active", (bool*)&lightBuffer->data.lights[i].active);

			ImGui::SliderInt("Type", (int*)&lightBuffer->data.lights[i].type, 0, 3);
			ImGui::ColorEdit4("Color", (float*)&lightBuffer->data.lights[i].color);
			ImGui::SliderFloat3("Direction", (float*)&lightBuffer->data.lights[i].direction, -1, 1);
			ImGui::SliderFloat3("Position", (float*)&lightBuffer->data.lights[i].position, -100, 100);
			ImGui::SliderFloat("Range", (float*)&lightBuffer->data.lights[i].range, 0, 100);
			ImGui::SliderFloat("Inner", (float*)&lightBuffer->data.lights[i].inner, 0, 180);
			ImGui::SliderFloat("Outer", (float*)&lightBuffer->data.lights[i].outer, 0, 180);
			ImGui::SliderFloat("Length", (float*)&lightBuffer->data.lights[i].length, 0, 200);

			ImGui::EndMenu();
		}
	}
	
}

void Environment::Set()
{
	SetViewport();
	SetViewProjection();

	lightBuffer->SetPSBuffer(0);
}

void Environment::SetViewProjection()
{
	mainCamera->SetBuffer();
	projectionBuffer->SetVSBuffer(2);
	projectionBuffer->SetDSBuffer(2);
	projectionBuffer->SetGSBuffer(2);
}

void Environment::SetViewport(float width, float height)
{	
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	DC->RSSetViewports(1, &viewport);
}

void Environment::CreatePerspective()
{	
	projectionBuffer = new MatrixBuffer();
	projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 1000.0f);
	projectionBuffer->Set(projection);	
}
