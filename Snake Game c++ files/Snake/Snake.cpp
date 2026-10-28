#include "Snake.h"

Snake::Snake(sf::Vector2i front) // snake starts facing up
{
	length = 3;
	bestLength = length;
	snakeSegments = new LinkList<SnakeSegment>;
	SnakeSegment segment = SnakeSegment(front, sf::Vector2i(0, -1));
	snakeSegments->addFront(segment);
	segment.position.y--;
	snakeSegments->addFront(segment);
	segment.position.y--;
	snakeSegments->addFront(segment);
	moveDelay = 0.4f;
}

Snake::~Snake()
{
	delete snakeSegments;
}

void Snake::BeginShrink()
{
	shrinkSegments = length / 3 - 1;
	Shrink();
}

void Snake::Shrink()
{
	for (int i = 0; i < 1; i++)
	{
		snakeSegments->removeEnd();
	}
	length -= 1;
	if (length < 2)
	{
		alive = 0;
	}
	if (length > 5)
	{
		moveDelay = 0.4f / (1 + ((float(length) - 5.f) / 40.f)); // Snake moves faster the longer its length is
	}
	if (length <= 5)
	{
		moveDelay = 0.4f; // snake moves at this speed at 5 length or less
	}
}

void Snake::Grow()
{
	snakeSegments->addEnd(growSegment);
	length++;
	if (length > bestLength)
	{
		bestLength = length;
	}
	if (length > 5)
	{
		moveDelay = 0.4f / (1 + ((float(length) - 5.f) / 40.f)); // Snake moves faster the longer its length is
	}
}


int const Snake::GetLength()
{
	return length;
}

int Snake::Move(sf::Vector2i direction, std::vector<Food>& foodVector) // returns value of food eaten
{
	if (shrinkSegments > 0)
	{
		Shrink();
		shrinkSegments--;
	}
	SnakeSegment frontSegment = snakeSegments->getFront()->data;
	if (frontSegment.direction + direction == sf::Vector2i(0, 0)) //prevent snake going back into itself
	{
		direction = frontSegment.direction;
	}
	sf::Vector2i newPosition = frontSegment.position + direction;

	// check for collision with food

	int value = 999;
	for (Food &food : foodVector)
	{
		if (newPosition == food.getPosition())
		{
			value = food.getValue();
			food.removeFood();
		}
	}

	// check for collision with self
	bool collision = checkCollision(newPosition,1,0,1);

	if (0 <= newPosition.x && newPosition.x < 10 && 0 <= newPosition.y && newPosition.y < 10 && !collision) //check for collision with wall
	{
		growSegment = snakeSegments->getEnd()->data;
		snakeSegments->removeEnd();
		SnakeSegment segment = SnakeSegment(newPosition, direction);
		snakeSegments->addFront(segment);
	}
	
	else //kill snake
	{
		alive = 0;
	}
	return value;
}

void const Snake::Draw(sf::RenderWindow& window, int gridSize)
{
	LinkListNode<SnakeSegment>* node = snakeSegments->getFront();
	bool colour = 0;
	while (node!=nullptr)
	{
		sf::RectangleShape rectangle{};
		rectangle.setPosition(sf::Vector2f(node->data.position * gridSize + sf::Vector2i(50,250)));
		rectangle.setSize(sf::Vector2f(gridSize, gridSize));

		if (node == snakeSegments->getFront()) // Draw head
		{
			const int offsetForwards = 12;
			const int offsetSideways = 8;
			rectangle.setFillColor(sf::Color(200, 30, 30));

			//left eye
			sf::CircleShape eyeL = sf::CircleShape(5);
			eyeL.setOrigin(sf::Vector2f(5, 5));

			sf::Vector2i offset = sf::Vector2i();
			offset.x = offsetForwards * node->data.direction.x + offsetSideways * node->data.direction.y;
			offset.y = offsetForwards * node->data.direction.y + offsetSideways * node->data.direction.x;

			eyeL.setPosition(sf::Vector2f(rectangle.getPosition().x + 25 + offset.x, rectangle.getPosition().y + 25 + offset.y));
			eyeL.setFillColor(sf::Color(0, 0, 0));

			//right eye
			sf::CircleShape eyeR = sf::CircleShape(5);
			eyeR.setOrigin(sf::Vector2f(5, 5));

			offset = sf::Vector2i();
			offset.x = offsetForwards * node->data.direction.x - offsetSideways * node->data.direction.y;
			offset.y = offsetForwards * node->data.direction.y - offsetSideways * node->data.direction.x;

			eyeR.setPosition(sf::Vector2f(rectangle.getPosition().x + 25 + offset.x, rectangle.getPosition().y + 25 + offset.y));
			eyeR.setFillColor(sf::Color(0, 0, 0));

			window.draw(rectangle);
			window.draw(eyeL);
			window.draw(eyeR);
		}

		else
		{
			sf::CircleShape scale(25);
			sf::Vector2f scalePos(node->data.position * gridSize + sf::Vector2i(50, 250));
			//Add offset to scale and scale it
			if (node->data.direction.x != 0)
			{
				scale.setScale(sf::Vector2f(0.5,1));
				scalePos.x = scalePos.x - 25 * node->data.direction.x + 12.5;
			}
			else
			{
				scale.setScale(sf::Vector2f(1, 0.5));
				scalePos.y = scalePos.y - 25 * node->data.direction.y + 12.5;
			}
			scale.setPosition(scalePos);

			if (node->next == nullptr && shrinkSegments > 0) // Make tail orange while shrinking
			{
				rectangle.setFillColor(sf::Color(204, 102, 0));
			}
			else if (colour) // Alternate snake colour between 2 shades of green
			{
				colour = 0;
				rectangle.setFillColor(sf::Color(0, 180, 0));
				scale.setFillColor(sf::Color(0, 255, 0));
			}
			else
			{
				colour = 1;
				rectangle.setFillColor(sf::Color(0, 255, 0));
				scale.setFillColor(sf::Color(0, 180, 0));
			}
			window.draw(rectangle);
			if (node->next != nullptr)
			{
				window.draw(scale);
			}
		}
		node = node->next;
	}
}

bool const Snake::checkCollision(sf::Vector2i position, bool ignoreHead, bool safeZone, bool skipTail)
{
	LinkListNode<SnakeSegment>* collisionSegment = snakeSegments->getFront();

	if (safeZone) //Checks all tiles 2 squares away from the head to ensure no unfair situations where an incorrect answer spawns too close to react
	{
		int directions[12][2] = { { 1, 1 },{ 1, 0 },{ 0, 1 },{ -1, -1 },{ -1, 0 },{ 0, -1 },{ -1, 1 },{ 1, -1 },{ 2, 0 },{ -2, 0 },{ 0, 2 },{ 0, -2 } };
		sf::Vector2i safeZonePosition;
		for (int* direction : directions)
		{
			safeZonePosition = collisionSegment->data.position;
			safeZonePosition.x += direction[0];
			safeZonePosition.y += direction[1];
			if (position == safeZonePosition)
			{
				return true;
			}
		}
	}

	if (ignoreHead) // skips over the head of the snake for when snake is checking if it collides with self
	{
		collisionSegment = collisionSegment->next;
	}

	if (skipTail && collisionSegment != nullptr)
	{
		while (collisionSegment->next != nullptr) //->next to skip tail
		{
			if (position == collisionSegment->data.position)
			{
				return true;
			}
			collisionSegment = collisionSegment->next;
		}
	}
	else
	{
		while (collisionSegment != nullptr) //->next to skip tail
		{
			if (position == collisionSegment->data.position)
			{
				return true;
			}
			collisionSegment = collisionSegment->next;
		}
	}
	
	return false;
}

int const Snake::getBestLength()
{
	return bestLength;
}

bool const Snake::getAlive()
{
	return alive;
}

float const Snake::GetMoveDelay()
{
	return moveDelay;
}

void Snake::SetMoveDelay(float const& newMoveDelay)
{
	moveDelay = newMoveDelay;
}
