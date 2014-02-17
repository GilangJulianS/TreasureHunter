#include "stdafx.h"
#include "Verteks.h"

Verteks::Verteks(){

}

Verteks::Verteks(int nVerteks){
	count++;
	num = count;
	time = 0;
	state = STATE_DEFAULT;
	lengths = new int[nVerteks];
	type = CONTAIN_NOTHING;
}
	
void Verteks::setUp(int type){
	this->type = type;
	if(type == CONTAIN_RED){
		time = 1;
		nRed++;
	}
	else if (type == CONTAIN_YELLOW){
		nYellow++;
		time = 3;
	}
	else if(type == CONTAIN_GREEN){
		nGreen++;
		time = 4;
	}
}

int Verteks::getType() {
	return type;
}

int Verteks::getNum() {
	return num;
}

int Verteks::getX() {
	return x;
}

void Verteks::setX(int x) {
	this->x = x;
}

int Verteks::getY() {
	return y;
}

void Verteks::setY(int y) {
	this->y = y;
}

int Verteks::count = 0;
int Verteks::nRed = 0;
int Verteks::nYellow = 0;
int Verteks::nGreen = 0;

