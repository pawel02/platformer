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
		const float& maxSpeed
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

	void calculateWallBounce(float deltaTime);
	void calculateSideMovement(float deltaTime);

private:
	sf::Texture texture;
	sf::Sprite sprite;

	sf::Vector2u windowSize;

	EventsManager* eventsManager;
	glm::vec2 playerSize;


	// Player movement
	sf::Vector2f pos;
	glm::vec2 velocity = {0.0f, 0.0f};
	bool canJump = true;
	float originalMaxSpeed;
	float maxSpeed;
	const float decceleration = 0.005f;
	const float acceleration = .01f;
	float jumpForce = -1.5f;
	const float sideInJumpControl = 0.1f;
	const float gravity = 0.005f;
	float floorPos = 0.0f;
	float shouldIgnoreSideInputs = false;

	float speedIncreaseOverTime = 0.001f;

	char keys = 0x00;
};