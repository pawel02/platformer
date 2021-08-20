#pragma once
#include <SFML/Graphics.hpp>
#include "../EventSystem/KeyboardEvent.hpp"
#include <glm/glm.hpp>

/*
Player class that will move the player about and
includes all of the collision detection algorithms
*/
class Player
{
public:
	Player(
		const sf::Vector2u& windowSize,
		EventsManager* eventsManager,
		const glm::vec2& playerSize,
		const float& movementSpeed
	) noexcept;

	~Player() noexcept;

	const sf::Drawable& update(float deltaTime);


private:

	/*
	Creates the sprite with the texture
	*/
	void initialize();

	void handleKeyPressed(KeyPressedEvent* ev);
	void handleKeyReleased(KeyReleasedEvent *ev);

private:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f pos;

	sf::Vector2u windowSize;

	EventsManager* eventsManager;
	glm::vec2 playerSize;

	glm::vec2 velocity;

	float movementSpeed;

	char keys = 0x00;
};