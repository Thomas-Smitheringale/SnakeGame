/*
    Snake - Start Project

    This project is set up to link to SFML (a library supporting 2D graphics, sound, user input and networing)
    There is a subdirectory in the project called SFML-3.0.0 containing SFML headers and libraries
    The project settings are configured to look for these files

    SFML reference: https://www.sfml-dev.org/documentation/3.0.0/
    SFML Tutorials: https://www.sfml-dev.org/tutorials/3.0/
*/

//TO DO
//finish switching buttons over to using the button class

#include <SFML/Graphics.hpp>
#include <iostream>
#include "LinkList.h"
#include "Snake.h"
#include "ObjectPool.h"
#include "Food.h"
#include "GameManager.h"
#include "ResourceHolder.h"
#include "Button.h"
#include <map>
GameManager* GameManager::instance = nullptr;
sf::Font ResourceHolder::font = sf::Font("arial.ttf");

//sf::Font font = sf::Font("arial.ttf");
int main()
{
    constexpr int gridSize = 50;
    constexpr int gameArea[2] = { 10,10 }; 

    sf::RenderWindow window(
        sf::VideoMode({ gridSize * (gameArea[0] + 2), gridSize * (gameArea[1] + 2) + 200 }), "Snake");

    sf::Clock clock;

    sf::Vector2i snakeDirection = sf::Vector2i(0, -1);;
    srand(time(0));

    GameManager* gameManager = GameManager::GetInstance();

    bool LMBPrev = false; // Was left mouse button pressed last frame?
    bool quit = false;
    std::map<std::string, Button> buttons{};

    buttons["Main_Play"] = Button(sf::Vector2f(window.getSize().x / 2 - 150, window.getSize().y / 2 - 300),sf::Vector2f(300,100),"Play", sf::Color(0, 200, 0));
    buttons["Main_Mode"] = Button(sf::Vector2f(window.getSize().x / 2 - 150, window.getSize().y / 2 - 100), sf::Vector2f(300, 100), "Mode: Times Tables", sf::Color(0, 200, 0));
    buttons["Main_Mode"].setTextSize(30);
    buttons["Main_Quit"] = Button(sf::Vector2f(window.getSize().x / 2 - 150, window.getSize().y / 2 + 100), sf::Vector2f(300, 100), "Quit", sf::Color(0, 200, 0));
    
    buttons["Lose_Continue"] = Button(sf::Vector2f(window.getSize().x / 2 - 150, window.getSize().y / 2 + 100), sf::Vector2f(300, 100), "Continue", sf::Color(0, 200, 0));

    //window
    while (window.isOpen() && !quit)
    {
        float deltaTime = clock.restart().asSeconds();
        window.clear();

        // Keyboard and mouse input
        while (std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            snakeDirection = sf::Vector2i(-1, 0);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            snakeDirection = sf::Vector2i(1, 0);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            snakeDirection = sf::Vector2i(0, -1);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            snakeDirection = sf::Vector2i(0, 1);
        }

        bool LMB = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left); // is Left Mouse Button pressed?
        bool click = 0;
        if (!LMB && LMBPrev) //if left mouse button was pressed last frame but released this frame, click is true
        {
            click = 1;
        }
        LMBPrev = LMB;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);


        //Draw Main Menu
        if (gameManager->getGameState() == GameManager::GameState::mainMenu)
        {
            // Draw backgound
            sf::RectangleShape rectangle = sf::RectangleShape(sf::Vector2f(window.getSize().x, window.getSize().y)); 
            rectangle.setFillColor(sf::Color(0, 0, 220));
            window.draw(rectangle);

            if (click && buttons["Main_Play"].ButtonClicked(mousePos))
            {
                gameManager->startGame(gameArea);
            }
            else if (click && buttons["Main_Mode"].ButtonClicked(mousePos))
            {
                switch (gameManager->getGameMode())
                {
                case GameManager::GameMode::timesTables:
                    gameManager->setGameMode(GameManager::GameMode::addition);
                    buttons["Main_Mode"].setText("Mode: Addition");
                    break;
                case GameManager::GameMode::addition:
                    gameManager->setGameMode(GameManager::GameMode::timesTablesTimer);
                    buttons["Main_Mode"].setText("Mode: Times Tables Timer");
                    buttons["Main_Mode"].setTextSize(25);
                    break;
                case GameManager::GameMode::timesTablesTimer:
                    gameManager->setGameMode(GameManager::GameMode::mixed);
                    buttons["Main_Mode"].setText("Mode: Mixed");
                    buttons["Main_Mode"].setTextSize(30);
                    break;
                case GameManager::GameMode::mixed:
                    gameManager->setGameMode(GameManager::GameMode::timesTables);
                    buttons["Main_Mode"].setText("Mode: Times Tables");
                    break;
                default:
                    break;
                }
            }
            else if (click && buttons["Main_Quit"].ButtonClicked(mousePos))
            {
                quit = true;
            }

            buttons["Main_Play"].Draw(window);
            buttons["Main_Mode"].Draw(window);
            buttons["Main_Quit"].Draw(window);
        }
        

        //Game
        else
        {


            
            if (gameManager->getGameState() != GameManager::GameState::dead)
            {
                gameManager->update(snakeDirection, deltaTime);
            }
            

            // Draw backgound
            sf::RectangleShape rectangle = sf::RectangleShape(sf::Vector2f(window.getSize().x, 200)); //draw top area for UI
            rectangle.setFillColor(sf::Color(0, 0, 255));
            window.draw(rectangle);

            // Draw playable area
            //draw walls
            rectangle = sf::RectangleShape(sf::Vector2f(window.getSize().x, window.getSize().y - 200)); 
            rectangle.setFillColor(sf::Color(0, 200, 0));
            rectangle.setOutlineColor((sf::Color(0, 0, 0)));
            rectangle.setOutlineThickness(3);
            rectangle.setPosition(sf::Vector2f(0, 200));
            window.draw(rectangle);
            //draw game area
            rectangle = sf::RectangleShape(sf::Vector2f(window.getSize().x - gridSize * 2, window.getSize().y - 200 - gridSize * 2)); 
            rectangle.setFillColor(sf::Color(255, 150, 30));
            rectangle.setOutlineColor((sf::Color(0, 0, 0)));
            rectangle.setOutlineThickness(3);
            rectangle.setPosition(sf::Vector2f(50, 250));
            window.draw(rectangle);
            //draw grid over game area - half the squares have a grid of differently coloured squares over them, the other half are empty and just show the big square behind
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    if ((i + j) % 2 == 0)
                    {
                        sf::RectangleShape square = sf::RectangleShape(sf::Vector2f(gridSize,gridSize));
                        square.setFillColor(sf::Color(255, 120, 20));
                        square.setPosition(sf::Vector2f((i + 1) * gridSize, (j + 5) * gridSize));
                        window.draw(square);
                    }
                }
            }

            //Draw Score
            sf::Text score(ResourceHolder::font, "Score: " + std::to_string(gameManager->getScore()), 50);
            score.setPosition(sf::Vector2f(20,20));
            window.draw(score);

            // Draw food

            for (Food &food : GameManager::GetInstance()->getFoodPool()->getObjects())
            {
                food.Draw(window, gridSize);
            }
        
            gameManager->Draw(window, gridSize);

            if (gameManager->getGameState() == GameManager::GameState::dead)
            {
                // Draw backgound
                sf::RectangleShape rectangle = sf::RectangleShape(sf::Vector2f(600, 200)); //draw top area for UI
                rectangle.setFillColor(sf::Color(0, 0, 0,200));
                rectangle.setPosition(sf::Vector2f(window.getSize().x / 2 - 300, window.getSize().y / 2 - 200));
                window.draw(rectangle);

                // Show score
                sf::Text score(ResourceHolder::font, "You got a score of " + std::to_string(gameManager->getScore()) + ".\nWell Done!", 50);
                score.setPosition(sf::Vector2f(window.getSize().x / 2 - 250, window.getSize().y / 2 - 150));
                score.setFillColor(sf::Color(255, 255, 255));
                window.draw(score);

                buttons["Lose_Continue"].Draw(window);

                if (click && buttons["Lose_Continue"].ButtonClicked(mousePos))
                {
                    gameManager->MainMenu();
                }
            }

        }

        //draw snake and display window
        window.display();
    }
}