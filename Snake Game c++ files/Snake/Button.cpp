#include "Button.h"
//// Draw button
//sf::Vector2f buttonPos(window.getSize().x / 2 - 150, window.getSize().y / 2 - 100);
//sf::RectangleShape button = sf::RectangleShape(sf::Vector2f(300, 100));
//button.setFillColor(sf::Color(0, 100, 0));
//button.setPosition(buttonPos);
//window.draw(button);
//
//// Button Text
//sf::Text text(ResourceHolder::font, "Play", 50);
//text.setPosition(sf::Vector2f(window.getSize().x / 2 - 60, window.getSize().y / 2 - 75));
//text.setFillColor(sf::Color(0, 0, 0));
//window.draw(text);
//if (click && mousePos.x > buttonPos.x && mousePos.x < buttonPos.x + 300 && mousePos.y > buttonPos.y && mousePos.y < buttonPos.y + 100)
//{
//    gameManager->startGame(gameArea);
//}

Button::Button()
{
}

Button::Button(sf::Vector2f buttonPosition, sf::Vector2f buttonSize, std::string buttonText, sf::Color color)
{
	position = buttonPosition;
	size = buttonSize;
	text = buttonText;
	colour = color;
}

bool Button::ButtonClicked(sf::Vector2i mousePos)
{
	return (mousePos.x > position.x && mousePos.x < position.x + size.x && mousePos.y > position.y && mousePos.y < position.y + size.y);
}

void Button::Draw(sf::RenderWindow& window)
{
	// Draw button rectangle
	sf::RectangleShape button = sf::RectangleShape(sf::Vector2f(size.x, size.y));
	button.setFillColor(sf::Color(0, 100, 0));
	button.setPosition(sf::Vector2f(position.x, position.y));
	button.setFillColor(colour);
	button.setOutlineColor((sf::Color(0, 0, 0)));
	button.setOutlineThickness(3);
	window.draw(button);

	// Button Text
	sf::Text drawText(ResourceHolder::font, text, textSize);
	drawText.setFillColor(sf::Color(0, 0, 0));
	//Centre Text
	sf::FloatRect rc = drawText.getLocalBounds();
	drawText.setOrigin(sf::Vector2f(rc.size.x / 2, rc.size.y / 2));

	drawText.setPosition(sf::Vector2f(position.x + size.x / 2, position.y + size.y / 2));
	window.draw(drawText);
}

void Button::setText(std::string newText)
{
	text = newText;
}

void Button::setTextSize(int newSize)
{
	textSize = newSize;
}
