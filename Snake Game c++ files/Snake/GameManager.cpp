#include "GameManager.h"
GameManager::GameManager()
{
    gameState = GameState::mainMenu;
    foodPool = new ObjectPool<Food>();
    for (unsigned int i = 0; i < 5; i++)
    {
        foodPool->createObject();
    }

    snake = nullptr;
    gameMode = GameMode::timesTables;
    answer = 0;
}

GameManager* GameManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new GameManager();
    }
    return instance;
}

void GameManager::EatFood(int number)
{
    if (gameState == GameState::makingQuestion) //making question - add number to question
    {
        if (operands[0] == 0)
        {
            operands[0] = number;
        }
        else
        {
            operands[1] = number;
            gameState = GameState::answeringQuestion;
            int foodCount;

            // More incorrect answers for longer snakes, up to 4 incorrect answers.
            if (snake->GetLength() < 10)
            {
                foodCount = 2;
            }
            else if (snake->GetLength() < 20)
            {
                foodCount = 3;
            }
            else
            {
                foodCount = 4;
            }

            int foodCounter = 0;
            bool answerPlaced = false;
            int numbersUsed[5]{};
            for (Food& food : foodPool->getObjects())
            {
                food.removeFood();
                if (!answerPlaced)
                {
                    answerPlaced = true;
                    if (timesTables)
                    {
                        food.PlaceFood(sf::Vector2i(findEmpty()), operands[0] * operands[1]);
                        numbersUsed[0] = operands[0] * operands[1];
                        answer = operands[0] * operands[1];
                    }
                    else
                    {
                        food.PlaceFood(sf::Vector2i(findEmpty()), operands[0] + operands[1]);
                        numbersUsed[0] = operands[0] + operands[1];
                        answer = operands[0] + operands[1];
                    }
                }
                else if (foodCounter < foodCount)
                {
                    //Generate a random number not already in the array
                    bool numberInArray = true;
                    int randomNumber;
                    while (numberInArray)
                    {
                        numberInArray = false;
                        if (timesTables)
                        {
                            randomNumber = operands[rand() % 2] * (rand() % 12 + 1); //Correct answer made with a random operand to make it less obviously wrong
                        }
                        else
                        {
                            if (operands[0] > operands[1]) //This ensures that the answers are always bigger than both operands as otherwise they would be very obviously wrong
                            {
                                randomNumber = (rand() % 12 + 1) + operands[0];
                            }
                            else
                            {
                                randomNumber = (rand() % 12 + 1) + operands[1];
                            }
                        }
                        for(int number : numbersUsed)
                        {
                            if (number == randomNumber)
                            {
                                numberInArray = true;
                            }
                        }
                    }

                    foodCounter++;
                    food.PlaceFood(findEmpty(), randomNumber);
                    numbersUsed[foodCounter] = randomNumber;
                }
            }
        }
    }
    else if (number == answer) //correct answer
    {
        operands[0] = 0;
        operands[1] = 0;
        if (gameMode == GameMode::mixed)
        {
            timesTables = rand() % 2;
        }
        int foodCount = 2;
        for (Food& food : GameManager::GetInstance()->getFoodPool()->getObjects())
        {
            food.removeFood();
            if (foodCount > 0)
            {
                foodCount--;
                if(timesTables)
                {
                    int value = rand() % 12 + 1;
                    food.PlaceFood(findEmpty(), value);
                }
                else
                {
                    int value = rand() % 30 + 1;
                    food.PlaceFood(findEmpty(), value);
                }
            }
        }
        gameState = GameState::makingQuestion;
        timer += 15;
        if (timer > 45)
        {
            timer = 45;
        }
        snake->Grow();
    }
    else //incorrect answer
    {
        snake->BeginShrink();
        timer -= 5;
    }
}

ObjectPool<Food>* const GameManager::getFoodPool()
{
    return foodPool;
}

void const GameManager::Draw(sf::RenderWindow& window, int gridSize)
{
    // Draw question UI
    if (operands[0] * operands[1] != 0) //check that both are not 0 - if one is 0 then multiplying them gives 0
    {
        char symbol;
        if (timesTables)
        {
            symbol = '*';
        }
        else
        {
            symbol = '+';
        }
        sf::String string = std::to_string(operands[0]) + ' ' + symbol + ' ' + std::to_string(operands[1]) + " =";
        sf::Text text(ResourceHolder::font,string,50);
        text.setPosition(sf::Vector2f(200, 100));
        window.draw(text);
    }

    //draw timer if timer enabled
    if (gameMode == GameMode::timesTablesTimer)
    {
        sf::Text text(ResourceHolder::font, "Timer: " + std::to_string((int)timer), 50);
        text.setPosition(sf::Vector2f(370, 20));
        window.draw(text);
    }

    //draw snake
    snake->Draw(window, gridSize);
}

sf::Vector2i const GameManager::findEmpty(bool avoidSnake)
{
    bool empty = 0;
    sf::Vector2i position;
    while (!empty)
    {
        empty = 1;
        position = sf::Vector2i(rand() % gameArea[0], rand() % gameArea[1]);
        for (Food& const food : foodPool->getObjects())
        {
            if (food.getPosition() == position)
            {
                empty = 0;
            }
        }
        if (snake != nullptr)
        {
            if (snake->checkCollision(position, 0, 1))
            {
                empty = 0;
            }
        }
    }
    return position;
}

void GameManager::startGame(int const newGameArea[2]) //Start the game
{
    snake = new Snake(sf::Vector2i(5, 5));
    snake->SetMoveDelay(0.4f);
    moveTimer = -3.f;
    gameArea[0] = newGameArea[0];
    gameArea[1] = newGameArea[1];

    int foodCount = 2;
    for (Food& food : GameManager::GetInstance()->getFoodPool()->getObjects())
    {
        food.removeFood();
        if (foodCount > 0)
        {
            foodCount--;
            int value = rand() % 12 + 1;
            food.PlaceFood(findEmpty(), value);
        }
    }

    if (gameMode == GameMode::timesTablesTimer)
    {
        timer = 34.f;
    }

    if (gameMode == GameMode::timesTables || gameMode == GameMode::timesTablesTimer)
    {
        timesTables = true;
    }
    else if (gameMode == GameMode::addition)
    {
        timesTables = false;
    }
    else
    {
        timesTables = rand() % 2;
    }

    gameState = GameState::makingQuestion;
    operands[0] = 0;
    operands[1] = 0;
}

void GameManager::update(sf::Vector2i direction, float const& deltaTime) // Move the snake
{
    moveTimer += deltaTime;
    if (gameMode == GameMode::timesTablesTimer)
    {
        timer -= deltaTime;
        if (timer <= 0)
        {
            endGame();
        }
    }
    if (moveTimer > snake->GetMoveDelay())
    {
        moveTimer = 0;
        int foodEaten = snake->Move(direction, foodPool->getObjects());
        if (!snake->getAlive())
        {
            endGame();
        }
        else if (foodEaten != 999)
        {
            EatFood(foodEaten);
        }
    }
    
}

void GameManager::endGame()
{
    gameState = GameState::dead;
}

GameManager::GameState GameManager::getGameState()
{
    return gameState;
}

int GameManager::getScore()
{
    return snake->getBestLength();
}

void GameManager::MainMenu()
{
    gameState = GameState::mainMenu;
}

void GameManager::setGameMode(GameMode newMode)
{
    gameMode = newMode;
}

GameManager::GameMode GameManager::getGameMode()
{
    return gameMode;
}

