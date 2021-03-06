#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <glm/glm.hpp>
#include "../EventSystem/KeyboardEvent.hpp"
#include "../Obstacles/ObstaclesManager.hpp"

struct PlayerInfo
{
	const sf::Drawable* sprite;
	char playerState = 0x00;
	unsigned int score = 0;
};

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
		const float& maxSpeed,
		const ObstacleManager* obstacleManager
	) noexcept;

	~Player() noexcept;

	PlayerInfo update(float deltaTime);

	// resets the player
	void restart();

private:

	/*
	Creates the sprite with the texture
	*/
	void initialize();

	void handleKeyPressed(KeyPressedEvent* ev);
	void handleKeyReleased(KeyReleasedEvent *ev);

	void calculateWallBounce(float deltaTime);
	void calculateSideMovement(float deltaTime);
	void collision(float deltaTime);
	void calculateObstacleCollision(float deltaTime);

	void died();

	void playJump();

private:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::SoundBuffer buffer;
	sf::Sound sound;

	sf::SoundBuffer scoreSoundbuffer;
	sf::Sound scoreSound;
	bool scoreSoundPlayed = false;

	sf::Vector2u windowSize;

	EventsManager* eventsManager;
	const ObstacleManager* obstacleManager;
	glm::vec2 playerSize;


	// Player movement
	sf::Vector2f pos;
	glm::vec2 velocity = {0.0f, 0.0f};
	bool canJump = true;
	float originalMaxSpeed;
	float maxSpeed;
	const float decceleration = 0.005f;
	const float acceleration = .01f;
	float jumpForce = -1.2f;
	const float sideInJumpControl = 0.1f;
	const float gravity = 0.005f;
	float floorPos = 0.0f;
	float shouldIgnoreSideInputs = false;

	float speedIncreaseOverTime = 0.001f;

	char keys = 0x00;

	/*
	0x00 = game started
	0x01 = player went on first block
	0x02 = player died
	*/
	char playerState = 0x00;
	unsigned int score = 0;

};