#include "stdafx.h"
#include "World.h"
#include <time.h>
#include "Game.h"

using namespace std;

vector<Verteks> verteks;
vector<Peti> peti;
Game game;

World::World() {
	verteks.clear();
	Player player;
	peti.clear();
	solIdx = 0;
	srand(time(NULL));
	deltaTime = 0;
}
	
void World::initialize(){
	int in;
	int nMerah, nKuning, nHijau;
	// modal
	cin >> player.coins;
	// given
	cin >> nMerah;
	cin >> nKuning;
	cin >> nHijau;
	for(int i=0; i<nMerah; i++){
		cin >> in;
		Peti p(in-1, Peti::RED);
		peti.push_back(p);
	}
	for(int i=0; i<nKuning; i++){
		cin >> in;
		Peti p(in-1, Peti::YELLOW);
		peti.push_back(p);
	}
	for(int i=0; i<nHijau; i++){
		cin >> in;
		Peti p(in-1, Peti::GREEN);
		peti.push_back(p);
	}
	cin >> in;
	player.curVerteks = in-1;  //posisi player -1 (verteks mulai dari 0)
	cin >> minScore;
	cin >> minTime;
	cin >> nVerteks;
	jarak = new int[nVerteks];
	jalan = 1;
	solution = new int[nVerteks];
	int randX, randY;
	bool avail;
	int difX, difY;
	for(int i=0;i<nVerteks;i++){
		Verteks v(nVerteks);
		do{
			avail = true;
			randX = (rand()%20)*50;
			randY = (rand()%12)*50;
			for(int j=0; j<i; j++){
				difX = randX - verteks[j].getX();
				difY = randY - verteks[j].getY();
				if(difX <0)
					difX *= -1;
				if(difY <0)
					difY *= -1;
				if(difX + difY <= 150){
					avail = false;
					break;
				}
			}
		}while(!avail);
		v.set(randX, randY);
		for(int j=0;j<nVerteks;j++){
			cin >> v.lengths[j];
		}
		verteks.push_back(v);
	}
	int n = nMerah + nKuning + nHijau;
	Peti p;
	for(int i=0;i<n;i++){
		p = peti[i];
		if(p.type == Peti::RED)
			verteks[p.loc].type = Verteks::CONTAIN_RED;
		if(p.type == Peti::YELLOW)
			verteks[p.loc].type = Verteks::CONTAIN_YELLOW;
		if(p.type == Peti::GREEN)
			verteks[p.loc].type = Verteks::CONTAIN_GREEN;
	}
	//
	player.x = verteks[player.curVerteks].getX();
	player.y = verteks[player.curVerteks].getY();
}



void World::update(int elapsedTime){
	deltaTime += elapsedTime;
	float travelX = 0, travelY = 0;

	//std::cout << "delta" << elapsedTime << endl;

	if(deltaTime >= 17){
		
		deltaTime = 0;
		/*for (i = 0; i <nVerteks; i++){
			cout << solution[i] << endl;
		} */ 
		player.destVerteks = verteks[solution[jalan]].getNum() - 1;
		player.curVerteks = verteks[solution[jalan-1]].getNum() - 1;
		/*if(player.x == verteks[player.destVerteks].getX() && player.y == verteks[player.destVerteks].getY()){
			i++;
		}*/
		//cout << jalan << endl;
		if(verteks[player.destVerteks].bound.intersects(player.bound)){
			player.set(verteks[player.destVerteks].bound.left, verteks[player.destVerteks].bound.top);
			if(verteks[player.destVerteks].getType() == verteks[player.destVerteks].CONTAIN_GREEN){
				sleep(milliseconds(500));
			}
			else if (verteks[player.destVerteks].getType() == verteks[player.destVerteks].CONTAIN_YELLOW){
				sleep(milliseconds(1000));
			}
			else if (verteks[player.destVerteks].getType() == verteks[player.destVerteks].CONTAIN_RED){
				sleep(milliseconds(1500));
			}
			jalan++;
			//player.curVerteks = verteks[solution[jalan-1]].getNum() - 1;
		}
		//std::cout << "Cur" << verteks[player.curVerteks].bound.left << " " << verteks[player.curVerteks].bound.top << endl;
		//std::cout << "Dest" << verteks[player.destVerteks].bound.left << " " << verteks[player.destVerteks].bound.top << endl;
		//std::cout << "Player" << player.bound.left << " " << player.bound.top << endl << endl;
		travelX = ((verteks[player.destVerteks].centerX - verteks[player.curVerteks].centerX) / verteks[player.curVerteks].lengths[player.destVerteks]) / 20;
		travelY = ((verteks[player.destVerteks].centerY - verteks[player.curVerteks].centerY) / verteks[player.curVerteks].lengths[player.destVerteks]) / 20;
		player.move(travelX, travelY);
		//cout<< player.x<< " " << player.y << endl;
		//cout << "jarak :" << verteks[player.curVerteks].lengths[player.destVerteks]<< endl;
		//cout<< "dest "<<verteks[player.destVerteks].getX() << " " << verteks[player.destVerteks].getY() << endl;
		//cout<< "cur " <<verteks[player.curVerteks].getX() << " " << verteks[player.curVerteks].getY() << endl;
		//cout<<"player " << player.x << " " << player.y <<endl;
	}
}


void World::solve(){
	int j, point;
	int jaraktempuh = 0;
	int minLength, idxL, idxR, idx;
	float minRatio, ratio;
	Verteks curVerteks = verteks[player.curVerteks];
	//System.out.println(player.curVerteks);
	idxL = player.curVerteks;
	solution[solIdx] = idxL;
	solIdx++;
	for(int i=1; i<nVerteks; i++){
		j = 0;
		minLength = 9999;
		minRatio = 9999;
		idxL = -1;
		idxR = -1;
		while(j<nVerteks){
			if((j!=player.curVerteks) && (curVerteks.lengths[j] < minLength) && (curVerteks.lengths[j]!=-99)){
				if(!isDuplicate(j)){
					minLength = curVerteks.lengths[j];
					idxL = j;
				}
			}
			switch(verteks[j].type){
			case Verteks::CONTAIN_RED:
				point = 2;
				break;
			case Verteks::CONTAIN_YELLOW:
				point = 1;
				break;
			default:
				point = 999;
			}
				
			if(j!=player.curVerteks && (point!=999) && (curVerteks.lengths[j]!=-99)){
				ratio = (float)curVerteks.lengths[j]/(float)point;
				if(ratio < minRatio){
					if(!isDuplicate(j)){
						minRatio = ratio;
						idxR = j;
					}
				}
			}
			j++;
		}
		if(idxR!=-1)
			idx = idxR;
		else
			idx = idxL;
		player.curVerteks = idx;
		curVerteks = verteks[idx];
		jaraktempuh += minLength;
		solution[solIdx] = idx;
		solIdx++;
	}
	for(int i=0; i<solIdx; i++){
		cout << verteks[solution[i]].getNum();
	}
	cout << endl;
}

bool World::isDuplicate(int j){
	bool duplicate = false;
	for(int k=0; k<solIdx; k++){
		if(j == solution[k]){
			duplicate = true;
		}
	}
	return duplicate;
}



vector<Verteks> World::getVertices(){
	return verteks;
}

vector<Peti> World::getPeti(){
	return peti;
}

void World::generatePos(){
	
}
