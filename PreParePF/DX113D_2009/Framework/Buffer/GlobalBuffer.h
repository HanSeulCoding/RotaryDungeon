#pragma once

class MatrixBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix matrix;
	}data;

public:
	MatrixBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.matrix = XMMatrixIdentity();
	}

	void Set(Matrix value)
	{
		data.matrix = XMMatrixTranspose(value);
	}
};

struct Light
{
	Float4 color = Float4(1, 1, 1, 1);

	Float3 direction = Float3(0, -1, 1);
	int type = 0;

	Float3 position = Float3(0, 0, 0);
	float range = 80.0f;

	float inner = 55.0f;
	float outer = 65.0f;
	
	float length = 50.0f;
	int active = 1;
};

class LightBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Light lights[MAX_LIGHT];

		Float4 ambient;
		Float4 ambientCeil;

		int lightCount = 0;

		float padding[3];
	}data;

	LightBuffer() : ConstBuffer(&data, sizeof(Data))
	{		
		data.ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
		data.ambientCeil = { 0.1f, 0.1f, 0.1f, 1.0f };
	}

	void Add()
	{
		data.lightCount++;
	}
};

class VectorBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float4 vector;
	}data;

	VectorBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.vector = Float4(0, 0, 0, 0);
	}
};

class MaterialBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float4 diffuse;
		Float4 specular;
		Float4 ambient;
		Float4 emissive;

		float shininess;

		int hasDiffuseMap;
		int hasSpecularMap;
		int hasNormalMap;

	}data;

	MaterialBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.diffuse = { 1, 1, 1, 1 };
		data.specular = { 1, 1, 1, 1 };
		data.ambient = { 0.1f, 0.1f, 0.1f, 1 };
		data.emissive = { 0, 0, 0, 0 };

		data.shininess = 24.0f;

		data.hasDiffuseMap = 0;
		data.hasSpecularMap = 0;
		data.hasNormalMap = 0;
	}
};

class RayBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float3 position;
		float size;

		Float3 direction;
		float padding;
	}data;

	RayBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.position = Float3(0, 0, 0);
		data.size = 0;
		data.direction = Float3(0, 0, 0);
	}
};
class BrushBuffer : public ConstBuffer
{
public:
	struct Data
	{
		int type;
		Float3 location;		

		float range;
		Float3 color;
	}data;

	BrushBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.type = 0;
		data.location = Float3(0, 0, 0);
		data.range = 10;
		data.color = Float3(0, 0.5f, 0);
	}
};

class BoneBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Matrix bones[MAX_BONE];
	}data;

	BoneBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		for (UINT i = 0; i < MAX_BONE; i++)
			data.bones[i] = XMMatrixIdentity();
	}

	void Add(Matrix matrix, UINT index)
	{
		data.bones[index] = XMMatrixTranspose(matrix);
	}
};

class FrameBuffer : public ConstBuffer
{
public:
	struct Clip
	{
		int clip = 0;
		UINT curFrame = 0;
		float time = 0.0f;
		float speed = 1.0f;
	};

	struct Frame
	{
		float takeTime;
		float tweenTime;
		float runningTime;
		UINT frameCount;

		Clip cur;
		Clip next;

		Frame() : takeTime(0.5f), tweenTime(0.5f), runningTime(0.0f)
		{			
			next.clip = -1;
		}
	};

	struct Data
	{
		Frame frames[MAX_INSTANCE];
	} data;

	FrameBuffer() : ConstBuffer(&data, sizeof(Data))
	{	
		
	}
};

class TypeBuffer : public ConstBuffer
{
public:
	struct Data
	{
		int type = 0;		

		float padding[3];
	}data;

	TypeBuffer() : ConstBuffer(&data, sizeof(Data))
	{		
	}
};

class IntBuffer : public ConstBuffer
{
public:
	struct Data
	{
		int value[4] = {};
	}data;

	IntBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};

class TerrainBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float2 distance = Float2(1, 1000);
		Float2 factor = Float2(1, 64);

		float cellSpacing = 5.0f;
		float cellSpacingU;
		float cellSpacingV;
		float heightScale = 20.0f;

		Float4 cullings[6];
	}data;

	TerrainBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};

class SparkBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float3 direction = Float3(0, 0, 0);
		float duration = 0.0f;

		float time = 0.0f;
		float padding[3];
	}data;

	SparkBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};

class ColorBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float4 color = Float4(1, 1, 1, 1);		
	}data;

	ColorBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};

class SpriteBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float2 maxFrame = Float2(0, 0);
		Float2 curFrame = Float2(0, 0);
	}data;

	SpriteBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};
class HPBuffer : public ConstBuffer
{
public:
	struct Data
	{
		float value;
		float padding[3] = {};
	}data;

	HPBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};

class ExpBuffer : public ConstBuffer
{
public:
	struct Data
	{
		float value;
		float padding[3] = {};
	}data;

	ExpBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};
class BlackWhiteBuffer : public ConstBuffer
{
public:
	struct Data
	{
		int hasBlack = 0;
		float padding[3] = {};
	}data;

	BlackWhiteBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};