#pragma once
#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"

#include <iostream>
#include <string>
#include <list>

using namespace std;
using namespace sf;

class Game{

public:
	Game();
	void run();
	void input();
private:
	bool isExiting();
	void gameLoop();
	
	enum GameState { UNINITIALIZED, SPLASH, PAUSE, MENU, PLAY, EXIT};

	static GameState gameState;
	RenderWindow mainWindow;
	
};