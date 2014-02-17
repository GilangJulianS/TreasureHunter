#include "World.h"
class GameScreen{
public:
	static const int CIRCLE_RADIUS = 25;
	void update(sf::RenderWindow& renderWindow, World world);
private:
	sf::Vector2f midPoint(sf::Vector2f v1, sf::Vector2f v2);
};