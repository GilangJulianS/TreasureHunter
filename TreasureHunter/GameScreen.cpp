
#include "stdafx.h"
#include "GameScreen.h"

#include <vector>

using namespace sf;

Sprite sprite;
Sprite player;
Texture texture;
Texture character;
CircleShape circle;
sf::Font font;
sf::Text text;
std::string str;

void GameScreen::init(World world){
	vertices = world.getVertices();
	peti = world.getPeti();
	positions = new Vector2f[world.nVerteks];
	// get position 
	for(int i=0;i<world.nVerteks;i++){
		v = vertices[i];
		Vector2f f2((float)(v.getX() + Verteks::VERTEKS_RADIUS), (float)(v.getY() + Verteks::VERTEKS_RADIUS));
		positions[i] = f2;
	}
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
	player = Sprite(character);
	player.scale(0.175f, 0.175f);
	circle = CircleShape(Verteks::VERTEKS_RADIUS);
	font.loadFromFile("comic.ttf");
	text  = Text("",font);
	text.setCharacterSize(20);
}


void GameScreen::update(RenderWindow& renderWindow, World world){
	// Draw Grid
	/*for(int i=1; i<21; i++){
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
	}*/
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
	for(int i=0; i<world.nVerteks-1;i++){
		for(int j=i+1;j<world.nVerteks;j++){
			length = vertices[i].lengths[j];
			if(length != 0 && length != -99){
				str = std::to_string(length);
				text.setString(str);
				text.setPosition(midPoint(positions[i], positions[j]));
				renderWindow.draw(text);
			}
		}
	}
	player.setPosition(world.player.bound.left, world.player.bound.top);
	renderWindow.draw(player);
	renderWindow.display();

}

Vector2f GameScreen::midPoint(Vector2f v1, Vector2f v2){
	float x, y;
	x = (v1.x + v2.x)/2;
	y = (v1.y + v2.y)/2;
	Vector2f f(x,y);
	return f;
}