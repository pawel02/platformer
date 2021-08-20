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
	const std::vector<sf::RectangleShape>& update();

private:
	void initialize() noexcept;

private:
	sf::Vector2u windowSize;
	std::vector<sf::RectangleShape> obstacles;

	// How many obstacles should be spawned per second
	const float spawnRate = 1.0f;

};