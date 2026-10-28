#pragma once
#include "LinkList.h"
#include "ObjectPool.h"
#include "Food.h"
#include "Snake.h"
#include "ResourceHolder.h"
class GameManager
{
public:
	enum class GameState
	{
		dead,
		makingQuestion,
		answeringQuestion,
		mainMenu
	};
	enum class GameMode
	{
		timesTables,
		addition,
		timesTablesTimer,
		mixed
	};
private:
	static GameManager* instance;
	GameManager();
	
	GameState gameState;
	GameMode gameMode;
	int operands[2];
	int gameArea[2] = { 14,10 };
	float timer; //timer for timesTablesTimer mode
	ObjectPool<Food>* foodPool;
	Snake* snake;
	float moveTimer = -3.f;
	int answer;
	bool timesTables; //True if question is multiplication, false if it is addition
public:
	static GameManager* GetInstance();
	void EatFood(int number);
	ObjectPool<Food>* const getFoodPool();
	void const Draw(sf::RenderWindow& window,int gridSize);
	sf::Vector2i const findEmpty(bool avoidSnake = 1);
	void startGame(int const newGameArea[2]);
	void update(sf::Vector2i direction,float const &deltaTime);
	void endGame();
	GameState getGameState();
	int getScore();
	void MainMenu();
	void setGameMode(GameMode newMode);
	GameMode getGameMode();
};

