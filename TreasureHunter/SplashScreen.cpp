
#include "stdafx.h"
#include "SplashScreen.h"

void SplashScreen::show(sf::RenderWindow& renderWindow){
	sf::Texture texture;
	if(texture.loadFromFile("Bitmap/Box.png")!=true){
		return;
	}
	
	sf::Sprite sprite(texture);
	sprite.scale(0.08f,0.08f);
	renderWindow.draw(sprite);
	renderWindow.display();

	sf::Event event;
	while(true){
		while(renderWindow.pollEvent(event)){
			if(event.type == sf::Event::EventType::MouseButtonPressed){
				return;
			}
		}
	}
}