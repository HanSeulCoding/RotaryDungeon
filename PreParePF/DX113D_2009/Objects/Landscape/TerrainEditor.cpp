#include "Framework.h"

TerrainEditor::TerrainEditor(UINT width, UINT height)
	: width(width), height(height),
	heightMap(nullptr), secondMap(nullptr), thirdMap(nullptr),
	isRaise(true), adjustValue(50),
	structuredBuffer(nullptr), input(nullptr), output(nullptr),
	isPainting(true), paintValue(5), selectMap(0)
{
	material = new Material(L"TerrainSplatting");
	material->SetDiffuseMap(L"Textures/Environment/TerrainGEN_3LAVAColor.png");
	material->SetNormalMap(L"Textures/Environment/TerrainGEN_3Normal.png");
	material->SetSpecularMap(L"Textures/Environment/TerrainGEN_3Depo.png");
	alphaMap = Texture::Add(L"Textures/HeightMaps/AlphaMap.png");
	
	CreateMesh();
	CreateCompute();

	igfd::ImGuiFileDialog::Instance()->OpenDialog("StartKey", "Start", 0, ".");
	projectPath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
	igfd::ImGuiFileDialog::Instance()->CloseDialog("StartKey");	

	brushBuffer = new BrushBuffer();
	rayBuffer = new RayBuffer();
	CAMERA->localPosition = Vector3(0, 0, 0);
}

TerrainEditor::~TerrainEditor()
{
	delete material;
	delete mesh;

	delete structuredBuffer;
	delete rayBuffer;
	delete[] input;
	delete[] output;

	delete brushBuffer;
}

void TerrainEditor::Update()
{
	if (KEY_PRESS(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
	{
		if (isPainting)
			PaintBrush();
		else
			AdjustY();
	}

	if (KEY_UP(VK_LBUTTON))
	{
		CreateNormal();
		CreateTangent();
		mesh->UpdateVertex(vertices.data(), vertices.size());
	}
}

void TerrainEditor::Render()
{
	mesh->IASet();

	SetWorldBuffer();

	brushBuffer->SetPSBuffer(10);

	alphaMap->PSSet(10);
	if(secondMap != nullptr)
		secondMap->PSSet(11);
	if (thirdMap != nullptr)
		thirdMap->PSSet(12);
	material->Set();

	DC->DrawIndexed((UINT)indices.size(), 0, 0);		
}

void TerrainEditor::PostRener()
{
	ImGui::Text("TerrainEditor");
	SelectMap();

	Vector3 temp;
	Picking(&temp);
	brushBuffer->data.location = temp;

	ImGui::Text("PickingPos : %f, %f, %f", temp.x, temp.y, temp.z);
	ImGui::SliderInt("BrushType", &brushBuffer->data.type, 0, 1);
	ImGui::SliderFloat("BrushRange", &brushBuffer->data.range, 1, 50);
	ImGui::ColorEdit3("BrushColor", (float*)&brushBuffer->data.color);
	ImGui::Checkbox("Raise", &isRaise);
	ImGui::Checkbox("Paint", &isPainting);
	ImGui::SliderInt("SelectMap", &selectMap, 0, 1);

	Save();
	Load();
}

bool TerrainEditor::Picking(OUT Vector3* position)
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

void TerrainEditor::AdjustY()
{
	Vector3 location = brushBuffer->data.location;

	switch (brushBuffer->data.type)
	{
	case 0:
	{
		for (VertexType& vertex : vertices)
		{
			Vector3 p1 = Vector3(vertex.position.x, 0, vertex.position.z);
			Vector3 p2 = Vector3(location.x, 0, location.z);

			float distance = (p2 - p1).Length();

			float temp = adjustValue * max(0, cos(XM_PIDIV2 * distance / brushBuffer->data.range));

			if (distance <= brushBuffer->data.range)
			{
				if (isRaise)
					vertex.position.y += temp * DELTA;
				else
					vertex.position.y -= temp * DELTA;

				if (vertex.position.y > MAX_HEIGHT)
					vertex.position.y = MAX_HEIGHT;
				else if (vertex.position.y < 0)
					vertex.position.y = 0;
			}
		}
	}
		break;	
	case 1:
	{
		float size = brushBuffer->data.range * 0.5f;

		float left = location.x - size;
		float right = location.x + size;
		float top = location.z + size;
		float bottom = location.z - size;

		if (left < 0) left = 0;
		if (right > width) right = width;
		if (top > height) top = height;
		if (bottom < 0) bottom = 0;

		for (UINT z = (UINT)bottom; z <= (UINT)top; z++)
		{
			for (UINT x = (UINT)left; x <= (UINT)right; x++)
			{
				UINT index = width * z + x;

				if (isRaise)
					vertices[index].position.y += adjustValue * DELTA;
				else
					vertices[index].position.y -= adjustValue * DELTA;

				if (vertices[index].position.y > MAX_HEIGHT)
					vertices[index].position.y = MAX_HEIGHT;
				else if (vertices[index].position.y < 0)
					vertices[index].position.y = 0;
			}
		}
	}
		break;
	}

	mesh->UpdateVertex(vertices.data(), vertices.size());
}

void TerrainEditor::PaintBrush()
{
	Vector3 location = brushBuffer->data.location;

	switch (brushBuffer->data.type)
	{
	case 0:
	{
		for (VertexType& vertex : vertices)
		{
			Vector3 p1 = Vector3(vertex.position.x, 0, vertex.position.z);
			Vector3 p2 = Vector3(location.x, 0, location.z);

			float distance = (p2 - p1).Length();

			float temp = paintValue * max(0, cos(XM_PIDIV2 * distance / brushBuffer->data.range));

			if (distance <= brushBuffer->data.range)
			{
				if (isRaise)
					vertex.alpha[selectMap] += temp * DELTA;
				else
					vertex.alpha[selectMap] -= temp * DELTA;

				vertex.alpha[selectMap] = Saturate(vertex.alpha[selectMap]);
			}
		}
	}
	break;
	case 1:
	{
		float size = brushBuffer->data.range * 0.5f;

		float left = location.x - size;
		float right = location.x + size;
		float top = location.z + size;
		float bottom = location.z - size;

		if (left < 0) left = 0;
		if (right > width) right = width;
		if (top > height) top = height;
		if (bottom < 0) bottom = 0;

		for (UINT z = (UINT)bottom; z <= (UINT)top; z++)
		{
			for (UINT x = (UINT)left; x <= (UINT)right; x++)
			{
				UINT index = width * z + x;

				if (isRaise)
					vertices[index].alpha[selectMap] += paintValue * DELTA;
				else
					vertices[index].alpha[selectMap] -= paintValue * DELTA;

				vertices[index].alpha[selectMap] = Saturate(vertices[index].alpha[selectMap]);
			}
		}
	}
	break;
	}

	mesh->UpdateVertex(vertices.data(), vertices.size());
}

void TerrainEditor::CreateMesh()
{
	vector<Float4> pixels;

	if (heightMap)
	{
		width = heightMap->Width();
		height = heightMap->Height();
		pixels = heightMap->ReadPixels();
	}

	vertices.clear();
	indices.clear();

	//Vertices
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			VertexType vertex;
			vertex.position = Float3((float)x, 0, (float)z);
			vertex.uv = Float2(x / (float)width, z / (float)height);	

			UINT index = width * z + x;
			if (pixels.size() > index)
				vertex.position.y = pixels[index].x * MAX_HEIGHT;

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

	if (input != nullptr)
		delete[] input;

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

void TerrainEditor::CreateNormal()
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

void TerrainEditor::CreateTangent()
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

void TerrainEditor::CreateCompute()
{
	computeShader = Shader::AddCS(L"ComputePicking");

	if (structuredBuffer != nullptr)
		delete structuredBuffer;

	structuredBuffer = new StructuredBuffer(input, sizeof(InputDesc), size,
		sizeof(OutputDesc), size);
	
	if (output != nullptr)
		delete[] output;

	output = new OutputDesc[size];
}

void TerrainEditor::SelectMap()
{
	if (ImGui::Button("DiffuseMap"))
		igfd::ImGuiFileDialog::Instance()->OpenDialog("DiffuseKey", "Select DiffuseMap", ".png,.jpg,.tga,.dds", ".");
		
	if (igfd::ImGuiFileDialog::Instance()->FileDialog("DiffuseKey"))
	{
		if (igfd::ImGuiFileDialog::Instance()->IsOk)
		{
			string path = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
			path = path.substr(projectPath.size() + 1, path.length());
			material->SetDiffuseMap(ToWString(path));
		}

		igfd::ImGuiFileDialog::Instance()->CloseDialog("DiffuseKey");
	}

	if (ImGui::Button("SapcularMap"))
		igfd::ImGuiFileDialog::Instance()->OpenDialog("SpecularKey", "Select SapcularMap", ".png,.jpg,.tga,.dds", ".");

	if (igfd::ImGuiFileDialog::Instance()->FileDialog("SpecularKey"))
	{
		if (igfd::ImGuiFileDialog::Instance()->IsOk)
		{
			string path = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
			path = path.substr(projectPath.size() + 1, path.length());
			material->SetSpecularMap(ToWString(path));
		}

		igfd::ImGuiFileDialog::Instance()->CloseDialog("SpecularKey");
	}

	if (ImGui::Button("NormalMap"))
		igfd::ImGuiFileDialog::Instance()->OpenDialog("NormalKey", "Select NormalMap", ".png,.jpg,.tga,.dds", ".");

	if (igfd::ImGuiFileDialog::Instance()->FileDialog("NormalKey"))
	{
		if (igfd::ImGuiFileDialog::Instance()->IsOk)
		{
			string path = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
			path = path.substr(projectPath.size() + 1, path.length());
			material->SetNormalMap(ToWString(path));
		}

		igfd::ImGuiFileDialog::Instance()->CloseDialog("NormalKey");
	}

	if (ImGui::Button("SecondMap"))
		igfd::ImGuiFileDialog::Instance()->OpenDialog("SecondKey", "Select SecondMap", ".png,.jpg,.tga,.dds", ".");

	if (igfd::ImGuiFileDialog::Instance()->FileDialog("SecondKey"))
	{
		if (igfd::ImGuiFileDialog::Instance()->IsOk)
		{
			string path = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
			path = path.substr(projectPath.size() + 1, path.length());
			secondMap = Texture::Add(ToWString(path));
		}

		igfd::ImGuiFileDialog::Instance()->CloseDialog("SecondKey");
	}

	if (ImGui::Button("ThirdMap"))
		igfd::ImGuiFileDialog::Instance()->OpenDialog("ThirdKey", "Select ThirdMap", ".png,.jpg,.tga,.dds", ".");

	if (igfd::ImGuiFileDialog::Instance()->FileDialog("ThirdKey"))
	{
		if (igfd::ImGuiFileDialog::Instance()->IsOk)
		{
			string path = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
			path = path.substr(projectPath.size() + 1, path.length());
			thirdMap = Texture::Add(ToWString(path));
		}

		igfd::ImGuiFileDialog::Instance()->CloseDialog("ThirdKey");
	}
}

void TerrainEditor::Save()
{
	if (ImGui::Button("Save HeightMap"))
	{
		saveType = HEIGHTMAP;
		igfd::ImGuiFileDialog::Instance()->OpenDialog("SaveKey", "Save", ".png,.jpg,.tga,.dds", ".");
	}

	/*if (ImGui::Button("Save AlphaMap"))
	{
		saveType = ALPHAMAP;
		igfd::ImGuiFileDialog::Instance()->OpenDialog("SaveKey", "Save", ".png,.jpg,.tga,.dds", ".");
	}*/

	if (ImGui::Button("Save Data"))
	{
		saveType = DATA;
		igfd::ImGuiFileDialog::Instance()->OpenDialog("SaveKey", "Save", ".data", ".");
	}

	if (igfd::ImGuiFileDialog::Instance()->FileDialog("SaveKey"))
	{
		if (igfd::ImGuiFileDialog::Instance()->IsOk)
		{
			string path = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
			path = path.substr(projectPath.size() + 1, path.length());
			switch (saveType)
			{
			case TerrainEditor::HEIGHTMAP:
				SaveHeightMap(ToWString(path));
				break;
			case TerrainEditor::ALPHAMAP:
				SaveAlphaMap(ToWString(path));
				break;
			case TerrainEditor::DATA:
				SaveData(ToWString(path));
				break;
			default:
				break;
			}			
		}

		igfd::ImGuiFileDialog::Instance()->CloseDialog("SaveKey");
	}
}

void TerrainEditor::Load()
{
	if (ImGui::Button("Load HeightMap"))
	{
		saveType = HEIGHTMAP;
		igfd::ImGuiFileDialog::Instance()->OpenDialog("LoadKey", "Load", ".png,.jpg,.tga,.dds", ".");
	}


	if (ImGui::Button("Load Data"))
	{
		saveType = DATA;
		igfd::ImGuiFileDialog::Instance()->OpenDialog("LoadKey", "Load", ".data", ".");
	}

	if (igfd::ImGuiFileDialog::Instance()->FileDialog("LoadKey"))
	{
		if (igfd::ImGuiFileDialog::Instance()->IsOk)
		{
			string path = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
			path = path.substr(projectPath.size() + 1, path.length());
			switch (saveType)
			{
			case TerrainEditor::HEIGHTMAP:
				LoadHeightMap(ToWString(path));
				break;
			/*case TerrainEditor::ALPHAMAP:
				LoadAlphaMap(ToWString(path));
				break;*/
			case TerrainEditor::DATA:
				LoadData(ToWString(path));
				break;
			default:
				break;
			}			
		}

		igfd::ImGuiFileDialog::Instance()->CloseDialog("LoadKey");
	}
}

void TerrainEditor::SaveHeightMap(wstring heightFile)
{
	UINT size = width * height * 4;
	uint8_t* pixels = new uint8_t[size];

	for (UINT i = 0; i < size / 4; i++)
	{
		float y = vertices[i].position.y;

		uint8_t height = y * 255 / MAX_HEIGHT;

		pixels[i * 4 + 0] = height;
		pixels[i * 4 + 1] = height;
		pixels[i * 4 + 2] = height;
		pixels[i * 4 + 3] = 255;
	}

	Image image;
	image.width = width;
	image.height = height;
	image.pixels = pixels;
	image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	image.rowPitch = width * 4;
	image.slicePitch = width * height * 4;

	SaveToWICFile(image, WIC_FLAGS_FORCE_RGB, GetWICCodec(WIC_CODEC_PNG), heightFile.c_str());
}

void TerrainEditor::LoadHeightMap(wstring heightFile)
{
	heightMap = Texture::Load(heightFile);

	delete mesh;

	CreateMesh();
	CreateCompute();
}

void TerrainEditor::SaveAlphaMap(wstring alphaFile)
{
	UINT size = width * height * 4;
	uint8_t* pixels = new uint8_t[size];

	for (UINT i = 0; i < size / 4; i++)
	{
		pixels[i * 4 + 0] = vertices[i].alpha[0] * 255;
		pixels[i * 4 + 1] = vertices[i].alpha[1] * 255;
		pixels[i * 4 + 2] = vertices[i].alpha[2] * 255;
		pixels[i * 4 + 3] = 255;
	}

	Image image;
	image.width = width;
	image.height = height;
	image.pixels = pixels;
	image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	image.rowPitch = width * 4;
	image.slicePitch = width * height * 4;

	SaveToWICFile(image, WIC_FLAGS_FORCE_RGB, GetWICCodec(WIC_CODEC_PNG), alphaFile.c_str());
}

void TerrainEditor::LoadAlphaMap(wstring alphaFile)
{
	alphaMap = Texture::Load(alphaFile);
	
	vector<Float4> pixels = alphaMap->ReadPixels();

	for (UINT i = 0 ; i < vertices.size() ; i++)
	{
		vertices[i].alpha[0] = pixels[i].x;
		vertices[i].alpha[1] = pixels[i].y;
		vertices[i].alpha[2] = pixels[i].z;
		vertices[i].alpha[3] = pixels[i].w;
	}

	mesh->UpdateVertex(vertices.data(), vertices.size());
}

void TerrainEditor::SaveData(wstring dataFile)
{
	BinaryWriter* w = new BinaryWriter(dataFile);
	
	if(heightMap != nullptr)
		w->String(ToString(heightMap->GetFile()));
	else
		w->String("None");

	if(alphaMap != nullptr)
		w->String(ToString(alphaMap->GetFile()));
	else
		w->String("None");

	if(material->GetDiffuseMap() != nullptr)
		w->String(ToString(material->GetDiffuseMap()->GetFile()));
	else
		w->String("None");
	
	if (material->GetSpecularMap() != nullptr)
		w->String(ToString(material->GetSpecularMap()->GetFile()));
	else
		w->String("None");

	if (material->GetNormalMap() != nullptr)
		w->String(ToString(material->GetNormalMap()->GetFile()));
	else
		w->String("None");

	if (secondMap != nullptr)
		w->String(ToString(secondMap->GetFile()));
	else
		w->String("None");

	if (thirdMap != nullptr)
		w->String(ToString(thirdMap->GetFile()));
	else
		w->String("None");

	delete w;
}

void TerrainEditor::LoadData(wstring dataFile)
{
	BinaryReader* r = new BinaryReader(dataFile);

	wstring heightMap = ToWString(r->String());
	wstring alphaMap = ToWString(r->String());
	material->SetDiffuseMap(ToWString(r->String()));
	material->SetSpecularMap(ToWString(r->String()));
	material->SetNormalMap(ToWString(r->String()));
	secondMap = Texture::Add(ToWString(r->String()));
	thirdMap = Texture::Add(ToWString(r->String()));

	LoadHeightMap(heightMap);
	LoadAlphaMap(alphaMap);

	delete r;
}
