#include "stdafx.h"

class Button{

public:
	Button(float x, float y, float width, float height);
	sf::FloatRect bound;
	sf::RectangleShape texture;
};