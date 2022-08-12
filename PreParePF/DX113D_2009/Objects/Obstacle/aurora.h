#pragma once
class aurora
{
private:
	Collider* collider;
	bool isChangedScene;
	bool isCollision;
	int stageNum;
public:
	aurora(Vector3 position);
	~aurora();

	void Update();
	void Render();
	void PostRender();

	bool GetIsChangedScene() { return isChangedScene; }

	Collider* GetCollider() { return collider; }
	void PlayerCollision(Collider* targetCollider);
	void SetStageNum(int value) {
		stageNum = value;
	}
};