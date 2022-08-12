#pragma once

class TileMap;

class AStar
{
private:
	UINT width, height;

	vector<Node*> nodes;	
	Heap* heap;	

	Float2 interval;

	vector<Collider*> obstacles;
public:
	AStar(UINT width = 20, UINT height = 20);
	~AStar();

	void Update();
	void Render();

	void SetNode(class Terrain* terrain);

	void Reset();
	int FindCloseNode(Vector3 pos);

	void MakeDirectPath(IN Vector3 start, IN Vector3 end, OUT vector<Vector3>& path);

	bool CollisionObstacle(Ray ray, float destDistance);
	
	vector<Vector3> GetOrder(int start, int end);
	void SetObstacles(vector<class Item*> obstacles);
private:
	float GetDistance(int start, int end);

	int GetMinNode();
	void Extend(int center, int end);

	void SetEdge();
};