#pragma once

class Program
{
private:
	Scene* scene;

public:
	Program();
	~Program();

	void Update();

	void PreRener();
	void Render();
	void PostRender();

private:
	void Create();
	void Delete();
	void CreateAudio();
};