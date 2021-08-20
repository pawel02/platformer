#include "ObstaclesManager.hpp"

ObstacleManager::ObstacleManager(const sf::Vector2u& windowSize) noexcept
	:windowSize{windowSize}
{
	initialize();
}

ObstacleManager::~ObstacleManager() noexcept
{
}

const std::vector<sf::RectangleShape>& ObstacleManager::update()
{
	// This is where the movement for the obstacles will happen
	return obstacles;
}

void ObstacleManager::initialize() noexcept
{
	// For the moment only create 1 obstacle
	// so that I can test the collisions with the player
	obstacles.emplace_back(sf::RectangleShape{ {100.0f, 20.0f} });
	obstacles[0].setFillColor(sf::Color::White);
	obstacles[0].setPosition(400, 700);
}
