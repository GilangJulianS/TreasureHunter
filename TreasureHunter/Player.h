#pragma once
#include "Tool.h"

class Player {

public:
	int curVerteks, destVerteks;
	int nVisitedVertices;
	int coins;
	
	// constructor
	Player();
	
	void update(int time);

	int x, y;

};
