#include "World.h"
class GameScreen{
public:
	void init(World world);
	void update(sf::RenderWindow& renderWindow, World world);
	sf::Vector2f *positions;
	
private:
	sf::Vector2f midPoint(sf::Vector2f v1, sf::Vector2f v2);
	
	Verteks v;
	std::vector<Verteks> vertices;
	std::vector<Peti> peti;
	
};