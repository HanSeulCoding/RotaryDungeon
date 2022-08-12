#pragma once
#include "Framework/Math/Vector3.h"

#define KEYMAX 255

class Control : public Singleton<Control>
{
private:
	friend class Singleton;

	enum {
		NONE,
		DOWN,
		UP,
		PRESS
	};

	BYTE curState[KEYMAX];
	BYTE oldState[KEYMAX];
	BYTE mapState[KEYMAX];

	Vector3 mousePos;

	Control();
	~Control();
public:	
	void Update();

	bool Down(UINT key) { return mapState[key] == DOWN; }
	bool Up(UINT key) { return mapState[key] == UP; }
	bool Press(UINT key) { return mapState[key] == PRESS; }

	void SetMousePos(LPARAM lParam);

	Vector3 GetMousePos() { return mousePos; }
};