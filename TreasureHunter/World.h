
#pragma once
#include "Verteks.h"
#include "Peti.h"
#include "Player.h"
#include <vector>


class World {

public:
	
	Player player;
	int *solution;
	int *jarak;
	int jalan;
	int minScore, minTime, nVerteks, solIdx;
	World();
	
	void initialize();

	void solve();

	void generatePos();

	void update(int elapsedTime);

	std::vector<Verteks> getVertices();

	std::vector<Peti> getPeti();




private:
	bool isDuplicate(int j);
	int deltaTime;
};
