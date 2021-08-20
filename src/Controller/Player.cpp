#include <vector>
#include <iostream>
#include "Player.hpp"
#include "../common/bit.hpp"

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
	// subscribe to all the events
	eventsManager->subscribe(sf::Event::KeyPressed, [&](std::shared_ptr<BasicEvent> ev) {
		handleKeyPressed(static_cast<KeyPressedEvent*>(ev.get()));
	});

	eventsManager->subscribe(sf::Event::KeyReleased, [&](std::shared_ptr<BasicEvent> ev) {
		handleKeyReleased(static_cast<KeyReleasedEvent*>(ev.get()));
	});

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
	pos = { (windowSize.x / 2) - (playerSize.x / 2), (windowSize.y / 2) - (playerSize.y / 2) };
	sprite.setPosition(pos);
}

const sf::Drawable& Player::update(float deltaTime)
{
	float deltaSpeed = deltaTime * movementSpeed;
	if (keys & BIT(0) || keys & BIT(3)) // W / Space go up
	{

	}
	if (keys & BIT(1)) // A go left
	{
		pos.x -= deltaSpeed;
	}
	if (keys & BIT(2)) // D go right
	{
		pos.x += deltaSpeed;
	}
	sprite.setPosition(pos);

	return sprite;
}



void Player::handleKeyPressed(KeyPressedEvent* ev)
{
	switch (ev->getKeyCode())
	{
		case sf::Keyboard::W:
		{
			keys |= BIT(0);
			break;
		}
		case sf::Keyboard::A:
		{
			keys |= BIT(1);
			break;
		}
		case sf::Keyboard::D:
		{
			keys |= BIT(2);
			break;
		}
		case sf::Keyboard::Space:
		{
			keys |= BIT(3);
			break;
		}
	}
}

void Player::handleKeyReleased(KeyReleasedEvent* ev)
{
	switch (ev->getKeyCode())
	{
		case sf::Keyboard::W:
		{
			keys ^= BIT(0);
			break;
		}
		case sf::Keyboard::A:
		{
			keys ^= BIT(1);
			break;
		}
		case sf::Keyboard::D:
		{
			keys ^= BIT(2);
			break;
		}
		case sf::Keyboard::Space:
		{
			keys ^= BIT(3);
			break;
		}
	}
}



