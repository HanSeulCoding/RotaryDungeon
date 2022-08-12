#include "Framework.h"

Grass::Grass(Terrain* terrain) : count(1000)
{
	VertexSize* vertices = new VertexSize[count];

	for (UINT i = 0; i < count; i++)
	{
		Vector3 pos;
		pos.x = Random(0.0f, terrain->GetSize().x - 1);
		pos.z = Random(0.0f, terrain->GetSize().y - 1);

		Vector3 scale;
		scale.x = Random(3.0f, 10.0f);
		scale.y = Random(3.0f, 10.0f);
		scale.z = 1.0f;

		pos.y = (scale.y * 0.5f + terrain->GetHeight(pos));

		vertices[i].position = pos;
		vertices[i].size = { scale.x, scale.y };
	}

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexSize), count);
	delete[] vertices;

	material = new Material(L"Billboard");
	material->SetDiffuseMap(L"Textures/Environment/Grass.png");

	geometryShader = Shader::AddGS(L"Billboard");

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Alpha(true);
	//blendState[1]->AlphaToCoverage(true);

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);
}

Grass::~Grass()
{
	delete blendState[0];
	delete blendState[1];

	delete depthState[0];
	delete depthState[1];
}

void Grass::Update()
{
}

void Grass::Render()
{
	blendState[1]->SetState();
	depthState[1]->SetState();

	//for (Billboard* tree : trees)
		//tree->Render();

	vertexBuffer->IASet();
	DC->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	material->Set();
	geometryShader->Set();

	DC->Draw(count, 0);

	DC->GSSetShader(nullptr, nullptr, 0);

	blendState[0]->SetState();
	depthState[0]->SetState();
}

void Grass::PreRender()
{
}
