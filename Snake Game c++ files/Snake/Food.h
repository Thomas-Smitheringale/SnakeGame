#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceHolder.h"
class Food
{
private:
	int value;
	sf::Vector2i position;
public:
	Food();
	~Food();
	void PlaceFood(sf::Vector2i position,int value);
	void removeFood();
	int const getValue();
	sf::Vector2i const getPosition();
	void const Draw(sf::RenderWindow& window, int gridSize);
};

