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

	float f;//����ȿ�� f = g + h
	float g;//����� ���� n������ ���
	float h;//n���� ������ ������ ���� ���

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