#include "Framework.h"

TerrainData::TerrainData(wstring heightFile)
{
	heightMap = Texture::Add(heightFile);

	CreateData();
	CreateNormal();

	AlignVertexData();
}

TerrainData::~TerrainData()
{
}

void TerrainData::CreateData()
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

			prevData.emplace_back(vertex);
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
}

void TerrainData::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = prevData[index0].position;
		Vector3 v1 = prevData[index1].position;
		Vector3 v2 = prevData[index2].position;

		Vector3 A = v1 - v0;
		Vector3 B = v2 - v0;

		Vector3 normal = Vector3::Cross(A, B).Normalized();

		prevData[index0].normal = normal + prevData[index0].normal;
		prevData[index1].normal = normal + prevData[index1].normal;
		prevData[index2].normal = normal + prevData[index2].normal;
	}
}

void TerrainData::AlignVertexData()
{
	vertices.resize(indices.size());

	UINT index = 0;
	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			UINT index0 = width * z + x;//0
			UINT index1 = width * z + x + 1;//1
			UINT index2 = width * (z + 1) + x;//2
			UINT index3 = width * (z + 1) + x + 1;//3			
			
			vertices[index++] = prevData[index2];
			vertices[index++] = prevData[index3];
			vertices[index++] = prevData[index0];

			vertices[index++] = prevData[index0];
			vertices[index++] = prevData[index3];
			vertices[index++] = prevData[index1];
		}
	}
}
