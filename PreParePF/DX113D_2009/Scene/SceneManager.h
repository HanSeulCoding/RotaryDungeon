#pragma once
class SceneManager : public Singleton<SceneManager>
{
private:
	friend class Singleton;
	map<int, Scene*> scenes;
	Scene* curScene;
	~SceneManager();
	SceneManager();

public:
	void Update();
	void Render();
	void PostRender();
	void PreRender();

	Scene* Add(int key, Scene* scene);
	//void ChangeScene(int key, Vector3 pos = { 0,0,0 });
	void ChangeScene(int key);
	//void ChangeScene(int loadingKey, int key, Vector2 pos = { 0,0 });

};