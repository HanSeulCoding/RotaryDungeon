#include "Framework.h"
#include "ModelRenderScene.h"

ModelRenderScene::ModelRenderScene()
{
	string name = "Boss";

	model = new Model(name);	
	//model->SetShader(L"Model");
	//model->SetDiffuseMap(L"ModelData/Materials/lightning.png");
	//model->SetNormalMap(L"ModelData/Materials/chicken leg piece_Normal.png");
	model->SetShader(L"Lighting");
	
	sphere = new Sphere(L"Lighting", 10);
		
	//node = model->GetNodeTransform("RightHand");
	//sphere->SetParent(&node);
	CAMERA->localPosition = Vector3(14.60, 77.60, -203.80);
	CAMERA->localRotation = Vector3(-0.16, -0.12, 0);
}

ModelRenderScene::~ModelRenderScene()
{
	delete model;
	delete sphere;
}

void ModelRenderScene::Update()
{
}

void ModelRenderScene::PreRender()
{
}

void ModelRenderScene::Render()
{
	model->Render();
	//sphere->Render();
}

void ModelRenderScene::PostRender()
{
	ImGui::SliderFloat3("rotation", (float*)&model->localRotation, 0, XM_PI * 2);
}
