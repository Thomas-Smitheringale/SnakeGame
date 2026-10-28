#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceHolder.h"
class Button
{
private:
	sf::Vector2f position;
	sf::Vector2f size;
	std::string text;
	int textSize = 50;
	sf::Color colour;
public:
	Button();
	Button(sf::Vector2f buttonPosition, sf::Vector2f buttonSize, std::string buttonText,sf::Color color);
	bool ButtonClicked(sf::Vector2i mousePos);
	void Draw(sf::RenderWindow& window);
	void setText(std::string newText);
	void setTextSize(int newSize);
};

