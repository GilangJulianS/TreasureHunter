#include "stdafx.h"
#include "Player.h"

std::vector<Tool> tools;

Player::Player() {
	tools.clear();
	coins = 0;
	nVisitedVertices = 1;
}