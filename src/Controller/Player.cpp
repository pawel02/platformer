#include <vector>
#include "Player.hpp"
#include "../common/bit.hpp"

Player::Player(const sf::Vector2u& windowSize, EventsManager* eventsManager, 
	const glm::vec2& playerSize, const float& maxSpeed, const ObstacleManager* obstacleManager) noexcept
	:windowSize{windowSize},
	eventsManager{eventsManager},
	playerSize{playerSize},
	maxSpeed{ maxSpeed },
	originalMaxSpeed{ maxSpeed},
	floorPos{ windowSize.y - playerSize.y },
	velocity{0.0f, 0.0f},
	obstacleManager{obstacleManager}
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

PlayerInfo Player::update(float deltaTime)
{
	if (playerState != 0x02)
	{
		// check if the user can jump
		if ((keys & BIT(0) || keys & BIT(3)) && canJump) // W / Space go up
		{
			velocity.y = jumpForce;
			canJump = false;
		}

		calculateSideMovement(deltaTime);

		// adds gravity into the mix
		velocity.y += deltaTime * gravity;

		// update the users position
		pos.x += velocity.x * deltaTime;
		pos.y += velocity.y * deltaTime;

		// do collision detection
		collision(deltaTime);
		calculateObstacleCollision(deltaTime);

		// if you hit the ground after starting then you lost
		if (pos.y + playerSize.y >= windowSize.y && playerState == 0x01)
		{
			died();
		}
		else if (playerState == 0x01)
		{
			// add some score
			score += static_cast<unsigned int>(0.5f * deltaTime);
		}

		sprite.setPosition(pos);
	}
	
	return 
	{
		&sprite,
		playerState,
		score
	};
}

void Player::restart()
{
	pos = { (windowSize.x / 2) - (playerSize.x / 2), floorPos };

	velocity = { 0.0f, 0.0f };
	canJump = true;
	maxSpeed = originalMaxSpeed;
	jumpForce = -1.2f;
	shouldIgnoreSideInputs = false;

	speedIncreaseOverTime = 0.001f;

	keys = 0x00;
	playerState = 0x00;
	score = 0;
	sprite.setPosition(pos);
}



void Player::handleKeyPressed(KeyPressedEvent* ev)
{
	if (playerState != 0x02)
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
}

void Player::handleKeyReleased(KeyReleasedEvent* ev)
{
	if (playerState != 0x02)
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
}

void Player::calculateWallBounce(float deltaTime)
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

		// Increased the speed if you are constantly jumping and hitting side walls
		maxSpeed = std::min(maxSpeed + (speedIncreaseOverTime * deltaTime), 1.3f);
		jumpForce = std::max(jumpForce - (speedIncreaseOverTime * deltaTime), -2.3f);
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

void Player::collision(float deltaTime)
{
	// You hit the ground reset everything
	if (pos.y >= floorPos)
	{
		pos.y = floorPos;
		canJump = true;

		// if you are no longer jumping then reset the speed boost
		if (!(keys & BIT(0) || keys & BIT(3)))
		{
			maxSpeed = originalMaxSpeed;
			jumpForce = -1.5f;
		}
	}

	// check if the user has hit a wall
	if (pos.x <= 0.0f)
	{
		pos.x = 0.0f;
		calculateWallBounce(deltaTime);
	}
	if (pos.x >= windowSize.x - playerSize.x)
	{
		pos.x = windowSize.x - playerSize.x;
		calculateWallBounce(deltaTime);
	}

}

void Player::calculateObstacleCollision(float deltaTime)
{
	// Go through all the obstacles
	/*
	you can bounce off the side walls and it will launch you in the opposite direction
	You can also bounce off the top
	Hitting the roof results in the velocity.being set to 0
	*/
	const std::vector<sf::RectangleShape> obstacles = obstacleManager->getObstacles();

	for (size_t i = 0; i < obstacles.size(); i++)
	{
		sf::FloatRect obstacleBounds = obstacles[i].getGlobalBounds();

		// if you hit the top part of an obstacle then let it behave just like hitting
		// any other part of the ground
		if (
			obstacleBounds.intersects(sprite.getGlobalBounds()) &&
			pos.y + playerSize.x >= obstacleBounds.top &&
			pos.y <= obstacleBounds.top
			&& !(pos.x + playerSize.x < obstacleBounds.left
				|| pos.x > obstacleBounds.left + obstacleBounds.width)
			&& velocity.y > 0.0f
			)
		{
			pos.y = obstacleBounds.top - playerSize.x;
			velocity.y = obstacleManager->getSpeed() * 2;
			canJump = true;

			if (playerState == 0x00)
			{
				playerState = 0x01;
			}
		}

		// if you hit the bottom of the obstacle then set velocity.y to twice the speed of obstacles
		// start falling
		if (
			obstacleBounds.intersects(sprite.getGlobalBounds()) &&
			pos.y <= obstacleBounds.top + obstacleBounds.height &&
			pos.y + playerSize.y > obstacleBounds.top + obstacleBounds.height
			&& !(pos.x + playerSize.x < obstacleBounds.left
				|| pos.x > obstacleBounds.left + obstacleBounds.width)
			)
		{
			pos.y = obstacleBounds.top + obstacleBounds.height;
			velocity.y = obstacleManager->getSpeed() * 2;
		}
		// if you hit either side then its just like hitting a wall 
		// where you can bounce in the opposite direction
		if (
			obstacleBounds.intersects(sprite.getGlobalBounds()) &&
			pos.x + playerSize.x >= obstacleBounds.left
			&& pos.x <= obstacleBounds.left
			&& (
				pos.y + playerSize.y > obstacleBounds.top
				&& pos.y < obstacleBounds.top + obstacleBounds.height
				)
			)
		{
			pos.x = obstacleBounds.left - playerSize.x;
			calculateWallBounce(deltaTime);
		}

		//right wall
		if (
			obstacleBounds.intersects(sprite.getGlobalBounds()) &&
			pos.x <= obstacleBounds.left + obstacleBounds.width
			&& pos.x + playerSize.x >= obstacleBounds.left + obstacleBounds.width
			&& (
				pos.y + playerSize.y > obstacleBounds.top
				&& pos.y < obstacleBounds.top + obstacleBounds.height
				)
			)
		{
			pos.x = obstacleBounds.left + obstacleBounds.width;
			calculateWallBounce(deltaTime);
		}
	
	}
}

void Player::died()
{
	playerState = 0x02;
}




