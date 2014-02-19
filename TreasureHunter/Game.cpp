#include "stdafx.h"
#include "Game.h"
#include "Button.h"
#include "SplashScreen.h"
#include "GameScreen.h"
#include "World.h"
#include <thread>


GameScreen gameScreen;
World world;
bool finishState = false;
bool startClicked = false;
Event event;
ContextSettings settings;
	

Game::Game(){
	bool release = true;
	bool drag = false;
	settings.antialiasingLevel = 8;
	if(gameState != UNINITIALIZED)
		return;
	
	//SplashScreen splash;
	//splash.show(mainWindow);
	gameState = Game::PLAY;

	//
	world.initialize();
	
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	mainWindow.create(VideoMode(1024,600,32),"TreasureHunter", Style::Default ,settings);
	Button startButton(0, 0, 50, 50);
	gameScreen.init(world);
	
	while(!startClicked){
		while(mainWindow.pollEvent(event)){
			if(startButton.bound.contains(event.mouseButton.x, event.mouseButton.y)){
				startClicked = true;
				mainWindow.close();
			}
			if(event.type == Event::Closed){
				mainWindow.close();
			}
			gameScreen.update(mainWindow, world);
			mainWindow.draw(startButton.texture);
			mainWindow.display();
		}
	}

	sf::Thread thread1(&Game::run, this);
	//Game::run();
	thread1.launch();
	world.solve();
	

	//
	/*while(!isExiting()){
		gameLoop();
	}*/
	while(!finishState){
	}
	thread1.wait();
	mainWindow.close();
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
	
	
	mainWindow.create(VideoMode(1024,600,32),"TreasureHunter", Style::Default ,settings);
	gameScreen.init(world);
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

void Game::input(){
	while(!finishState){
		while(mainWindow.pollEvent(event)){
			if(event.type == Event::EventType::MouseButtonPressed){
				return;
			}
		}
	}
}

Game::GameState Game::gameState = UNINITIALIZED;