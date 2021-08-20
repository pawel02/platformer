#include <vector>
#include <iostream>
#include "Player.hpp"
#include "../common/bit.hpp"

Player::Player(const sf::Vector2u& windowSize, EventsManager* eventsManager, const glm::vec2& playerSize, const float& maxSpeed) noexcept
	:windowSize{windowSize},
	eventsManager{eventsManager},
	playerSize{playerSize},
	maxSpeed{ maxSpeed },
	floorPos{ windowSize.y - playerSize.y },
	velocity{0.0f, 0.0f}
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
	pos = { (windowSize.x / 2) - (playerSize.x / 2), floorPos };
	sprite.setPosition(pos);
}

const sf::Drawable& Player::update(float deltaTime)
{
	if ((keys & BIT(0) || keys & BIT(3)) && canJump) // W / Space go up
	{
		velocity.y = jumpForce;
		canJump = false;
	}

	calculateSideMovement(deltaTime);

	// start moving the player towards the bottom of the screen
	if (!canJump)
	{
		velocity.y += deltaTime * gravity;
	}

	pos.x += velocity.x * deltaTime;
	pos.y += velocity.y * deltaTime;

	// make sure that you cannot go lower than the screen
	if (pos.y >= floorPos)
	{
		pos.y = floorPos;
		canJump = true;
	}

	// check if the user has hit a wall
	if (pos.x <= 0.0f)
	{
		pos.x = 0.0f;
		calculateWallBounce();
	}
	if (pos.x >= windowSize.x - playerSize.x)
	{
		pos.x = windowSize.x - playerSize.x;
		calculateWallBounce();
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

void Player::calculateWallBounce()
{
	// if you have upward velocity and Space / W is pressed
	// then bounce of the wall in the opposite direction
	if (velocity.y < 0.0f
		&& (keys & BIT(0) || keys & BIT(3)))
	{
		velocity.y = jumpForce;
		if (velocity.x >= 0.0f)
		{
			velocity.x = -maxSpeed;
		}
		else
		{
			velocity.x = maxSpeed;
		}
	}
}

void Player::calculateSideMovement(float deltaTime)
{
	// slowly decrease the velocity when the keys are not down
	if (velocity.x > 0.0f && canJump)
		velocity.x = std::max(velocity.x - (decceleration * deltaTime), 0.0f);
	if (velocity.x < 0.0f && canJump)
		velocity.x = std::min(velocity.x + (decceleration * deltaTime), 0.0f);

	if (keys & BIT(1) && canJump) // A go left
	{
		velocity.x = std::max(velocity.x - (acceleration * deltaTime), maxSpeed * -1.0f);
	}
	else if (keys & BIT(1))
	{
		velocity.x = std::max(velocity.x - (acceleration * sideInJumpControl * deltaTime), maxSpeed * -1.0f);
	}


	if (keys & BIT(2) && canJump) // D go right
	{
		velocity.x = std::min(velocity.x + (acceleration * deltaTime), maxSpeed);
	}
	else if (keys & BIT(2)) // D go right
	{
		velocity.x = std::min(velocity.x + (acceleration * sideInJumpControl * deltaTime), maxSpeed);
	}
}



