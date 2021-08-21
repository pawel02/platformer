#include <random> 
#include <iostream>
#include "ObstaclesManager.hpp"

ObstacleManager::ObstacleManager(const sf::Vector2u& windowSize) noexcept
	:windowSize{windowSize}
{
	initialize();
}

ObstacleManager::~ObstacleManager() noexcept
{
}

const std::vector<sf::RectangleShape>& ObstacleManager::update(float deltaTime)
{
	lastUpdated += deltaTime;
	if (lastUpdated >= spawnRate)
	{
		spawnObstacle();
		lastUpdated = 0.0f;
		std::random_device r;
		std::default_random_engine e1(r());
		std::uniform_int_distribution<int> uniform_dist(1000, 1600);
		spawnRate = static_cast<float>(uniform_dist(e1));
	}
	// move all the obstacles down at a constant speed
	for (auto& obstacle : obstacles)
	{
		obstacle.setPosition(obstacle.getPosition().x, obstacle.getPosition().y + (speed * deltaTime));

		// When the obstacle reaches the bottom remove it
		if (obstacle.getPosition().y + obstacle.getSize().y > windowSize.y)
		{
			obstacles.erase(std::find_if(std::begin(obstacles), std::end(obstacles), [&](sf::RectangleShape& element) {
				return element.getGlobalBounds() == obstacle.getGlobalBounds();
			}));
		}
	}

	return obstacles;
}

void ObstacleManager::spawnObstacle() noexcept
{
	//simply adds an obstacle from the top
	obstacles.emplace_back(sf::RectangleShape{ {150.0f, 20.0f} });
	(obstacles.end() - 1)->setFillColor(sf::Color::White);
	std::random_device r;
	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_dist(0, windowSize.x - 150);
	(obstacles.end() - 1)->setPosition(uniform_dist(e1), 0);
}

void ObstacleManager::initialize() noexcept
{
	// For the moment only create 1 obstacle
	// so that I can test the collisions with the player
}
