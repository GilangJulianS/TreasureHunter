#include "stdafx.h"
#include "Player.h"


Player::Player() {
	coins = 0;
	nTools1x = 0;
	nTools2x = 0;
	nTools4x = 0;
	delayTime = 0;
	timeToGo = 0;
	timeToGoCounter = 0;
	nVisitedVertices = 1;
	bukaPeti = false;
}

void Player::set(float x, float y){
	this->x = x;
	this->y = y;
	bound = sf::FloatRect(x, y, Player::DEFAULT_WIDTH, Player::DEFAULT_HEIGHT);
	colliBound = sf::FloatRect(x+20, y+20, 10, 10);
}

void Player::setCenter(float x, float y){
	this->x = x-25;
	this->y = y-25;
	bound = sf::FloatRect(this->x, this->y, Player::DEFAULT_WIDTH, Player::DEFAULT_HEIGHT);
	colliBound = sf::FloatRect(x-5, y-5, 10, 10);
}

void Player::move(float x, float y){
	this->x += x;
	this->y += y;
	bound.left = this->x;
	bound.top = this->y;
	bound.height = Player::DEFAULT_HEIGHT;
	bound.width = Player::DEFAULT_WIDTH;
	colliBound.left = this->x + 20;
	colliBound.top = this->y + 20;
	colliBound.height = 10;
	colliBound.width = 10;
}

void Player::buyTool(Tool tool){
	if(coins >= tool.price){
		coins -= tool.price;
		switch(tool.type){
		case Tool::TYPE_NORMAL:
			nTools1x++;
			break;
		case Tool::TYPE_2X:
			nTools2x++;
			break;
		case Tool::TYPE_4X:
			nTools4x++;
			break;
		}
	}
}

void Player::useTool(int type){
	switch(type){
	case Tool::TYPE_NORMAL:
		nTools1x--;
		break;
	case Tool::TYPE_2X:
		nTools2x--;
		break;
	case Tool::TYPE_4X:
		nTools4x--;
		break;
	}
}