#pragma once
#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"

#include <iostream>
#include <string>
#include <list>
#include "Verteks.h"
#include "Peti.h"
#include "Player.h"
#include <vector>

using namespace std;
using namespace sf;

class Game{

public:
	static const int SCREEN_SIZE_X = 1024;
	static const int SCREEN_SIZE_Y = 600;

	Game();
	void run();
	void input();
	void posUpdate();
	//

	Player player;
	int *solution;
	int *jarak;
	int jalan;
	int minScore, minTime, nVerteks, solIdx;
	
	void initialize();

	void solve();

	void generatePos();

	void update(int elapsedTime);

	std::vector<Verteks> getVertices();

	std::vector<Peti> getPeti();

	//

	void init();
	void draw();
	sf::Vector2f *positions;

private:
	bool isExiting();
	void gameLoop();
	
	enum GameState { UNINITIALIZED, SPLASH, PAUSE, MENU, PLAY, EXIT};

	static GameState gameState;
	RenderWindow mainWindow;
	
	//
	bool isDuplicate(int j);
	int deltaTime;

	sf::Vector2f midPoint(sf::Vector2f v1, sf::Vector2f v2);
	sf::Font font;
	
	Verteks v;

	
	Sprite sprite;
	Sprite playerSprite;
	Texture texture;
	Texture character;
	CircleShape circle;
};