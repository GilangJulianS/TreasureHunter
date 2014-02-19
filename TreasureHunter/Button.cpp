#include "stdafx.h"
#include "Button.h"

Button::Button(float x, float y, float width, float height){
	bound = sf::FloatRect(x, y, width, height);
	texture = sf::RectangleShape(sf::Vector2f(width, height));
	texture.setPosition(sf::Vector2f(x, y));
	texture.setFillColor(sf::Color::Red);
}