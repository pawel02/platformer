#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../EventSystem/Event.hpp"
#include "../Controller/Player.hpp"
#include "../Obstacles/ObstaclesManager.hpp"

/*
Manages the whole game things such as spawning the player
Main game loop etc
*/
class GameState
{
public:
	GameState(unsigned int windowWidth, unsigned int windowHeight) noexcept;
	~GameState() noexcept;

	int gameLoop();

private:
	void initialize() noexcept;

private:
	sf::RenderWindow window;
	EventsManager eventsManager;

	Player player;
	ObstacleManager obstacleManager;

	sf::Clock clock;
	sf::Int32 currTime;
};