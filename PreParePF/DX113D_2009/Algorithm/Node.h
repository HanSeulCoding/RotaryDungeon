#pragma once

class Node
{
public:
	enum State
	{
		NONE,
		OPEN,
		CLOSED,
		USING,
		OBSTACLE
	};

private:
	friend class AStar;

	struct Edge
	{
		int index;
		float cost;		
	};

	Vector3 pos;
	int index;
	int via;

	float f;//최종효율 f = g + h
	float g;//출발지 부터 n까지의 비용
	float h;//n부터 목적지 까지의 추정 비용

	State state;

	vector<Edge*> edges;

	Collider* collider;
public:
	Node(Vector3 pos, int index);
	~Node();

	void Render();

	void AddEdge(Node* node);

	void SetState(State value) { state = value; }

	float GetCost() { return f; }
};