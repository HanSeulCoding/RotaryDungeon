#include "Framework.h"

Terrain::Terrain(wstring shaderFile, wstring dataFile)
	: heightMap(nullptr), alphaMap(nullptr), secondMap(nullptr), thirdMap(nullptr)
{
	material = new Material(shaderFile);
	LoadData(dataFile);

	CreateMesh();	

	fillMode[0] = new RasterizerState();
	fillMode[1] = new RasterizerState();
	//fillMode[1]->FillMode(D3D11_FILL_WIREFRAME);

	computeShader = Shader::AddCS(L"ComputePicking");

	structuredBuffer = new StructuredBuffer(input, sizeof(InputDesc), size,
		sizeof(OutputDesc), size);

	rayBuffer = new RayBuffer();
	output = new OutputDesc[size];
}

Terrain::~Terrain()
{
	delete material;
	delete mesh;

	delete fillMode[0];
	delete fillMode[1];

	delete structuredBuffer;
	delete rayBuffer;
	delete[] input;
	delete[] output;
}

void Terrain::Update()
{
}

void Terrain::Render()
{
	mesh->IASet();

	SetWorldBuffer();

	if(alphaMap != nullptr)	alphaMap->PSSet(10);
	if(secondMap != nullptr) secondMap->PSSet(11);
	if (thirdMap != nullptr) thirdMap->PSSet(12);

	material->Set();

	fillMode[1]->SetState();

	DC->DrawIndexed((UINT)indices.size(), 0, 0);

	fillMode[0]->SetState();
}

bool Terrain::Picking(OUT Vector3* position)
{
	Ray ray = CAMERA->ScreenPointToRay(MOUSEPOS);

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			UINT index[4];
			index[0] = width * z + x;
			index[1] = width * z + x + 1;
			index[2] = width * (z + 1) + x;
			index[3] = width * (z + 1) + x + 1;

			Vector3 p[4];
			for (UINT i = 0; i < 4; i++)
				p[i] = vertices[index[i]].position;

			float distance;
			if (Intersects(ray.pos.data, ray.dir.data, p[0].data, p[1].data, p[2].data, distance))
			{
				*position = ray.pos + ray.dir * distance;
				return true;
			}

			if (Intersects(ray.pos.data, ray.dir.data, p[3].data, p[1].data, p[2].data, distance))
			{
				*position = ray.pos + ray.dir * distance;
				return true;
			}
		}
	}

	return false;
}

bool Terrain::ComputePicking(OUT Vector3* position)
{
	Ray ray = CAMERA->ScreenPointToRay(MOUSEPOS);

	rayBuffer->data.position = ray.pos;
	rayBuffer->data.direction = ray.dir;
	rayBuffer->data.size = size;
	computeShader->Set();

	rayBuffer->SetCSBuffer(0);

	DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());
	DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

	UINT x = ceil((float)size / 1024.0f);

	DC->Dispatch(x, 1, 1);

	structuredBuffer->Copy(output, sizeof(OutputDesc) * size);

	float minDistance = FLT_MAX;
	int minIndex = -1;

	for (UINT i = 0; i < size; i++)
	{
		OutputDesc temp = output[i];
		if (temp.picked)
		{
			if (minDistance > temp.distance)
			{
				minDistance = temp.distance;
				minIndex = i;
			}
		}
	}

	if (minIndex >= 0)
	{
		*position = ray.pos + ray.dir * minDistance;
		return true;
	}

	return false;
}

float Terrain::GetHeight(Vector3 position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x > width) return 0.0f;
	if (z < 0 || z > height) return 0.0f;

	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + x + 1;
	index[3] = width * (z + 1) + x + 1;

	Vector3 p[4];
	for (UINT i = 0; i < 4; i++)
		p[i] = vertices[index[i]].position;

	float u = position.x - p[0].x;
	float v = position.z - p[0].z;

	Vector3 result;

	if (u + v <= 1.0f)
	{
		result = p[0] + (p[2] - p[0]) * u + (p[1] - p[0]) * v;
	}
	else
	{
		u = 1.0f - u;
		v = 1.0f - v;

		result = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
	}

	return result.y;
}

void Terrain::CreateMesh()
{
	width = heightMap->Width();
	height = heightMap->Height();

	vector<Float4> pixels = heightMap->ReadPixels();

	//Vertices
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			VertexType vertex;
			vertex.position = Float3((float)x, 0, (float)z);
			vertex.uv = Float2(x / (float)width, z / (float)height);

			UINT index = width * z + x;
			vertex.position.y = pixels[index].x * 20.0f;

			vertices.emplace_back(vertex);
		}
	}

	//Indices
	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices.emplace_back(width * z + x);//0
			indices.emplace_back(width * (z + 1) + x);//1
			indices.emplace_back(width * (z + 1) + x + 1);//2

			indices.emplace_back(width * z + x);//0
			indices.emplace_back(width * (z + 1) + x + 1);//2
			indices.emplace_back(width * z + x + 1);//3
		}
	}

	size = indices.size() / 3;

	input = new InputDesc[size];
	for (UINT i = 0; i < size; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		input[i].v0 = vertices[index0].position;
		input[i].v1 = vertices[index1].position;
		input[i].v2 = vertices[index2].position;

		input[i].index = i;
	}

	CreateNormal();
	CreateTangent();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), (UINT)vertices.size(),
		indices.data(), (UINT)indices.size());
}

void Terrain::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = vertices[index0].position;
		Vector3 v1 = vertices[index1].position;
		Vector3 v2 = vertices[index2].position;

		Vector3 A = v1 - v0;
		Vector3 B = v2 - v0;

		Vector3 normal = Vector3::Cross(A, B).Normalized();

		vertices[index0].normal = normal + vertices[index0].normal;
		vertices[index1].normal = normal + vertices[index1].normal;
		vertices[index2].normal = normal + vertices[index2].normal;
	}
}

void Terrain::CreateTangent()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexType vertex0 = vertices[index0];
		VertexType vertex1 = vertices[index1];
		VertexType vertex2 = vertices[index2];

		Vector3 p0 = vertex0.position;
		Vector3 p1 = vertex1.position;
		Vector3 p2 = vertex2.position;

		Float2 uv0 = vertex0.uv;
		Float2 uv1 = vertex1.uv;
		Float2 uv2 = vertex2.uv;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		float u0 = uv1.x - uv0.x;
		float u1 = uv2.x - uv0.x;
		float v0 = uv1.y - uv0.y;
		float v1 = uv2.y - uv0.y;

		float d = 1.0f / (u0 * v1 - v0 * u1);
		Vector3 tangent = (e0 * v1 - e1 * v0) * d;

		vertices[index0].tangent = tangent + vertices[index0].tangent;
		vertices[index1].tangent = tangent + vertices[index1].tangent;
		vertices[index2].tangent = tangent + vertices[index2].tangent;
	}

	for (VertexType& vertex : vertices)
	{
		Vector3 t = vertex.tangent;
		Vector3 n = vertex.normal;

		vertex.tangent = (t - n * Vector3::Dot(n, t)).Normalized();
	}
}

void Terrain::LoadData(wstring dataFile)
{
	BinaryReader* r = new BinaryReader(dataFile);

	heightMap = Texture::Add(ToWString(r->String()));
	alphaMap = Texture::Add(ToWString(r->String()));
	material->SetDiffuseMap(ToWString(r->String()));
	material->SetSpecularMap(ToWString(r->String()));
	material->SetNormalMap(ToWString(r->String()));
	secondMap = Texture::Add(ToWString(r->String()));
	thirdMap = Texture::Add(ToWString(r->String()));	

	delete r;

	if (heightMap == nullptr)
		assert(true);
}
