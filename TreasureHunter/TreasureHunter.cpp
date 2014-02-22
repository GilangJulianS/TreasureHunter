// TreasureHunter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Game.h"
#include "Button.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Game game;
	float lastMouseX = 0, lastMouseY = 0;
	int x = 0, y = 0;
	bool loading = true;

	ContextSettings settings;
	settings.antialiasingLevel = 4;
	Game::mainWindow.create(VideoMode(1366,768,32), "Treasure Hunter(One Piece Edition)",Style::Fullscreen, settings);
	sf::Sprite loadingScreen;
	sf::Texture loadTexture;
	loadTexture.loadFromFile("Bitmap/Loading.png");
	loadingScreen = Sprite(loadTexture);
	Game::mainWindow.draw(loadingScreen);
	Game::mainWindow.display();

	#pragma region
	sf::Texture luffyButton,luffyButton2, namiButton, namiButton2, zoroButton,zoroButton2, playButtonTexture, playSelectedTexture;
	sf:: RectangleShape luffyButtonShape, namiButtonShape, zoroButtonShape;
	sf::Sprite playButton;
	
	

	luffyButton.loadFromFile("Bitmap/luffybw.jpg");
	luffyButton2.loadFromFile("Bitmap/luffy.jpg");
	luffyButtonShape.setSize(sf::Vector2f(300,500));
	luffyButtonShape.setPosition(662,390);
	luffyButtonShape.setOrigin(150, 250);
	luffyButtonShape.setTexture(&luffyButton);

	namiButton.loadFromFile("Bitmap/namibw.jpg");
	namiButton2.loadFromFile("Bitmap/nami.jpg");
	namiButtonShape.setSize(sf::Vector2f(300,500));
	namiButtonShape.setPosition(242,435);
	namiButtonShape.setOrigin(150,250);
	namiButtonShape.setTexture(&namiButton);

	zoroButton.loadFromFile("Bitmap/zorobw.jpg");
	zoroButton2.loadFromFile("Bitmap/zoro.jpg");
	zoroButtonShape.setSize(sf::Vector2f(300,500));
	zoroButtonShape.setPosition(1085,435);
	zoroButtonShape.setOrigin(150,250);
	zoroButtonShape.setTexture(&zoroButton);
	

	playButtonTexture.loadFromFile("Bitmap/PlayButton.png");
	playSelectedTexture.loadFromFile("Bitmap/PlaySelected.png");
	playButton = sf::Sprite(playButtonTexture);
	playButton.setScale(0.6f, 0.6f);
	playButton.setPosition(683, 614);
	playButton.setOrigin(132.5f, 43);
	#pragma endregion Button AI mode

	#pragma region
	sf::Texture backButton;sf::RectangleShape backButtonShape;
	backButton.loadFromFile("Bitmap/backButton.png");
	backButtonShape.setSize(sf::Vector2f(200,80));
	backButtonShape.setOrigin(100,40);
	#pragma endregion Button Back

	#pragma region

	sf::Texture frontBG, menuBG, AImodeBG;
	sf::RectangleShape frontBGShape,AImodeBGShape;
	frontBG.loadFromFile("Bitmap/frontBG.jpg");
	menuBG.loadFromFile("Bitmap/menuBG.jpg");
	frontBGShape.setSize(sf::Vector2f(1366,768));
	AImodeBG.loadFromFile("Bitmap/AImodeBG.jpg");

	#pragma endregion Background

	#pragma region

	sf::Texture AImodeButton,pvpButton, vsAIButton, watchBot;
	sf::RectangleShape pvpButtonShape, watchBotShape, vsAIButtonShape;
	vsAIButton.loadFromFile("Bitmap/AIMode.png");
	/*vsAIButtonShape.setPosition(1070,269);
	vsAIButtonShape.setSize(sf::Vector2f(220,250));
	vsAIButtonShape.setOrigin(110,125);*/
	vsAIButtonShape.setPosition(1070,340);
	vsAIButtonShape.setSize(sf::Vector2f(500,647));
	vsAIButtonShape.setOrigin(250,324);
	vsAIButtonShape.setTexture(&vsAIButton);
	/*pvpButton2.loadFromFile("Bitmap/pvp.png");
	pvpButtonShape2.setSize(sf::Vector2f(139,250));
	pvpButtonShape2.setPosition(53,158);*/
	pvpButton.loadFromFile("Bitmap/PlayerMode.png");
	pvpButtonShape.setSize(sf::Vector2f(500,647));
	pvpButtonShape.setPosition(294,340);
	pvpButtonShape.setOrigin(250, 324);
	/*pvpButtonShape.setSize(sf::Vector2f(225,245));
	pvpButtonShape.setPosition(294,290);
	pvpButtonShape.setOrigin(112, 122);*/
	pvpButtonShape.setTexture(&pvpButton);
	/*watchBot.loadFromFile("Bitmap/watchBot.png");
	watchBotShape.setSize(sf::Vector2f(261,208));
	watchBotShape.setPosition(138,540);
	watchBotShape.setTexture(&watchBot);*/
	#pragma endregion Menu Item

	#pragma region

	
	backButtonShape.setTexture(&backButton);
	
	

	#pragma endregion Initialize

	Game::gameState = Game::GameState::FRONT;
	

	while (!Game::isCreated){
		sf::Event event123;
		if(Game::gameState == Game::GameState::FRONT){
			#pragma region
			frontBGShape.setTexture(&frontBG);
			while(Game::mainWindow.pollEvent(event123)){
				if(event123.mouseMove.x != 0 && event123.mouseMove.y != 0){
					lastMouseX = (float)event123.mouseMove.x;
					lastMouseY = (float)event123.mouseMove.y;
				}
				switch (event123.type)
				{
					case sf::Event::Closed:
						Game::mainWindow.close();
						break;
					case sf::Event::MouseMoved:
						if (playButton.getGlobalBounds().contains(lastMouseX, lastMouseY)){
							playButton.setTexture(playSelectedTexture);
							playButton.setScale(0.5f, 0.5f);
						}
						else{
							playButton.setTexture(playButtonTexture);
							playButton.setScale(0.6f, 0.6f);
						}
						break;
					case sf::Event::KeyPressed:
						if (event123.key.code == sf::Keyboard::Escape){
							Game::mainWindow.close();
						}
						break;
					case sf::Event::MouseButtonPressed :
						if (playButton.getGlobalBounds().contains(lastMouseX, lastMouseY)){
							Game::gameState = Game::GameState::MENU;
						}
						cout << event123.mouseButton.x << " " << event123.mouseButton.y << endl;
					default:
						break;
				}
			}
			Game::mainWindow.draw(frontBGShape);
			Game::mainWindow.draw(playButton);
			#pragma endregion FrontPage
		}
		if(Game::gameState == Game::GameState::MENU){
			#pragma region
			frontBGShape.setTexture(&menuBG);
			while(Game::mainWindow.pollEvent(event123)){
				if(event123.mouseMove.x != 0 && event123.mouseMove.y != 0){
					lastMouseX = (float)event123.mouseMove.x;
					lastMouseY = (float)event123.mouseMove.y;
				}
				switch (event123.type)
				{
					case sf::Event::Closed:
						Game::mainWindow.close();
						break;

					case sf::Event::MouseMoved:
						if(vsAIButtonShape.getGlobalBounds().contains(lastMouseX, lastMouseY))
							vsAIButtonShape.setScale(0.9f, 0.9f);
						else
							vsAIButtonShape.setScale(1, 1);
						if(pvpButtonShape.getGlobalBounds().contains(lastMouseX, lastMouseY))
							pvpButtonShape.setScale(0.9f, 0.9f);
						else
							pvpButtonShape.setScale(1,1);
						if(backButtonShape.getGlobalBounds().contains(lastMouseX, lastMouseY))
							backButtonShape.setScale(0.9f, 0.9f);
						else
							backButtonShape.setScale(1, 1);
						break;

					case sf::Event::KeyPressed:
						if (event123.key.code == sf::Keyboard::Escape){
							Game::mainWindow.close();
						}
						break;

					case sf::Event::MouseButtonPressed :
						if (vsAIButtonShape.getGlobalBounds().contains(lastMouseX, lastMouseY)){
							Game::gameState = Game::GameState::AIMODE;
							Game::gameMode = Game::AI_MODE;
						}
						if(pvpButtonShape.getGlobalBounds().contains(lastMouseX, lastMouseY)){
							Game::gameState = Game::GameState::AIMODE;
							Game::gameMode = Game::PLAYER_MODE;
						}
						if (backButtonShape.getGlobalBounds().contains(lastMouseX, lastMouseY)){
							Game::gameState = Game::GameState::FRONT;
						}
					default:
						break;
				}
			}
			#pragma endregion MENU
		}
		if(Game::gameState == Game::GameState::AIMODE){
			#pragma region
			frontBGShape.setTexture(&AImodeBG);
			while(Game::mainWindow.pollEvent(event123)){
				if(event123.mouseMove.x != 0 && event123.mouseMove.y != 0){
					lastMouseX = (float)event123.mouseMove.x;
					lastMouseY = (float)event123.mouseMove.y;
				}
				switch (event123.type)
				{
					case sf::Event::Closed:
						Game::mainWindow.close();
						break;

					case sf::Event::MouseMoved:
						#pragma region
						if (namiButtonShape.getGlobalBounds().contains(lastMouseX, lastMouseY)){
							namiButtonShape.setTexture(&namiButton2);
							namiButtonShape.setScale(0.9f, 0.9f);
						}
						else if (luffyButtonShape.getGlobalBounds().contains(lastMouseX, lastMouseY)){
							luffyButtonShape.setTexture(&luffyButton2);
							luffyButtonShape.setScale(0.9f, 0.9f);
						}
						else if (zoroButtonShape.getGlobalBounds().contains(lastMouseX, lastMouseY)){
							zoroButtonShape.setTexture(&zoroButton2);
							zoroButtonShape.setScale(0.9f, 0.9f);
						}
						else{
							luffyButtonShape.setTexture(&luffyButton);
							namiButtonShape.setTexture(&namiButton);
							zoroButtonShape.setTexture(&zoroButton);
							luffyButtonShape.setScale(1,1);
							namiButtonShape.setScale(1,1);
							zoroButtonShape.setScale(1,1);
						}
						if(backButtonShape.getGlobalBounds().contains(lastMouseX, lastMouseY))
							backButtonShape.setScale(0.9f, 0.9f);
						else
							backButtonShape.setScale(1, 1);
						break;
						#pragma endregion Button Change
					case sf::Event::KeyPressed:
						if (event123.key.code == sf::Keyboard::Escape){
							Game::gameState = Game::GameState::MENU;
						}
						break;
					case sf::Event::MouseButtonPressed :
						if (backButtonShape.getGlobalBounds().contains(lastMouseX, lastMouseY))
							Game::gameState = Game::GameState::MENU;
						if(namiButtonShape.getGlobalBounds().contains(lastMouseX, lastMouseY)){
							Game::greedyMode = Game::GreedyMode::TYPE_1;
							Game::isCreated = true;
						}
						if(luffyButtonShape.getGlobalBounds().contains(lastMouseX, lastMouseY)){
							Game::greedyMode = Game::GreedyMode::TYPE_2;
							Game::isCreated = true;
						}
						if(zoroButtonShape.getGlobalBounds().contains(lastMouseX, lastMouseY)){
							Game::greedyMode = Game::GreedyMode::TYPE_3;
							Game::isCreated = true;
						}
						cout << event123.mouseButton.x << " " << event123.mouseButton.y << endl;
						break;
					default:
						break;
				}
			}	
			#pragma endregion AIMODE
		}
		
		if (Game::gameState == Game::GameState::MENU){
			Game::mainWindow.draw(frontBGShape);
			backButtonShape.setPosition(1100,688);
			Game::mainWindow.draw(pvpButtonShape);
			Game::mainWindow.draw(backButtonShape);
			//Game::mainWindow.draw(watchBotShape);
			Game::mainWindow.draw(vsAIButtonShape);
			
		}
		if (Game::gameState == Game::GameState::AIMODE){
			Game::mainWindow.draw(frontBGShape);
			backButtonShape.setPosition(660,678);
			Game::mainWindow.draw(namiButtonShape);
			Game::mainWindow.draw(luffyButtonShape);
			Game::mainWindow.draw(zoroButtonShape);
			Game::mainWindow.draw(backButtonShape);
			
		}
		Game::mainWindow.display();
		if(Game::isCreated){
			Game::mainWindow.draw(loadingScreen);
			Game::mainWindow.display();
			game.start();
		}

	}
	

	return 0;
}

