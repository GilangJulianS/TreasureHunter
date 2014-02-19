#include "stdafx.h"
#include "Game.h"
#include "Button.h"
#include "SplashScreen.h"
#include "Game.h"
#include <thread>
#include <time.h>

using namespace std;


bool finishState = false;
bool startClicked = false;
bool drag = false;
int dragVerteks = -1;
float x, y;
Event event;
ContextSettings settings;


vector<Verteks> verteks;
vector<Peti> peti;

sf::Text text;
std::string str;

Game::Game(){
	verteks.clear();
	peti.clear();
	solIdx = 0;
	srand(time(NULL));
	deltaTime = 0;

	if(gameState != UNINITIALIZED)
		return;
	
	//SplashScreen splash;
	//splash.show(mainWindow);
	gameState = Game::PLAY;

	//
	initialize();
	
	settings.antialiasingLevel = 8;
	mainWindow.create(VideoMode(Game::SCREEN_SIZE_X,Game::SCREEN_SIZE_Y,32),"TreasureHunter", Style::Default ,settings);
	Button startButton(0, 0, 50, 50);
	init();
	
	while(!startClicked){
		while(mainWindow.pollEvent(event)){
			if(startButton.bound.contains(event.mouseButton.x, event.mouseButton.y)){
				startClicked = true;
				mainWindow.close();
			}
			if(event.type == Event::Closed)
				mainWindow.close();
			if(event.type == Event::MouseButtonPressed && !drag){
				for(int i=0;i<nVerteks;i++){
					if(verteks[i].bound.contains(event.mouseButton.x, event.mouseButton.y))
						dragVerteks = i;
				}
				drag = true;
			}
			if(event.type == Event::MouseButtonReleased && drag){
				drag = false;
				dragVerteks = -1;
			}
			if(drag && dragVerteks!= -1){
				x = event.mouseMove.x;
				y = event.mouseMove.y;
				if(x > 0 && y > 0 && x < (float)Game::SCREEN_SIZE_X && y < (float)Game::SCREEN_SIZE_Y)
					verteks[dragVerteks].set(x - Verteks::VERTEKS_RADIUS, y - Verteks::VERTEKS_RADIUS);
			}
			posUpdate();
			draw();
			mainWindow.draw(startButton.texture);
			mainWindow.display();
		}
	}

	sf::Thread thread1(&Game::run, this);
	//Game::run();
	thread1.launch();
	solve();
	

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
	
	
	mainWindow.create(VideoMode(Game::SCREEN_SIZE_X,Game::SCREEN_SIZE_Y,32),"TreasureHunter", Style::Default ,settings);
	sf::Clock clock;
	while(!finishState){
		sf::Time elapsedTime = clock.restart();
		//std::cout << elapsedTime.asMilliseconds() <<"time" << std::endl;
		draw();
		update(elapsedTime.asMilliseconds());
		mainWindow.display();
		if (jalan > nVerteks - 1){
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
	
void Game::initialize(){
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
}

void Game::update(int elapsedTime){
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
				player.coins++;
			}
			else if (verteks[player.destVerteks].getType() == verteks[player.destVerteks].CONTAIN_RED){
				sleep(milliseconds(1500));
				player.coins +=2;
			}
			jalan++;
			cout << player.coins << endl;
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

void Game::posUpdate(){
	for(int i=0;i<nVerteks;i++){
		v = verteks[i];
		Vector2f f2((float)(v.getX() + Verteks::VERTEKS_RADIUS), (float)(v.getY() + Verteks::VERTEKS_RADIUS));
		positions[i] = f2;
	}
	player.x = verteks[player.curVerteks].getX();
	player.y = verteks[player.curVerteks].getY();
}

void Game::solve(){
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

void Game::init(){
	positions = new Vector2f[nVerteks];
	// get position 
	if(texture.loadFromFile("Bitmap/Box.png")!=true){
		std::cout << "Image file Box.png failed to load" << std::endl;
		return;
	}
	if(character.loadFromFile("Bitmap/char.png")!=true){
		std::cout << "Image file char.png failed to load" << std::endl;
		return;
	}
	sprite = Sprite(texture);
	sprite.scale(0.05f, 0.05f);
	playerSprite = Sprite(character);
	playerSprite.scale(0.175f, 0.175f);
	circle = CircleShape(Verteks::VERTEKS_RADIUS);
	if(font.loadFromFile("comic.ttf")!=true){
		cout << "failed load font" <<endl;
	}
	text  = Text("",font);
	text.setCharacterSize(20);
}

void Game::draw(){
	// Draw Grid
	/*for(int i=1; i<21; i++){
		Vertex line[] = {
			Vertex(Vector2f(50*i, 0)),
			Vertex(Vector2f(50*i, 600))
		};
		mainWindow.draw(line, 2, Lines);
	}
	for(int i=1; i<13; i++){
		Vertex line[] = {
			Vertex(Vector2f(0, 50*i)),
			Vertex(Vector2f(1024, 50*i))
		};
		mainWindow.draw(line, 2, Lines);
	}*/
	mainWindow.clear();
	// draw lines between verteks
	for(int i=0; i<nVerteks;i++){
		v = verteks[i];
		for(int j=0;j<nVerteks;j++){
			if(v.lengths[j] != 0 && v.lengths[j]!=-99){
				Vertex line[] = {
					Vertex(positions[i]),
					Vertex(positions[j])
				};
				mainWindow.draw(line, 2, Lines);
			}
		}
	}
	// draw verteks
	for(int i=0;i<nVerteks;i++){
		v = verteks[i];
		circle.setPosition(v.getX(), v.getY());
		if(i == dragVerteks)
			circle.setFillColor(Color::Blue);
		else
			circle.setFillColor(Color::White);
		mainWindow.draw(circle);
		if(v.getType() == Verteks::CONTAIN_RED){
			sprite.setColor(Color::Red);
			sprite.setPosition(v.getX(), v.getY());
			mainWindow.draw(sprite);
		}
		else if(v.getType() == Verteks::CONTAIN_YELLOW){
			sprite.setColor(Color::Yellow);
			sprite.setPosition(v.getX(), v.getY());
			mainWindow.draw(sprite);
		}
		else if(v.getType() == Verteks::CONTAIN_GREEN){
			sprite.setColor(Color::Green);
			sprite.setPosition(v.getX(), v.getY());
			mainWindow.draw(sprite);
		}
	}
	// draw text
	int length;
	for(int i=0; i<nVerteks-1;i++){
		for(int j=i+1;j<nVerteks;j++){
			length = verteks[i].lengths[j];
			if(length != 0 && length != -99){
				str = std::to_string(length);
				text.setString(str);
				text.setPosition(midPoint(positions[i], positions[j]));
				mainWindow.draw(text);
			}
		}
	}
	playerSprite.setPosition(player.bound.left, player.bound.top);
	mainWindow.draw(playerSprite);
	

}

Vector2f Game::midPoint(Vector2f v1, Vector2f v2){
	float x, y;
	x = (v1.x + v2.x)/2;
	y = (v1.y + v2.y)/2;
	Vector2f f(x,y);
	return f;
}

bool Game::isDuplicate(int j){
	bool duplicate = false;
	for(int k=0; k<solIdx; k++){
		if(j == solution[k]){
			duplicate = true;
		}
	}
	return duplicate;
}

vector<Verteks> Game::getVertices(){
	return verteks;
}

vector<Peti> Game::getPeti(){
	return peti;
}

void Game::generatePos(){
	
}

Game::GameState Game::gameState = UNINITIALIZED;