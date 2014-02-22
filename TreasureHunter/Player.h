#pragma once
#include "Tool.h"

class Player {

public:
	static const int DEFAULT_HEIGHT = 40;
	static const int DEFAULT_WIDTH = 40;
	int curVerteks, destVerteks;
	int nVisitedVertices;
	int coins, nTools1x, nTools2x, nTools4x;
	int delayTime;
	sf::FloatRect bound;
	sf::FloatRect colliBound;
	int timeToGo;
	int timeToGoCounter;
	bool bukaPeti;
	// constructor
	Player();
	
	void move(float x, float y);

	void set(float x, float y);

	void setCenter(float x, float y);

	void buyTool(Tool tool);

	void useTool(int type);

	float x, y;

};
