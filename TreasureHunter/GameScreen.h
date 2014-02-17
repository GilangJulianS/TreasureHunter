#include "World.h"
class GameScreen{
public:
	void update(sf::RenderWindow& renderWindow, World world);
private:
	sf::Vector2f midPoint(sf::Vector2f v1, sf::Vector2f v2);
};