#include "stdafx.h"
#include "Game.h"
#include "Button.h"
#include "SplashScreen.h"
#include "Game.h"
#include <thread>
#include <time.h>

using namespace std;



Event event;



vector<Verteks> verteks;
vector<Peti> peti;

sf::Text text, player1Text, player2Text, coin1Text, coin2Text, slowText, fastText;
sf::Text nTool11Text, nTool12Text, nTool14Text, nTool21Text, nTool22Text, nTool24Text;
std::string str;

Game::GameState Game::gameState;
Game::GreedyMode Game::greedyMode;
int Game::gameMode;
bool Game::isCreated;
RenderWindow Game::mainWindow;

Game::Game(){
	Game::isCreated = false;
	prepare();
	initialize();
}

void Game::start(){
	if(gameMode == Game::PLAYER_MODE)
		selectTime = true;
	else
		selectTime = false;
	init();
	
	//nunggu start button
	while(!startClicked){
		while(mainWindow.pollEvent(event)){
			if(event.mouseMove.x != 0 && event.mouseMove.y != 0){
				lastMouseX = (float)event.mouseMove.x;
				lastMouseY = (float)event.mouseMove.y;
			}
			processEvent(event);
			slide();
			if(playButton.getGlobalBounds().contains(lastMouseX, lastMouseY)){
				playButton.setScale(0.9f, 0.9f);
			}
			else if(playButton.getScale().x != 1){
				playButton.setScale(1, 1);
			}
			if(event.type == Event::MouseButtonPressed && !drag){
				if(playButton.getGlobalBounds().contains(lastMouseX, lastMouseY)){
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
			mainWindow.draw(playButton);
			mainWindow.display();
		}
	}
	switch(greedyMode){
	case GreedyMode::TYPE_1:
		solve1();
		break;
	case GreedyMode::TYPE_2:
		solve2();
		break;
	case GreedyMode::TYPE_3:
		solve3();
		break;
	default: break;
	}
	
	
	//formerly thread 1
	sf::Clock clock;
	sf::Time elapsedTime;
	while(!finishState && (maxTime-elapsedTimeUnit) > 0){
		elapsedTime = clock.restart();
		while(mainWindow.pollEvent(event)){
			if(event.mouseMove.x != 0 && event.mouseMove.y != 0){
				lastMouseX = (float)event.mouseMove.x;
				lastMouseY = (float)event.mouseMove.y;
			}
			slide();
			if(event.type == Event::Closed){
				mainWindow.close();
			}
		}
		update(elapsedTime.asMilliseconds());
		draw();
		mainWindow.display();
		if (jalan > nVerteks - 1){
			//finishState = true;
		}
		//sleep(milliseconds(delayTime));
	}
	while(true){
		while(mainWindow.pollEvent(event)){
			if(event.type == Event::MouseButtonPressed || event.type == Event::Closed)
				mainWindow.close();
		}
	}
}
	
void Game::initialize(){
	int in;
	int nMerah, nKuning, nHijau;
	// modal
	cin >> player1.coins;
	player2.coins = player1.coins;
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
	player1.curVerteks = in-1;  //posisi player -1 (verteks mulai dari 0)
	player2.curVerteks = in-1;
	cin >> minScore;
	cin >> maxTime;
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
		v.set((float)randX, (float)randY);
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
			verteks[p.loc].setUp(Verteks::CONTAIN_RED);
		if(p.type == Peti::YELLOW)
			verteks[p.loc].setUp(Verteks::CONTAIN_YELLOW);
		if(p.type == Peti::GREEN)
			verteks[p.loc].setUp(Verteks::CONTAIN_GREEN);
	}
	//
}

void Game::update(int elapsedTime){
	deltaTime += elapsedTime;
	float travelX = 0, travelY = 0;

	//std::cout << "delta" << elapsedTime << endl;
	if(deltaTime >= delayTime && !selectTime){
		
		deltaTime = 0;
		timeCounter++;
		if(timeCounter >= 20){
			elapsedTimeUnit++;
			timeCounter = 0;
		}
		//if(player1.colliBound.contains(verteks[player1.destVerteks].centerX, verteks[player1.destVerteks].centerY)){
		if(player1.timeToGo <=0){
			player1.setCenter(verteks[player1.destVerteks].centerX, verteks[player1.destVerteks].centerY);
			if(!player1.bukaPeti){
				if(verteks[player1.destVerteks].getType() == Verteks::CONTAIN_GREEN){
					if(player1.nTools1x > 0){
						player1.useTool(Tool::TYPE_NORMAL);
						player1.bukaPeti = true;
						player1.delayTime = 4;
						if(player1.delayTime <= verteks[player1.destVerteks].time)
							player1.coins++;
					}
				}
				else if (verteks[player1.destVerteks].getType() == Verteks::CONTAIN_YELLOW){
					if(player1.nTools2x > 0){
						player1.useTool(Tool::TYPE_2X);
						player1.bukaPeti = true;
						player1.delayTime = 2;
						if(player1.delayTime <= verteks[player1.destVerteks].time)
							player1.coins+=3;
					}
				}
				else if (verteks[player1.destVerteks].getType() == Verteks::CONTAIN_RED){
					if(player1.nTools4x > 0){
						player1.useTool(Tool::TYPE_4X);
						player1.bukaPeti = true;
						player1.delayTime = 1;
						if(player1.delayTime <= verteks[player1.destVerteks].time)
							player1.coins+=5;
					}
				}
			}
			if(player1.delayTime > 0){
				delayCounter++;
			}
			if(delayCounter >=20){
				player1.delayTime --;
				delayCounter = 0;
			}
			if(player1.delayTime <= 0 && jalan < solIdx-1){
				jalan++;
				player1.destVerteks = verteks[solution[jalan]].getNum() - 1;
				player1.curVerteks = verteks[solution[jalan-1]].getNum() - 1;
				player1.timeToGo = verteks[player1.curVerteks].lengths[player1.destVerteks];
			}
		}
		
		//player 1
		if(player1.timeToGoCounter >= 20){
			player1.timeToGo--;
			player1.timeToGoCounter = 0;
		}

		if(player1.delayTime<=0){
			if(player1.bukaPeti){
				player1.bukaPeti = false;
				if(jalan == solIdx -1)
					jalan++;
				verteks[solution[jalan-1]].type = Verteks::CONTAIN_NOTHING;
				requestDraw = true;
			}
			travelX = ((verteks[player1.destVerteks].centerX - verteks[player1.curVerteks].centerX) / verteks[player1.curVerteks].lengths[player1.destVerteks]) / 20;
			travelY = ((verteks[player1.destVerteks].centerY - verteks[player1.curVerteks].centerY) / verteks[player1.curVerteks].lengths[player1.destVerteks]) / 20;
			player1.move(travelX, travelY);
			player1.timeToGoCounter++;
		}

		//if(player2.colliBound.contains(verteks[player2.destVerteks].centerX, verteks[player2.destVerteks].centerY)){
		if(gameMode == Game::PLAYER_MODE){
			if(player2.timeToGo <=0){
				if(player2.delayTime <= 0)
					selectTime = true;
				player2.curVerteks = player2.destVerteks;
				player2.setCenter(verteks[player2.destVerteks].centerX, verteks[player2.destVerteks].centerY);
				if(player2.delayTime > 0){
					delayCounter++;
				}
				if(delayCounter >=20){
					player2.delayTime --;
					delayCounter = 0;
				}
			}
			if(player2.timeToGoCounter >= 20){
				player2.timeToGo--;
				player2.timeToGoCounter = 0;
			}
			if(player2.delayTime<=0){
				if(player2.bukaPeti){
					player2.bukaPeti = false;
					verteks[player2.curVerteks].type = Verteks::CONTAIN_NOTHING;
					requestDraw = true;
				}
				if(player2.curVerteks != player2.destVerteks){
					travelX = ((verteks[player2.destVerteks].centerX - verteks[player2.curVerteks].centerX) / verteks[player2.curVerteks].lengths[player2.destVerteks]) / 20;
					travelY = ((verteks[player2.destVerteks].centerY - verteks[player2.curVerteks].centerY) / verteks[player2.curVerteks].lengths[player2.destVerteks]) / 20;
					player2.move(travelX, travelY);
					player2.timeToGoCounter++;
				}
			}
		}
	}
	if(requestDraw){
		draw();
		mainWindow.display();
		requestDraw = false;
	}
	
	if(gameMode == Game::PLAYER_MODE){
		while(selectTime){
			while(mainWindow.pollEvent(event)){
				if(event.mouseMove.x != 0 && event.mouseMove.y != 0){
					lastMouseX = (float)event.mouseMove.x;
					lastMouseY = (float)event.mouseMove.y;
				}
				hoverVerteks = -1;
				for(int i=0;i<nVerteks;i++){
					if(verteks[i].bound.contains(lastMouseX, lastMouseY)){
						hoverVerteks = i;
					}
				}
				selectTool();
				if(event.type == Event::MouseButtonPressed){
					if(hoverVerteks >= 0 && hoverVerteks != player2.curVerteks && verteks[player2.curVerteks].lengths[hoverVerteks] != 0
						&& verteks[player2.curVerteks].lengths[hoverVerteks] != -99){
							player2.destVerteks = hoverVerteks;
							player2.timeToGo = verteks[player2.curVerteks].lengths[player2.destVerteks];
							selectTime = false;
					}
					switch(pointedTool){
					case Game::P2_TOOL_1X:
						if(player2.nTools1x > 0){
							player2.useTool(Tool::TYPE_NORMAL);
							player2.coins++;
							player2.delayTime = 4;
							player2.bukaPeti = true;
							selectTime = false;
						} break;
					case Game::P2_TOOL_2X:
						if(player2.nTools2x > 0){
							player2.useTool(Tool::TYPE_2X);
							player2.coins+=3;
							player2.delayTime = 2;
							player2.bukaPeti = true;
							selectTime = false;
						} break;
					case Game::P2_TOOL_4X:
						if(player2.nTools4x > 0){
							player2.useTool(Tool::TYPE_4X);
							player2.coins +=5;
							player2.delayTime = 1;
							player2.bukaPeti = true;
							selectTime = false;
						} break;
					default: break;
					}
				}
			}
			draw();
			mainWindow.display();
		}
	}
}

void Game::posUpdate(){
	for(int i=0;i<nVerteks;i++){
		v = verteks[i];
		Vector2f f2((float)(v.getX() + Verteks::VERTEKS_RADIUS), (float)(v.getY() + Verteks::VERTEKS_RADIUS));
		positions[i] = f2;
	}
	player1.set(verteks[player1.curVerteks].centerX-(Player::DEFAULT_WIDTH/2), verteks[player1.curVerteks].centerY-(Player::DEFAULT_HEIGHT/2));
	if(gameMode == Game::PLAYER_MODE)
		player2.set(verteks[player2.curVerteks].centerX-(Player::DEFAULT_WIDTH/2), verteks[player2.curVerteks].centerY-(Player::DEFAULT_HEIGHT/2));
}

void Game::solve1(){
	int j, point;
	int jaraktempuh = 0;
	int minLength, idxL, idxR, idx;
	float minRatio, ratio;
	Verteks curVerteks = verteks[player1.curVerteks];
	//System.out.println(player.curVerteks);
	idxL = player1.curVerteks;
	solution[solIdx] = idxL;
	cout<< solIdx<< "fidx" << endl;
	solIdx++;
	while(solIdx < nVerteks){
		j = 0;
		minLength = 9999;
		minRatio = 9999;
		idxL = -1;
		idxR = -1;
		while(j<nVerteks){
			if((j!=player1.curVerteks) && (curVerteks.lengths[j] < minLength) && (curVerteks.lengths[j]!=-99)){
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
				
			if(j!=player1.curVerteks && (point!=999) && (curVerteks.lengths[j]!=-99)){
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
		else if(idxL!= -1)
			idx = idxL;
		else
			break;
		player1.curVerteks = idx;
		curVerteks = verteks[idx];
		jaraktempuh += minLength;
		
		cout << solIdx << " "<< idx<<endl;
		solution[solIdx] = idx;
		solIdx++;
	}
	for(int i=0; i<solIdx; i++){
		if(verteks[solution[i]].type == Verteks::CONTAIN_RED){
			solutionTools[nTools] = Tool::TYPE_4X;
			nTools++;
		}
		else if(verteks[solution[i]].type == Verteks::CONTAIN_YELLOW){
			solutionTools[nTools] = Tool::TYPE_2X;
			nTools++;
		}
		cout << solution[i] +1;
	}
	for(int i=0; i<nTools;i++){
		if(solutionTools[i] == Tool::TYPE_2X)
			player1.buyTool(Tool::TYPE_2X);
		else if(solutionTools[i] == Tool::TYPE_4X)
			player1.buyTool(Tool::TYPE_4X);
	}
	cout << endl;
	player1.destVerteks = solution[1];
	player1.curVerteks = solution[0];
	player1.timeToGo = verteks[player1.curVerteks].lengths[player1.destVerteks];
}

void Game::solve2(){
	int in, i, j, k, point;
	int coins, ppg,playerPosition, minScore;
	int waktu = 0, jaraktempuh = 0,idxL, idx, alt;
	int MinRat, maxCoin, minLength;
	bool NotAllVisited, duplicate;
	char write;
	double rat, minRat;
	// modal
	coins = player1.coins;
	solIdx = 0; 
	// given
	
	playerPosition = player1.curVerteks;  //posisi player -1 (verteks mulai dari 0)
	ppg = playerPosition;
	idxL = playerPosition;
	int *SPC, *SPP, *LasJal, *simpul;
	double *SPR;
	bool *VisVer;
	SPC = new int[nVerteks];
	SPP = new int[nVerteks];
	LasJal = new int[nVerteks];
	simpul = new int[nVerteks];
	SPR = new double[nVerteks];
	VisVer = new bool[nVerteks];
	solution[solIdx] = idxL;
	solIdx++;
	int **jalanCoin, **jalanRatio, **jalanPath;
	double **lengthsRat;
	jalanCoin = new int*[nVerteks];
	jalanRatio = new int*[nVerteks];
	jalanPath = new int*[nVerteks];
	lengthsRat = new double*[nVerteks];
	for(int i=0; i<nVerteks; i++){
		switch(verteks[i].type){
		case Verteks::CONTAIN_RED:
			simpul[i] = 2;
			break;
		case Verteks::CONTAIN_YELLOW:
			simpul[i] = 1;
			break;
		case Verteks::CONTAIN_GREEN:
			simpul[i] = 0;
			break;
		default:
			simpul[i] = -1;
			break;
		}
	}
	for(int i=0;i<nVerteks;i++){
		lengthsRat[i] = new double[nVerteks];
		for(int j=0;j<nVerteks;j++){
			lengthsRat[i][j] = verteks[i].lengths[j];
		}
	}
	for(i = 0; i < nVerteks; i++){
		jalanCoin[i] = new int[nVerteks];
		for(j = 0; j < nVerteks;j++){
			if(j == 0){
				jalanCoin[i][j] = playerPosition + 1;
			}
			else {
				jalanCoin[i][j] = 0;
			}
		}					
	}
	for (i = 0; i < nVerteks; i++){
		SPC[i] = -999999999;
		VisVer[i] = false;
	}
	SPC[playerPosition] = 0;
	LasJal[playerPosition] = 0;
	NotAllVisited = true;
	while(NotAllVisited){
		maxCoin = -99999;
		idxL = -1;
		VisVer[playerPosition] = true;
		for(k = 0; k < nVerteks; k++){
			duplicate = false;
			if (k != playerPosition){
				rat = simpul[k];
				if(maxCoin < simpul[k] && !VisVer[k] && verteks[playerPosition].lengths[k] != -99){
					maxCoin = simpul[k];
					idxL = k;
				}
				if(SPC[k] < SPC[playerPosition] + simpul[k]){
					for(i = 0; i < LasJal[playerPosition] + 1; i++){
						if(i == LasJal[playerPosition]){
							
							for(j = 0; j < LasJal[playerPosition]; j++){
								if(k+1 == jalanCoin[k][j]){
									duplicate = true;
								}
							}
							if(!duplicate){
								jalanCoin[k][i+1] = k+1;
							}
						}
						jalanCoin[k][i] = jalanCoin[playerPosition][i];
					}
					if(!duplicate){
						SPC[k] = SPC[playerPosition] + simpul[k];
						LasJal[k] = LasJal[playerPosition] + 1;
					}
				}
			}
		}
		playerPosition = idxL;
		NotAllVisited = false;
		for (k = 0; k < nVerteks; k++){
			if(!VisVer[k]){
				NotAllVisited = true;
			}
		}
		if(idxL == -1){
			break;
		}
	}
	
	playerPosition = ppg;
	for(i = 0; i < nVerteks; i++){
		jalanPath[i] = new int[nVerteks];
		for(j = 0; j < nVerteks;j++){
			if(j == 0){
				jalanPath[i][j] = playerPosition + 1;
			}
			else {
				jalanPath[i][j] = 0;
			}
		}					
	}
	for(i = 0; i < nVerteks; i++){
		LasJal[i] = 0;
	}
	for (i = 0; i < nVerteks; i++){
		SPP[i] = 999999999;
		VisVer[i] = false;
	}
	SPP[playerPosition] = 0;
	LasJal[playerPosition] = 0;
	NotAllVisited = true;
	while(NotAllVisited){
		VisVer[playerPosition] = true;
		minLength = 99999;
		idxL = -1;
		for(k = 0; k < nVerteks; k++){
			if (k != playerPosition && verteks[playerPosition].lengths[k] != -99){
				if(minLength > verteks[playerPosition].lengths[k] && !VisVer[k]){
					minLength = verteks[playerPosition].lengths[k];
					idxL = k;
				}
				if(SPP[k] > SPP[playerPosition] + verteks[playerPosition].lengths[k]){
					SPP[k] = SPP[playerPosition] + verteks[playerPosition].lengths[k];
					for(i = 0; i < LasJal[playerPosition] + 1; i++){
						if(i == LasJal[playerPosition]){
							jalanPath[k][i+1] = k + 1;
						}
						jalanPath[k][i] = jalanPath[playerPosition][i];
					}
					LasJal[k] = LasJal[playerPosition] + 1;
				}
			}
		}
		if(idxL == -1){
			break;
		}
		playerPosition = idxL;
		NotAllVisited = false;
		for (k = 0; k < nVerteks; k++){
			if(!VisVer[k]){
				NotAllVisited = true;
			}
		}
		if(idxL == -1){
			break;
		}
	}

	playerPosition = ppg;
	for(i = 0; i < nVerteks; i++){
		jalanRatio[i] = new int[nVerteks];
		for(j = 0; j < nVerteks;j++){
			if(j == 0){
				jalanRatio[i][j] = playerPosition + 1;
			}
			else {
				jalanRatio[i][j] = 0;
			}
		}					
	}
	for (i = 0; i < nVerteks; i++){
		SPR[i] = -999999999;
		VisVer[i] = false;
	}
	SPR[playerPosition] = 0;
	for(i = 0; i < nVerteks; i++){
		LasJal[i] = 0;
	}
	
	NotAllVisited = true;
	while(NotAllVisited){
		minRat = 99999;
		idxL = -1;
		VisVer[playerPosition] = true;
		for(k = 0; k < nVerteks; k++){
			duplicate = false;
			if (simpul[k] == 0){
				rat = 0;
			}
			else 
				rat = (double)(lengthsRat[playerPosition][k] / simpul[k]);
			if (k != playerPosition  && lengthsRat[playerPosition][k] != -99){
				if(minRat > rat && !VisVer[k]){
					minRat = rat;
					idxL = k;
				}
				if(SPR[k] < SPR[playerPosition] + rat){
					for(i = 0; i < LasJal[playerPosition] + 1; i++){
						if(i == LasJal[playerPosition]){
							for(j = 0; j < LasJal[playerPosition]; j++){
								if(k+1 == jalanRatio[k][j]){
									duplicate = true;
								}
							}
							if(!duplicate){
								jalanRatio[k][i+1] = k+1;
							}
						}
						jalanRatio[k][i] = jalanRatio[playerPosition][i];
					}
					if(!duplicate){
						SPR[k] = SPR[playerPosition] + rat;
						LasJal[k] = LasJal[playerPosition] + 1;
					}
				}
			}
		}
		if (idxL == -1){
			break;
		}
		playerPosition = idxL;
		NotAllVisited = false;
		for (k = 0; k < nVerteks; k++){
			if(!VisVer[k]){
				NotAllVisited = true;
			}
		}
		if(idxL == -1){
			break;
		}
	}
	for(i = 0; i < nVerteks; i++){
		//cout << SPC[i] << endl;
		for(j = 0; j < nVerteks; j++){
			//cout << jalanCoin[i][j];
		}
		//cout << endl;
	}
	//cout << endl;
	for(i = 0; i < nVerteks; i++){
		cout << SPP[i] << endl;
		for(j = 0; j < nVerteks; j++){
			//cout << jalanPath[i][j];
		}
		//cout << endl;
	}
	//cout << endl;
	for(i = 0; i < nVerteks; i++){
		//cout << SPR[i] << endl;
		for(j = 0; j < nVerteks; j++){
			//cout << jalanRatio[i][j];
		}
		//cout << endl;
	}
	int mr, mp, mc, pc, pp, pr, choosenPath, passedPath, curCoin;
	mr = -9999999; mp = 9999999; mc = -9999999;
	for(i = 0; i < nVerteks; i++){
		if (mr < SPR[i]) {mr = SPR[i];pr = i;}
		if (mp > SPP[i] && SPP[i] != 0) {mp = SPP[i];pp = i;}
		if (mc < SPC[i]) {mc = SPC[i];pc = i;}
	}
	//cout << endl << pc << pp << pr << endl;
	curCoin = coins;
	passedPath = 0;
	choosenPath = pr;
	for(i = 0; i<nVerteks; i++){
		solution[i] = 0;
	}
	while(passedPath < nVerteks && curCoin > 0 && jalanCoin[choosenPath][passedPath] != 0){
		if(curCoin == 2){
			for(i = 0; i < nVerteks; i++){
				duplicate = false;
				for(j = 0; j < passedPath; j++){
					if (i == solution[j] - 1){
						duplicate == true;
					}
				}
				if(!duplicate && verteks[jalanCoin[choosenPath][passedPath]-1].lengths[i] != -99 && simpul[i] == 1){
					//cout << "masuk5" << i << endl;
					jalanCoin[choosenPath][passedPath] = i + 1;
				}
			}
		}
		else{
			if(jalanRatio[pr][passedPath] == jalanCoin[pc][passedPath]){
				duplicate = false;
				for(j = 0; j < passedPath; j++){
					if ( jalanCoin[pc][passedPath] == solution[j]){
						duplicate == true;
					}
				}
				if (!duplicate){
					solution[passedPath] = jalanCoin[pc][passedPath]-1;
				}
			}
			else{
				duplicate = false;
				for(j = 0; j < passedPath; j++){
					if (jalanCoin[pc][passedPath] == solution[j]){
						duplicate == true;
					}
				}
				if(!duplicate){
					solution[passedPath] = jalanCoin[pc][passedPath]-1;
				}
				duplicate = false;
				for(j = 0; j < passedPath; j++){
					if (jalanRatio[pc][passedPath] == solution[j]){
						duplicate == true;
					}
				}
				if(!duplicate){
					solution[passedPath] = jalanRatio[pc][passedPath]-1;
				}
			}
		}
		if(simpul[jalanCoin[choosenPath][passedPath]-1] == 2){
			curCoin -= 3;
		}
		else if(simpul[jalanCoin[choosenPath][passedPath]-1] == 1){
			curCoin -= 2;
		}
		passedPath ++;
		//cout << curCoin << endl;
	}
	//cout << endl;
	if (curCoin < 0){
		passedPath--;
	}
	solIdx = passedPath -1;
	cout<< "solution: " << endl;
	for(int i=0; i<solIdx; i++){
		if(verteks[solution[i]].type == Verteks::CONTAIN_RED){
			solutionTools[nTools] = Tool::TYPE_4X;
			nTools++;
		}
		else if(verteks[solution[i]].type == Verteks::CONTAIN_YELLOW){
			solutionTools[nTools] = Tool::TYPE_2X;
			nTools++;
		}
		cout << solution[i]+1 << endl;
	}
	for(int i=0; i<nTools;i++){
		if(solutionTools[i] == Tool::TYPE_2X)
			player1.buyTool(Tool::TYPE_2X);
		else if(solutionTools[i] == Tool::TYPE_4X)
			player1.buyTool(Tool::TYPE_4X);
	}
	cout << endl;
	player1.destVerteks = solution[1];
	player1.curVerteks = solution[0];
	player1.timeToGo = verteks[player1.curVerteks].lengths[player1.destVerteks];
}

void Game::solve3(){
	int in, i, j, point;
	int minScore;
	int waktu = 0, jaraktempuh = 0,idxL, idxR, idx;
	int MinRat, rat, minLength, playerPosition, coins;
	char write;
	// modal
	
	coins = player1.coins;
	playerPosition = player1.curVerteks;
	idxL = playerPosition;
	solution[solIdx] = idxL;
	solIdx++;

	i = 1;
	while (i < nVerteks){
		j = 0;
		minLength = 9999;
		MinRat = 9999;
		idxL = -1;
		idxR = -1;
		while(j<nVerteks){
			if((j!=playerPosition) && (verteks[playerPosition].lengths[j] < minLength) && (verteks[playerPosition].lengths[j]!=-99)){
				bool duplicate = false;
				for(int k=0; k<solIdx; k++){
					if(j == solution[k]){
						duplicate = true;
					}
				}
				if(!duplicate){
					minLength = verteks[playerPosition].lengths[j];
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

			if(j!=playerPosition && (point!=999) && (verteks[playerPosition].lengths[j]!=-99)){
				rat = (float)verteks[playerPosition].lengths[j]/(float)point;
				if(rat < MinRat){
					bool duplicate = false;
					for(int k=0; k<solIdx; k++){
						if(j == solution[k]){
							duplicate = true;
						}
					}
					if(!duplicate){
						MinRat = rat;
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
		if (verteks[playerPosition].lengths[idx] < 9999999){
			waktu += verteks[playerPosition].lengths[idx];
		}
		if (waktu > maxTime){
			break;
		}
		playerPosition = idx;
		solution[solIdx] = idx;
		switch(verteks[idx].type){
		case Verteks::CONTAIN_RED:
				if (coins >= 3){
					waktu += 1;
					coins += 2;
					write = 'M';
				}
				break;
		case Verteks::CONTAIN_YELLOW:
				if (coins >= 2){
					waktu += 2;
					coins += 1;
					write = 'K';
				}
				break;
			default:
				break;
		}
		solIdx++;
		i++;
		cout << idx + 1 << "," << write << endl;
	}
	cout << endl << waktu << " " << coins << endl;
	for(int i=0; i<solIdx; i++){
		if(verteks[solution[i]].type == Verteks::CONTAIN_RED){
			solutionTools[nTools] = Tool::TYPE_4X;
			nTools++;
		}
		else if(verteks[solution[i]].type == Verteks::CONTAIN_YELLOW){
			solutionTools[nTools] = Tool::TYPE_2X;
			nTools++;
		}
		
	}
	for(int i=0; i<nTools;i++){
		if(solutionTools[i] == Tool::TYPE_2X)
			player1.buyTool(Tool::TYPE_2X);
		else if(solutionTools[i] == Tool::TYPE_4X)
			player1.buyTool(Tool::TYPE_4X);
	}
	player1.destVerteks = solution[1];
	player1.curVerteks = solution[0];
	player1.timeToGo = verteks[player1.curVerteks].lengths[player1.destVerteks];
}

void Game::init(){
	positions = new Vector2f[nVerteks];
	boxTexture.loadFromFile("Bitmap/Box.png");
	characterTexture.loadFromFile("Bitmap/char.png");
	sideBarTexture.loadFromFile("Bitmap/Sidebar.png");
	sliderTexture.loadFromFile("Bitmap/Slider.png");
	tool1xTexture.loadFromFile("Bitmap/Tool1x.png");
	tool2xTexture.loadFromFile("Bitmap/Tool2x.png");
	tool4xTexture.loadFromFile("Bitmap/Tool4x.png");
	coinTexture.loadFromFile("Bitmap/koin.png");
	playBGTexture.loadFromFile("Bitmap/PlayBG.png");
	playButtonTexture.loadFromFile("Bitmap/startButton.png");
	playBG.setPosition(0,0);
	playBG.setSize(sf::Vector2f(1366,768));
	playBG.setTexture(&playBGTexture);
	playButton.setPosition(80, 700);
	playButton.setSize(sf::Vector2f(100,110));
	playButton.setOrigin(50,75);
	playButton.setTexture(&playButtonTexture);
	coin = Sprite(coinTexture);
	coin2 = Sprite(coinTexture);
	coin.setScale(0.3f, 0.3f);
	coin2.setScale(0.3f,0.3f);
	coin.setPosition(1000, 200);
	coin2.setPosition(1000, 530);
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
	character1.setSize(sf::Vector2f(60,70));
	character1.setTexture(&characterTexture);
	character2.setSize(sf::Vector2f(60,70));
	character2.setTexture(&characterTexture);
	sideBar = Sprite(sideBarTexture);
	sideBar.scale(0.86f, 0.676f);
	sideBar.setPosition(970, 0);
	circle = CircleShape((float)Verteks::VERTEKS_RADIUS);
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
	player1Text = Text("Player 1 : CPU", font);
	player1Text.setColor(Color::Black);
	player1Text.setCharacterSize(30);
	player1Text.setPosition(1000, 20);
	player2Text = Text("Player 2 : Human", font);
	player2Text.setColor(Color::Black);
	player2Text.setCharacterSize(30);
	player2Text.setPosition(1000, 350);
	coin1Text = Text("0", font);
	coin1Text.setColor(Color::Red);
	coin1Text.setCharacterSize(20);
	coin1Text.setPosition(coin.getPosition().x  + 80, coin.getPosition().y + 20);
	coin2Text = Text("0", font);
	coin2Text.setColor(Color::Red);
	coin2Text.setCharacterSize(20);
	coin2Text.setPosition(coin2.getPosition().x  + 80, coin2.getPosition().y + 20);
	nTool11Text = Text("0", font);
	nTool11Text.setColor(Color::Red);
	nTool11Text.setCharacterSize(20);
	nTool12Text = Text("0", font);
	nTool12Text.setColor(Color::Red);
	nTool12Text.setCharacterSize(20);
	nTool14Text = Text("0", font);
	nTool14Text.setColor(Color::Red);
	nTool14Text.setCharacterSize(20);
	nTool21Text = Text("0", font);
	nTool21Text.setColor(Color::Red);
	nTool21Text.setCharacterSize(20);
	nTool22Text = Text("0", font);
	nTool22Text.setColor(Color::Red);
	nTool22Text.setCharacterSize(20);
	nTool24Text = Text("0", font);
	nTool24Text.setColor(Color::Red);
	nTool24Text.setCharacterSize(20);
	slowText = Text("Slow", font);
	slowText.setColor(Color::Black);
	slowText.setCharacterSize(20);
	slowText.setPosition(1050, 670);
	fastText = Text("Fast", font);
	fastText.setColor(Color::Black);
	fastText.setCharacterSize(20);
	fastText.setPosition(1250, 670);
	FloatRect rect;
	rect = tool1xp1.getGlobalBounds();
	nTool11Text.setPosition(rect.left, rect.top + rect.height - 20);
	rect = tool2xp1.getGlobalBounds();
	nTool12Text.setPosition(rect.left, rect.top + rect.height - 20);
	rect = tool4xp1.getGlobalBounds();
	nTool14Text.setPosition(rect.left, rect.top + rect.height - 20);
	rect = tool1xp2.getGlobalBounds();
	nTool21Text.setPosition(rect.left, rect.top + rect.height - 20);
	rect = tool2xp2.getGlobalBounds();
	nTool22Text.setPosition(rect.left, rect.top + rect.height - 20);
	rect = tool4xp2.getGlobalBounds();
	nTool24Text.setPosition(rect.left, rect.top + rect.height - 20);
}

void Game::draw(){
	mainWindow.clear(Color::White);
	mainWindow.draw(playBG);
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
			circle.setFillColor(Color::Transparent);
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
	mainWindow.draw(player1Text);
	character1.setPosition(player1.bound.left, player1.bound.top);
	mainWindow.draw(character1);
	mainWindow.draw(tool1xp1);
	mainWindow.draw(tool2xp1);
	mainWindow.draw(tool4xp1);
	coin1Text.setString(std::to_string(player1.coins));
	mainWindow.draw(coin1Text);
	mainWindow.draw(coin);
	nTool11Text.setString(std::to_string(player1.nTools1x));
	nTool12Text.setString(std::to_string(player1.nTools2x));
	nTool14Text.setString(std::to_string(player1.nTools4x));
	mainWindow.draw(nTool11Text);
	mainWindow.draw(nTool12Text);
	mainWindow.draw(nTool14Text);
	mainWindow.draw(slideBar, 2, Lines);
	mainWindow.draw(slider);
	if(gameMode == Game::PLAYER_MODE){
		mainWindow.draw(player2Text);
		character2.setPosition(player2.bound.left, player2.bound.top);
		mainWindow.draw(character2);	
		mainWindow.draw(tool1xp2);
		mainWindow.draw(tool2xp2);
		mainWindow.draw(tool4xp2);
		coin2Text.setString(std::to_string(player2.coins));
		mainWindow.draw(coin2Text);
		mainWindow.draw(coin2);
		nTool21Text.setString(std::to_string(player2.nTools1x));
		nTool22Text.setString(std::to_string(player2.nTools2x));
		nTool24Text.setString(std::to_string(player2.nTools4x));
		mainWindow.draw(nTool21Text);
		mainWindow.draw(nTool22Text);
		mainWindow.draw(nTool24Text);
	}

	mainWindow.draw(slowText);
	mainWindow.draw(fastText);
	text.setColor(Color::White);
	text.setString("time : " + std::to_string(maxTime - elapsedTimeUnit));
	text.setPosition(850, 20);
	mainWindow.draw(text);
	text.setColor(Color::Black);
	if(showSelection)
		mainWindow.draw(selectionRect);
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
	/*if(tool1xp1.getGlobalBounds().contains(lastMouseX, lastMouseY)){
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
	}*/
	selectTool();
	
	if(event.type == Event::MouseButtonPressed){
		switch(pointedTool){
		/*case Game::P1_TOOL_1X:
			//player1.buyTool(Tool(Tool::TYPE_NORMAL));
			break;
		case Game::P1_TOOL_2X:
			//player1.buyTool(Tool(Tool::TYPE_2X));
			break;
		case Game::P1_TOOL_4X:
			//player1.buyTool(Tool(Tool::TYPE_4X));
			break;*/
		case Game::P2_TOOL_1X:
			player2.buyTool(Tool(Tool::TYPE_NORMAL));
			break;
		case Game::P2_TOOL_2X:
			player2.buyTool(Tool(Tool::TYPE_2X));
			break;
		case Game::P2_TOOL_4X:
			player2.buyTool(Tool(Tool::TYPE_4X));
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

void Game::selectTool(){
	if(gameMode == Game::PLAYER_MODE){
		if(tool1xp2.getGlobalBounds().contains(lastMouseX, lastMouseY)){
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
	}
	
}

void Game::slide(){
	if(slider.getGlobalBounds().contains(lastMouseX, lastMouseY)){
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
		slider.setPosition(lastMouseX - 10, (float)slideBar[0].position.y);
	}
	if(slider.getPosition().x < slideBar[0].position.x)
		slider.setPosition(slideBar[0].position.x-10, slideBar[0].position.y);
	if(slider.getPosition().x > slideBar[1].position.x)
		slider.setPosition(slideBar[1].position.x-10, slideBar[0].position.y);
	delayTime = Game::MAX_SLEEP_TIME - (((slider.getPosition().x - slideBar[0].position.x) / (slideBar[1].position.x - slideBar[0].position.x)) * Game::MAX_SLEEP_TIME);
}

void Game::prepare(){
	finishState = false;
	startClicked = false;
	drag = false;
	dragSlider = false;
	hoverSlider = false;
	toolTime = false;
	requestDraw = false;
	showSelection = false;
	toolClicked;
	dragVerteks = -1;
	hoverVerteks = -1;
	pointedTool = -1;
	lastMouseX = -1;
	lastMouseY = -1;
	timeCounter = 0;
	delayCounter = 0;
	elapsedTimeUnit = 0;
	delayTime = 20;
	solIdx = 0;
	srand(time(NULL));
	deltaTime = 0;
	nTools = 0;
	verteks.clear();
	peti.clear();
	//gameMode = Game::PLAYER_MODE;
	
}