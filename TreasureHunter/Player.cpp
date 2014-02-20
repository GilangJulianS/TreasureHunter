#include "stdafx.h"
#include "Player.h"

std::vector<Tool> tools;

Player::Player() {
	tools.clear();
	coins = 0;
	nVisitedVertices = 1;
}

void Player::set(float x, float y){
	this->x = x;
	this->y = y;
	bound = sf::FloatRect(x, y, Player::DEFAULT_WIDTH, Player::DEFAULT_HEIGHT);
}

void Player::move(float x, float y){
	this->x += x;
	this->y += y;
	bound.left = this->x;
	bound.top = this->y;
	bound.height = Player::DEFAULT_HEIGHT;
	bound.width = Player::DEFAULT_WIDTH;
}

void Player::buyTool(Tool tool){
	if(coins >= tool.price){
		coins -= tool.price;
		tools.push_back(tool);
	}
}