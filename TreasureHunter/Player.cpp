#include "stdafx.h"
#include "Player.h"


Player::Player() {
	coins = 0;
	nTools1x = 0;
	nTools2x = 0;
	nTools4x = 0;
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