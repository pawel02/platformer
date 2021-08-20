#include <vector>
#include "Player.hpp"

Player::Player(const sf::Vector2u& windowSize, EventsManager* eventsManager, const glm::vec2& playerSize, const float& movementSpeed) noexcept
	:windowSize{windowSize},
	eventsManager{eventsManager},
	playerSize{playerSize},
	movementSpeed{movementSpeed}
{
	initialize();
}

Player::~Player() noexcept
{
}

void Player::initialize()
{
	// for the moment draw a red sprite
	if (!texture.create(playerSize.x, playerSize.y))
	{
		// TODO: Add to the logging system that could not load the texture
	}

	const int textureSize = playerSize.x * playerSize.y * 4;
	std::vector<sf::Uint8> pixels;
	pixels.resize(textureSize);

	for (size_t i = 0; i < textureSize; i+=4)
	{
		pixels[i] =     255; // r
		pixels[i + 1] = 0;   // g
		pixels[i + 2] = 0;   // b
		pixels[i + 3] = 255; // a
	}

	texture.update(pixels.data());
	texture.setSmooth(true);
	sprite.setTexture(texture);

	// put the sprite in the middle of the screen for now
	sprite.setPosition((windowSize.x / 2) - (playerSize.x / 2), (windowSize.y / 2) - (playerSize.y / 2));
}

const sf::Drawable& Player::update(float deltaTime)
{
	// Here we can update the players position etc
	return sprite;
}


