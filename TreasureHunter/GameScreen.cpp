
#include "stdafx.h"
#include "GameScreen.h"

#include <vector>

using namespace sf;

void GameScreen::update(RenderWindow& renderWindow, World world){
	Texture texture;
	Texture character;
	sf::Vector2f *positions;
	sf::Font font;
	font.loadFromFile("comic.ttf");
	sf::Text text("",font);
	
	text.setCharacterSize(20);
	positions = new Vector2f[world.nVerteks];
	// Draw Grid
	for(int i=1; i<21; i++){
		Vertex line[] = {
			Vertex(Vector2f(50*i, 0)),
			Vertex(Vector2f(50*i, 600))
		};
		renderWindow.draw(line, 2, Lines);
	}
	for(int i=1; i<13; i++){
		Vertex line[] = {
			Vertex(Vector2f(0, 50*i)),
			Vertex(Vector2f(1024, 50*i))
		};
		renderWindow.draw(line, 2, Lines);
	}
	if(texture.loadFromFile("Bitmap/Box.png")!=true){
		std::cout << "Image file Box.png failed to load" << std::endl;
		return;
	}
	if(character.loadFromFile("Bitmap/char.png")!=true){
		std::cout << "Image file char.png failed to load" << std::endl;
		return;
	}
	Sprite sprite(texture);
	sprite.scale(0.05f, 0.05f);
	Sprite player(character);
	player.scale(0.175f, 0.175f);
	player.setPosition(world.player.x, world.player.y);
	CircleShape circle(GameScreen::CIRCLE_RADIUS);
	std::vector<Verteks> vertices;
	std::vector<Peti> peti;
	Verteks v;
	vertices = world.getVertices();
	peti = world.getPeti();
	// get position 
	for(int i=0;i<world.nVerteks;i++){
		v = vertices[i];
		Vector2f f2((float)(v.getX() + GameScreen::CIRCLE_RADIUS), (float)(v.getY() + GameScreen::CIRCLE_RADIUS));
		positions[i] = f2;
	}
	renderWindow.clear();
	// draw lines between vertices
	for(int i=0; i<world.nVerteks;i++){
		v = vertices[i];
		for(int j=0;j<world.nVerteks;j++){
			if(v.lengths[j] != 0 && v.lengths[j]!=-99){
				Vertex line[] = {
					Vertex(positions[i]),
					Vertex(positions[j])
				};
				renderWindow.draw(line, 2, Lines);
			}
		}
	}
	// draw vertices
	for(int i=0;i<world.nVerteks;i++){
		v = vertices[i];
		circle.setPosition(v.getX(), v.getY());
		renderWindow.draw(circle);
		if(v.getType() == Verteks::CONTAIN_RED){
			sprite.setColor(Color::Red);
			sprite.setPosition(v.getX(), v.getY());
			renderWindow.draw(sprite);
		}
		else if(v.getType() == Verteks::CONTAIN_YELLOW){
			sprite.setColor(Color::Yellow);
			sprite.setPosition(v.getX(), v.getY());
			renderWindow.draw(sprite);
		}
		else if(v.getType() == Verteks::CONTAIN_GREEN){
			sprite.setColor(Color::Green);
			sprite.setPosition(v.getX(), v.getY());
			renderWindow.draw(sprite);
		}
	}
	// draw text
	int length;
	Vector2f pos;
	for(int i=0; i<world.nVerteks-1;i++){
		for(int j=i+1;j<world.nVerteks;j++){
			length = vertices[i].lengths[j];
			if(length != 0 && length != -99){
				std::string str = std::to_string(length);
				text.setString(str);
				pos = midPoint(positions[i], positions[j]);
				text.setPosition(pos);
				renderWindow.draw(text);
			}
		}
	}
	renderWindow.draw(player);
	renderWindow.display();

	
	

	/*Event event;
	while(true){
		while(renderWindow.pollEvent(event)){
			if(event.type == Event::EventType::MouseButtonPressed){
				return;
			}
		}
	}*/
}

Vector2f GameScreen::midPoint(Vector2f v1, Vector2f v2){
	float x, y;
	x = (v1.x + v2.x)/2;
	y = (v1.y + v2.y)/2;
	Vector2f f(x,y);
	return f;
}