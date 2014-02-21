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
bool dragSlider = false;
bool hoverSlider = false;
bool showSelection;
bool toolClicked;
int dragVerteks = -1;
int hoverVerteks = -1;
int pointedTool = -1;
int lastMouseX = -1;
int lastMouseY = -1;
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
	delayTime = 20;
	solIdx = 0;
	srand(time(NULL));
	deltaTime = 0;
	nTools = 0;
	if(gameState != UNINITIALIZED)
		return;
	
	//SplashScreen splash;
	//splash.show(mainWindow);
	gameState = Game::MENU;
	initialize();
	settings.antialiasingLevel = 4;
	mainWindow.create(VideoMode(Game::SCREEN_SIZE_X,Game::SCREEN_SIZE_Y,32),"TreasureHunter", Style::Fullscreen ,settings);

	Button startButton(20, 708, 100, 40);
	init();
	
	while(!startClicked){
		while(mainWindow.pollEvent(event)){
			if(event.mouseMove.x != 0 && event.mouseMove.y != 0){
				lastMouseX = event.mouseMove.x;
				lastMouseY = event.mouseMove.y;
			}
			processEvent(event);
			if(startButton.bound.contains(lastMouseX, lastMouseY)){
				startButton.texture.setScale(0.9f, 0.9f);
				startButton.texture.setPosition(25, 710);
			}
			else if(startButton.texture.getScale().x != 1){
				startButton.texture.setScale(1, 1);
				startButton.texture.setPosition(20, 708);
			}
			
			if(event.type == Event::MouseButtonPressed && !drag){
				if(startButton.bound.contains(lastMouseX, lastMouseY)){
					startClicked = true;
				}
				for(int i=0;i<nVerteks;i++){
					if(verteks[i].bound.contains(lastMouseX, lastMouseY)){
						dragVerteks = i;
						hoverVerteks = i;
					}
				}
				drag = true;
			}
			if(event.type == Event::Closed)
				mainWindow.close();
			posUpdate();
			draw();
			mainWindow.draw(startButton.texture);
			if(showSelection)
				mainWindow.draw(selectionRect);
			mainWindow.display();
		}
	}

	solve();
	
	//formerly thread 1
	sf::Clock clock;
	while(!finishState){
		sf::Time elapsedTime = clock.restart();
		while(mainWindow.pollEvent(event)){
			if(slider.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)){
				hoverSlider = true;
			}
			if(hoverSlider)
				slider.setColor(Color::Yellow);
			else
				slider.setColor(Color::White);
			if(event.type == Event::MouseButtonPressed){
				dragSlider = true;
			}
			if(event.type == Event::MouseButtonReleased){
				dragSlider = false;
				hoverSlider = false;
			}
			if(dragSlider && hoverSlider){
				slider.setPosition(event.mouseMove.x - 10, slideBar[0].position.y);
			}
			if(slider.getPosition().x < slideBar[0].position.x)
				slider.setPosition(slideBar[0].position.x-10, slideBar[0].position.y);
			if(slider.getPosition().x > slideBar[1].position.x)
				slider.setPosition(slideBar[1].position.x-10, slideBar[0].position.y);
			delayTime = Game::MAX_SLEEP_TIME - (((slider.getPosition().x - slideBar[0].position.x) / (slideBar[1].position.x - slideBar[0].position.x)) * Game::MAX_SLEEP_TIME);
		}
		draw();
		update(elapsedTime.asMilliseconds());
		mainWindow.display();
		if (jalan > nVerteks - 1){
			finishState = true;
		}
		//sleep(milliseconds(delayTime));
	}

	while(true){
		while(mainWindow.pollEvent(event)){
			if(event.type == Event::MouseButtonPressed)
				mainWindow.close();
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
	solutionTools = new int[nVerteks];
	int randX, randY;
	bool avail;
	int difX, difY;
	for(int i=0;i<nVerteks;i++){
		Verteks v(nVerteks);
		do{
			avail = true;
			randX = (rand()%18)*50;
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

	if(deltaTime >= delayTime){
		
		deltaTime = 0;
		player.destVerteks = verteks[solution[jalan]].getNum() - 1;
		player.curVerteks = verteks[solution[jalan-1]].getNum() - 1;
		if(verteks[player.destVerteks].bound.intersects(player.bound)){
			player.set(verteks[player.destVerteks].bound.left, verteks[player.destVerteks].bound.top);
			if(verteks[player.destVerteks].getType() == verteks[player.destVerteks].CONTAIN_GREEN){
				sleep(milliseconds(250 * (delayTime/Game::MAX_SLEEP_TIME)));
				if(player.nTools1x > 0){
					player.useTool(Tool::TYPE_NORMAL);
					player.coins++;
				}
			}
			else if (verteks[player.destVerteks].getType() == verteks[player.destVerteks].CONTAIN_YELLOW){
				sleep(milliseconds(250 * (delayTime/Game::MAX_SLEEP_TIME)));
				if(player.nTools2x > 0){
					player.useTool(Tool::TYPE_2X);
					player.coins+=3;
				}
			}
			else if (verteks[player.destVerteks].getType() == verteks[player.destVerteks].CONTAIN_RED){
				sleep(milliseconds(250 * (delayTime/Game::MAX_SLEEP_TIME)));
				if(player.nTools4x > 0){
					player.useTool(Tool::TYPE_4X);
					player.coins +=5;
				}
			}
			jalan++;
			cout << player.coins << endl;
		}
		travelX = ((verteks[player.destVerteks].centerX - verteks[player.curVerteks].centerX) / verteks[player.curVerteks].lengths[player.destVerteks]) / 20;
		travelY = ((verteks[player.destVerteks].centerY - verteks[player.curVerteks].centerY) / verteks[player.curVerteks].lengths[player.destVerteks]) / 20;
		player.move(travelX, travelY);
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
		if(verteks[i].type == Verteks::CONTAIN_RED){
			solutionTools[nTools] = Tool::TYPE_4X;
			nTools++;
		}
		else if(verteks[i].type == Verteks::CONTAIN_YELLOW){
			solutionTools[nTools] = Tool::TYPE_2X;
			nTools++;
		}
		cout << verteks[solution[i]].getNum();
	}
	for(int i=0; i<nTools;i++){
		if(solutionTools[i] == Tool::TYPE_2X)
			player.buyTool(Tool::TYPE_2X);
		else if(solutionTools[i] == Tool::TYPE_4X)
			player.buyTool(Tool::TYPE_4X);
	}
	cout << endl;
}

void Game::init(){
	positions = new Vector2f[nVerteks];
	// get position 
	if(boxTexture.loadFromFile("Bitmap/Box.png")!=true){
		std::cout << "Image file Box.png failed to load" << std::endl;
		return;
	}
	if(characterTexture.loadFromFile("Bitmap/char.png")!=true){
		std::cout << "Image file char.png failed to load" << std::endl;
		return;
	}
	if(sideBarTexture.loadFromFile("Bitmap/Sidebar.png")!=true){
		std::cout << "Image file Sidebar.png failed to load" << std::endl;
		return;
	}
	if(sliderTexture.loadFromFile("Bitmap/Slider.png")!=true){
		std::cout << "Image file Slider.png failed to load" << std::endl;
		return;
	}
	if(tool1xTexture.loadFromFile("Bitmap/Tool1x.png")!=true){
		std::cout << "Image file Slider.png failed to load" << std::endl;
		return;
	}
	if(tool2xTexture.loadFromFile("Bitmap/Tool2x.png")!=true){
		std::cout << "Image file Slider.png failed to load" << std::endl;
		return;
	}
	if(tool4xTexture.loadFromFile("Bitmap/Tool4x.png")!=true){
		std::cout << "Image file Slider.png failed to load" << std::endl;
		return;
	}
	if(coinTexture.loadFromFile("Bitmap/koin.png")!=true){
		std::cout << "Image file koin.png failed to load" << std::endl;
		return;
	}
	coin = Sprite(coinTexture);
	coin.setScale(0.3f, 0.3f);
	tool1xp1 = Sprite(tool1xTexture);
	tool2xp1 = Sprite(tool2xTexture);
	tool4xp1 = Sprite(tool4xTexture);
	tool1xp2 = Sprite(tool1xTexture);
	tool2xp2 = Sprite(tool2xTexture);
	tool4xp2 = Sprite(tool4xTexture);
	tool1xp1.setScale(0.15f, 0.15f);
	tool2xp1.setScale(0.14f, 0.14f);
	tool4xp1.setScale(0.12f, 0.12f);
	tool1xp2.setScale(0.15f, 0.15f);
	tool2xp2.setScale(0.14f, 0.14f);
	tool4xp2.setScale(0.12f, 0.12f);
	tool1xp1.setPosition(1020, 100);
	tool2xp1.setPosition(1130, 100);
	tool4xp1.setPosition(1240, 100);
	tool1xp2.setPosition(1020, 430);
	tool2xp2.setPosition(1130, 430);
	tool4xp2.setPosition(1240, 430);
	slideBar = new Vertex[2];
	slideBar[0] = Vertex(Vector2f(1070, 700), Color::Black);
	slideBar[1] = Vertex(Vector2f(1270, 700), Color::Black);
	slider = Sprite(sliderTexture);
	slider.setScale(0.1f, 0.1f);
	slider.setPosition(slideBar[0].position.x,slideBar[0].position.y);
	box = Sprite(boxTexture);
	box.scale(0.05f, 0.05f);
	character = Sprite(characterTexture);
	character.scale(0.175f, 0.175f);
	sideBar = Sprite(sideBarTexture);
	sideBar.scale(0.86f, 0.676f);
	sideBar.setPosition(970, 0);
	circle = CircleShape(Verteks::VERTEKS_RADIUS);
	circle.setOutlineColor(Color::Black);
	circle.setOutlineThickness(5);
	selectionRect = RectangleShape(Vector2f(80,80));
	selectionRect.setFillColor(Color::Transparent);
	selectionRect.setOutlineThickness(5);
	selectionRect.setOutlineColor(Color::White);
	if(font.loadFromFile("comic.ttf")!=true){
		cout << "failed load font" <<endl;
	}
	text  = Text("",font);
	text.setColor(Color::Black);
	text.setCharacterSize(20);
}

void Game::draw(){
	mainWindow.clear(Color::White);
	mainWindow.draw(sideBar);
	// draw lines between verteks
	
	for(int i=0; i<nVerteks;i++){
		v = verteks[i];
		for(int j=0;j<nVerteks;j++){
			if(v.lengths[j] != 0 && v.lengths[j]!=-99){
				Vertex line[] = {
					Vertex(positions[i], Color::Black),
					Vertex(positions[j], Color::Black)
				};
				mainWindow.draw(line, 2, Lines);
			}
		}
	}
	// draw verteks
	for(int i=0;i<nVerteks;i++){
		v = verteks[i];
		circle.setPosition(v.getX(), v.getY());
		if(i == hoverVerteks)
			circle.setFillColor(Color::Cyan);
		else
			circle.setFillColor(Color::White);
		mainWindow.draw(circle);
		if(v.getType() == Verteks::CONTAIN_RED){
			box.setColor(Color::Red);
			box.setPosition(v.getX(), v.getY());
			mainWindow.draw(box);
		}
		else if(v.getType() == Verteks::CONTAIN_YELLOW){
			box.setColor(Color::Yellow);
			box.setPosition(v.getX(), v.getY());
			mainWindow.draw(box);
		}
		else if(v.getType() == Verteks::CONTAIN_GREEN){
			box.setColor(Color::Green);
			box.setPosition(v.getX(), v.getY());
			mainWindow.draw(box);
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
	character.setPosition(player.bound.left, player.bound.top);
	mainWindow.draw(character);
	mainWindow.draw(slideBar, 2, Lines);
	mainWindow.draw(slider);
	mainWindow.draw(tool1xp1);
	mainWindow.draw(tool2xp1);
	mainWindow.draw(tool4xp1);
	mainWindow.draw(tool1xp2);
	mainWindow.draw(tool2xp2);
	mainWindow.draw(tool4xp2);
	text.setString("Player 1");
	text.setPosition(1000, 20);
	text.setCharacterSize(30);
	mainWindow.draw(text);
	text.setString("Player 2");
	text.setPosition(1000, 350);
	mainWindow.draw(text);
	text.setCharacterSize(20);
	text.setString("Slow");
	text.setPosition(1050, 670);
	mainWindow.draw(text);
	text.setString("Fast");
	text.setPosition(1250, 670);
	mainWindow.draw(text);
	text.setColor(Color::Red);

	coin.setPosition(1000, 200);
	text.setString(std::to_string(player.coins));
	text.setPosition(coin.getPosition().x  + 80, coin.getPosition().y + 20);
	mainWindow.draw(text);
	mainWindow.draw(coin);
	coin.setPosition(1000, 530);
	text.setString(std::to_string(player.coins));
	text.setPosition(coin.getPosition().x  + 80, coin.getPosition().y + 20);
	mainWindow.draw(text);
	mainWindow.draw(coin);
	
	FloatRect rect;
	rect = tool1xp1.getGlobalBounds();
	text.setString(std::to_string(player.nTools1x));
	text.setPosition(rect.left, rect.top + rect.height - 20);
	mainWindow.draw(text);
	rect = tool2xp1.getGlobalBounds();
	text.setString(std::to_string(player.nTools2x));
	text.setPosition(rect.left, rect.top + rect.height - 20);
	mainWindow.draw(text);
	rect = tool4xp1.getGlobalBounds();
	text.setString(std::to_string(player.nTools4x));
	text.setPosition(rect.left, rect.top + rect.height - 20);
	mainWindow.draw(text);
	rect = tool1xp2.getGlobalBounds();
	text.setString(std::to_string(player.nTools1x));
	text.setPosition(rect.left, rect.top + rect.height - 20);
	mainWindow.draw(text);
	rect = tool2xp2.getGlobalBounds();
	text.setString(std::to_string(player.nTools2x));
	text.setPosition(rect.left, rect.top + rect.height - 20);
	mainWindow.draw(text);
	rect = tool4xp2.getGlobalBounds();
	text.setString(std::to_string(player.nTools4x));
	text.setPosition(rect.left, rect.top + rect.height - 20);
	mainWindow.draw(text);
	text.setColor(Color::Black);
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

void Game::processEvent(Event event){
	hoverVerteks = -1;
	for(int i=0;i<nVerteks;i++){
		if(verteks[i].bound.contains(lastMouseX, lastMouseY)){
			hoverVerteks = i;
		}
	}
	if(tool1xp1.getGlobalBounds().contains(lastMouseX, lastMouseY)){
		selectionRect.setPosition(1020, 90);
		showSelection = true;
		pointedTool = Game::P1_TOOL_1X;
	}
	else if(tool2xp1.getGlobalBounds().contains(lastMouseX, lastMouseY)){
		selectionRect.setPosition(1125, 90);
		showSelection = true;
		pointedTool = Game::P1_TOOL_2X;
	}
	else if(tool4xp1.getGlobalBounds().contains(lastMouseX, lastMouseY)){
		selectionRect.setPosition(1240, 90);
		showSelection = true;
		pointedTool = Game::P1_TOOL_4X;
	}
	else if(tool1xp2.getGlobalBounds().contains(lastMouseX, lastMouseY)){
		selectionRect.setPosition(1020, 420);
		showSelection = true;
		pointedTool = Game::P2_TOOL_1X;
	}
	else if(tool2xp2.getGlobalBounds().contains(lastMouseX, lastMouseY)){
		selectionRect.setPosition(1125, 420);
		showSelection = true;
		pointedTool = Game::P2_TOOL_2X;
	}
	else if(tool4xp2.getGlobalBounds().contains(lastMouseX, lastMouseY)){
		selectionRect.setPosition(1240, 420);
		showSelection = true;
		pointedTool = Game::P2_TOOL_4X;
	}
	else{
		pointedTool = -1;
		showSelection = false;
	}
	if(event.type == Event::MouseButtonPressed){
		switch(pointedTool){
		case Game::P1_TOOL_1X:
			player.buyTool(Tool(Tool::TYPE_NORMAL));
			break;
		case Game::P1_TOOL_2X:
			player.buyTool(Tool(Tool::TYPE_2X));
			break;
		case Game::P1_TOOL_4X:
			player.buyTool(Tool(Tool::TYPE_4X));
			break;
		case Game::P2_TOOL_1X:
			player.buyTool(Tool(Tool::TYPE_NORMAL));
			break;
		case Game::P2_TOOL_2X:
			player.buyTool(Tool(Tool::TYPE_2X));
			break;
		case Game::P2_TOOL_4X:
			player.buyTool(Tool(Tool::TYPE_4X));
			break;
		default: break;
		}
	}
	if(event.type == Event::MouseButtonReleased && drag){
		drag = false;
		dragVerteks = -1;
	}
	if(drag && dragVerteks!= -1){
		x = lastMouseX;
		y = lastMouseY;
		if(x > 0 && y > 0 && x < (float)sideBar.getPosition().x - Verteks::VERTEKS_RADIUS && y < (float)Game::SCREEN_SIZE_Y - 100)
			verteks[dragVerteks].set(x - Verteks::VERTEKS_RADIUS, y - Verteks::VERTEKS_RADIUS);
	}
}
Game::GameState Game::gameState = UNINITIALIZED;