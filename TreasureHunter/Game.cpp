#include "stdafx.h"
#include "Game.h"
#include "SplashScreen.h"
#include "GameScreen.h"
#include "World.h"
#include <thread>


GameScreen gameScreen;
World world;

void Game::start(){
	if(gameState != UNINITIALIZED)
		return;
	
	//SplashScreen splash;
	//splash.show(mainWindow);
	gameState = Game::PLAY;

	//
	world.initialize();


	sf::Thread thread1(&Game::run);
	//Game::run();
	thread1.launch();
	world.solve();
	

	//
	/*while(!isExiting()){
		gameLoop();
	}*/
	//thread1.wait();
	//mainWindow.close();
}

bool Game::isExiting(){
	if(gameState == GameState::EXIT)
		return true;
	else
		return false;
}

void Game::gameLoop(){
	sf::Event curEvent;
	while(mainWindow.pollEvent(curEvent)){
		/*switch(gameState){
		case Game::PLAY:
			mainWindow.clear(sf::Color(0,255,255));
			mainWindow.display();
			if(curEvent.type == sf::Event::Closed)
				gameState = Game::EXIT;
			break;
		}*/
	}
}

void Game::run(){
	ContextSettings settings;
	bool finishState = false;
	settings.antialiasingLevel = 8;
	mainWindow.create(VideoMode(1024,600,32),"TreasureHunter", Style::Default ,settings);
	sf::Clock clock;
	while(!finishState){
		sf::Time elapsedTime = clock.restart();
		//std::cout << elapsedTime.asMilliseconds() <<"time" << std::endl;
		gameScreen.update(mainWindow, world);
		world.update(elapsedTime.asMilliseconds());
		
		if (world.jalan > world.nVerteks - 1){
			finishState = true;
		}
		sleep(milliseconds(17));
	}
}

Game::GameState Game::gameState = UNINITIALIZED;
sf::RenderWindow Game::mainWindow;