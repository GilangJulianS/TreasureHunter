#pragma once
#include "Tool.h"

class Player {

public:
	static const int DEFAULT_HEIGHT = 40;
	static const int DEFAULT_WIDTH = 40;
	int curVerteks, destVerteks;
	int nVisitedVertices;
	int coins, nTools1x, nTools2x, nTools4x;
	sf::FloatRect bound;
	// constructor
	Player();
	
	void move(float x, float y);

	void set(float x, float y);

	void buyTool(Tool tool);

	void useTool(int type);

	float x, y;

};
