#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#pragma comment (lib, "legacy_stdio_definitions.lib")
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

	sf::Font font;
	sf::Text scoreText;

	sf::Text deathText;
	sf::Text deathScore;

	Player player;
	ObstacleManager obstacleManager;

	sf::Clock clock;
	sf::Int32 currTime;

	bool hasDied = false;
};