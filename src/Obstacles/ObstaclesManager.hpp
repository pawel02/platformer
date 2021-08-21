#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

/*
Represents a rectangle obstacles
that will be in the same position as the player moves up
on the map
Spawns them at a specified rate
*/

class ObstacleManager
{
public:
	ObstacleManager(
		const sf::Vector2u& windowSize
	) noexcept;
	~ObstacleManager() noexcept;

	const std::vector<sf::RectangleShape>& getObstacles() const noexcept { return obstacles; }
	const std::vector<sf::RectangleShape>& update(float deltaTime);

	const float& getSpeed() const noexcept { return speed; }
	void restart() noexcept;

private:
	void spawnObstacle() noexcept;

private:
	sf::Vector2u windowSize;
	std::vector<sf::RectangleShape> obstacles;

	// How many obstacles should be spawned per second
	float spawnRate = 1600.0f;
	float spawnRateMin = 1000.0f;
	float spawnRateMax = 1600.0f;
	float lastUpdated = 0.0f;
	float speed = 0.1f;
	const float increaseSpeedOverTime = 0.000001f;

};