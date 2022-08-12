#pragma once

class ModelAnimators : public ModelAnimator
{
private:
	struct InstanceData
	{
		Matrix world;
		UINT index;
	};

	vector<Transform*> transforms;
	InstanceData instanceData[MAX_INSTANCE];

	VertexBuffer* instanceBuffer;

	class Frustum* frustum;
	UINT drawCount;

	Vector3 minBox, maxBox;

	vector<map<UINT, CallBackParam>> InstanceEndEvents;
	vector<map<UINT, int>> params;
public:
	ModelAnimators(string file);
	~ModelAnimators();

	void Update();
	void Render();

	Transform* Add();

	void PlayClip(UINT instance, UINT clip, float speed = 1.0f, float takeTime = 0.2f);

	void UpdateTransforms();

	void SetInstanceEndEvents(UINT instance, UINT clip, CallBackParam Event) 
	{
		InstanceEndEvents[instance][clip] = Event;
	}
	void SetParams(UINT instance, UINT clip, int param) 
	{
		params[instance][clip] = param;
	}
};