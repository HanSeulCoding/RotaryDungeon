#pragma once

class Models : public ModelReader
{
private:
	struct InstanceData
	{
		Matrix world;
	};

	vector<Transform*> transforms;

	InstanceData instanceData[MAX_INSTANCE];

	VertexBuffer* instanceBuffer;

	UINT drawCount;
public:
	Models(string file);
	~Models();

	void Update();
	void Render();

	Transform* Add();

	InstanceData* GetInstanceData() { return instanceData; }
};