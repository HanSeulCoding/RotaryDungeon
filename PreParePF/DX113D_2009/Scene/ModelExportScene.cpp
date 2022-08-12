#include "Framework.h"
#include "ModelExportScene.h"

ModelExportScene::ModelExportScene()
{
	string name = "Boss";

	
	/*exporter = new ModelExporter("ModelData/FBX/" + name + ".fbx");
	exporter->ExportMaterial(name);
	exporter->ExportMesh(name);	
	delete exporter;*/
	
	
	
	exporter = new ModelExporter("ModelData/Animations/" + name + "/JumpAttack3.fbx");	
	exporter->ExportClip(name + "/JumpAttack3");
	delete exporter;

	/*exporter = new ModelExporter("ModelData/Animations/" + name + "/Run.fbx");
	exporter->ExportClip(name + "/Run");
	delete exporter;*/

	
	/*exporter = new ModelExporter("ModelData/Animations/" + name + "/JumpAttack.fbx");
	exporter->ExportClip(name + "/JumpAttack");
	delete exporter;
	
	exporter = new ModelExporter("ModelData/Animations/" + name + "/RockAttack.fbx");
	exporter->ExportClip(name + "/RockAttack");
	delete exporter;*/
	
}

ModelExportScene::~ModelExportScene()
{	
}

void ModelExportScene::Update()
{
}

void ModelExportScene::PreRender()
{
}

void ModelExportScene::Render()
{
}

void ModelExportScene::PostRender()
{
}
