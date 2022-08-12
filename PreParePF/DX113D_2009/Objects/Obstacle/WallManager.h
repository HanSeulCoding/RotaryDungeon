#pragma once
class WallManager
{
private:
	vector<Wall*> walls;
public:
	WallManager();
	~WallManager();

	void Update();
	void Render();
};