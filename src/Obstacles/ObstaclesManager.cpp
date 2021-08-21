#include <random> 
#include <iostream>
#include "ObstaclesManager.hpp"

ObstacleManager::ObstacleManager(const sf::Vector2u& windowSize) noexcept
	:windowSize{windowSize}
{
}

ObstacleManager::~ObstacleManager() noexcept
{
}

void ObstacleManager::restart() noexcept
{
	obstacles.clear();
	spawnRate = 1600.0f;
	spawnRateMin = 1000.0f;
	spawnRateMax = 1600.0f;
	lastUpdated = 0.0f;
	speed = 0.1f;
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
		std::uniform_int_distribution<int> uniform_dist(spawnRateMin, spawnRateMax);
		spawnRate = static_cast<float>(uniform_dist(e1));
	}
	speed += increaseSpeedOverTime * deltaTime;
	spawnRateMin = std::max(spawnRateMin - (increaseSpeedOverTime * 2 * deltaTime), 500.0f);
	spawnRateMax = std::max(spawnRateMax - (increaseSpeedOverTime * 2 * deltaTime), 700.0f);
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
	int rand = uniform_dist(e1);

	// make sure that you don't have an obstacle spawning right above the one below
	if (obstacles.size() > 2)
	{
		while (rand > (obstacles.end() - 2)->getPosition().x - ((obstacles.end() - 2)->getSize().x / 2)
			&& rand < (obstacles.end() - 2)->getPosition().x + ((obstacles.end() - 2)->getSize().x / 2)
			)
		{
			rand = uniform_dist(e1);
		}
	}
	(obstacles.end() - 1)->setPosition(rand, 0);
}