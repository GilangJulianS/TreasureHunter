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
	static void start();
	static void run();
	
private:
	static bool isExiting();
	static void gameLoop();

	enum GameState { UNINITIALIZED, SPLASH, PAUSE, MENU, PLAY, EXIT};

	static GameState gameState;
	static RenderWindow mainWindow;
	
};