#include "Food.h"

Food::Food()
{
    value = 0;
    position = sf::Vector2i(0, 0);
}

Food::~Food()
{
}

void Food::PlaceFood(sf::Vector2i position, int value)
{
    this->position = position;
    this->value = value;
}

void Food::removeFood()
{
    value = 0;
    position.x = -1;
    position.y = -1;
}

int const Food::getValue()
{
    return value;
}

sf::Vector2i const Food::getPosition()
{
    return position;
}

void const Food::Draw(sf::RenderWindow& window, int gridSize)
{
    if(value != 0)
    {
        sf::CircleShape circle = sf::CircleShape(gridSize / 2 - 2);
        sf::Vector2f drawPos = sf::Vector2f(position);
        drawPos.x = (drawPos.x + 1) * gridSize + 2;
        drawPos.y = (drawPos.y + 5) * gridSize + 2;
        circle.setPosition(drawPos);
        circle.setFillColor(sf::Color(255, 0, 0));
        circle.setOutlineColor((sf::Color(0, 0, 0)));
        circle.setOutlineThickness(3);
        window.draw(circle);
        sf::Text text = sf::Text(ResourceHolder::font,std::to_string(value), 25);
        //Centre Text
        sf::FloatRect rc = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(rc.size.x / 2, rc.size.y / 2));

        text.setPosition(drawPos + sf::Vector2f(20,10));
        window.draw(text);
    }
}