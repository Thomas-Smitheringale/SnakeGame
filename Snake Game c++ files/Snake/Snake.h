#pragma once
#include "LinkList.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Food.h"
class Snake
{
private:
	struct SnakeSegment
	{
		sf::Vector2i position;
		sf::Vector2i direction;

		SnakeSegment()
		{

		}

		SnakeSegment(sf::Vector2i position, sf::Vector2i direction)
		{
			this->position = position;
			this->direction = direction;
		}
	};
	int length;
	int bestLength;
	bool alive = 1;
	LinkList<SnakeSegment>* snakeSegments;
	SnakeSegment growSegment; //Segment is kept for a move so that if the snake grows it will grow to where it last was
	float moveDelay;
	int shrinkSegments = 0;
public:
	Snake(sf::Vector2i front);
	~Snake();
	void BeginShrink(); // Makes snake shrink by number of segments proportional to length, one segment per move
	void Shrink(); //Remove one segment from snake, return true if snake dead
	void Grow(); //Grow snake by one segment
	int const GetLength();
	int Move(sf::Vector2i direction,std::vector<Food>& foodVector); //Returns value of food eaten (999 if no food eaten)
	void const Draw(sf::RenderWindow& window,int gridSize);
	bool const checkCollision(sf::Vector2i position,bool ignoreHead = 0, bool safeZone = 0, bool skipTail = 0); //Return True if collision detected
																					  //if true, safeZone checks area around front of snake to avoid stuff spawning too close to snake
	int const getBestLength();
	bool const getAlive();
	float const GetMoveDelay();
	void SetMoveDelay(float const &newMoveDelay);
};

