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
#include <iostream>
#include <fstream>
#include <cstdio>


using namespace std;
using namespace sf;

class Game{

public:
	static const int SCREEN_SIZE_X = 1366;
	static const int SCREEN_SIZE_Y = 768;
	static const int MAX_SLEEP_TIME = 25;
	static const int P1_TOOL_1X = 0;
	static const int P1_TOOL_2X = 1;
	static const int P1_TOOL_4X = 2;
	static const int P2_TOOL_1X = 3;
	static const int P2_TOOL_2X = 4;
	static const int P2_TOOL_4X = 5;
	static const int AI_MODE = 0;
	static const int PLAYER_MODE = 1;

	Game();
	void input();
	void posUpdate();
	//
	static RenderWindow mainWindow;
	Player player1, player2;
	int *solution;
	int *solutionTools;
	int *jarak;
	int jalan;
	int nTools;
	int minScore, maxTime, nVerteks, solIdx;
	bool *isi;

	void initialize();

	void start();

	void solve1();
	void solve2();
	void solve3();

	void update(int elapsedTime);

	void init();
	
	void draw();

	void selectTool();

	void slide();

	void processEvent(sf::Event event);

	void prepare();
	
	sf::Vector2f *positions;
	enum GameState { FRONT, MENU, SINGEPLAYER, DOUBLEPLAYER, AIMODE, PLAY };
	enum GreedyMode{ TYPE_1, TYPE_2, TYPE_3 };

	static GameState gameState;
	static GreedyMode greedyMode;
	static int gameMode;
	static bool isCreated;

	FILE *file;
private:
	bool finishState;
	bool startClicked;
	bool drag;
	bool dragSlider;
	bool hoverSlider;
	bool showSelection;
	bool toolClicked;
	bool selectTime;
	bool toolTime;
	bool requestDraw;
	int dragVerteks;
	int hoverVerteks;
	int pointedTool;
	float lastMouseX;
	float lastMouseY;
	int timeCounter;
	int delayCounter;
	int elapsedTimeUnit;
	
	float x, y;

	
	
	
	//
	bool isDuplicate(int j);
	int deltaTime;
	int delayTime;

	sf::Vector2f midPoint(sf::Vector2f v1, sf::Vector2f v2);
	sf::Font font;
	
	Verteks v;

	
	Sprite box;
	sf::RectangleShape character1, character2;
	Sprite sideBar;
	Sprite slider;
	Sprite tool1xp1;
	Sprite tool2xp1;
	Sprite tool4xp1;
	Sprite tool1xp2;
	Sprite tool2xp2;
	Sprite tool4xp2;
	Sprite coin;
	Sprite coin2;
	Texture coinTexture;
	Texture tool1xTexture;
	Texture tool2xTexture;
	Texture tool4xTexture;
	Texture boxTexture;
	Texture characterTexture;
	Texture sideBarTexture;
	Texture sliderTexture;
	Texture playBGTexture;
	Texture playButtonTexture;
	RectangleShape playBG;
	RectangleShape playButton;
	CircleShape circle;
	RectangleShape selectionRect;
	Vertex *slideBar;

};