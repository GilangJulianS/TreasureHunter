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
	static const int SCREEN_SIZE_X = 1366;
	static const int SCREEN_SIZE_Y = 768;
	static const int MAX_SLEEP_TIME = 25;

	Game();
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

	void update(int elapsedTime);

	void init();
	
	void draw();
	
	sf::Vector2f *positions;

private:
	

	enum GameState { UNINITIALIZED, SPLASH, PAUSE, MENU, PLAY, EXIT};

	static GameState gameState;
	RenderWindow mainWindow;
	
	//
	bool isDuplicate(int j);
	int deltaTime;
	int delayTime;

	sf::Vector2f midPoint(sf::Vector2f v1, sf::Vector2f v2);
	sf::Font font;
	
	Verteks v;

	
	Sprite box;
	Sprite character;
	Sprite sideBar;
	Sprite slider;
	Texture boxTexture;
	Texture characterTexture;
	Texture sideBarTexture;
	Texture sliderTexture;
	CircleShape circle;
	Vertex *slideBar;

};