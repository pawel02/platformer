#include "GameState.hpp"
#include "../EventSystem/KeyboardEvent.hpp"

GameState::GameState(unsigned int windowWidth, unsigned int windowHeight) noexcept
	:window{ sf::VideoMode{windowWidth, windowHeight}, "Platformer"},
	currTime{ clock.getElapsedTime().asMilliseconds() },
	obstacleManager{{windowWidth, windowHeight}},
	player{ {windowWidth, windowHeight}, &eventsManager, {30, 30}, 0.5f, &obstacleManager }
{
	initialize();
}

GameState::~GameState() noexcept
{
}

void GameState::initialize() noexcept
{
	window.setFramerateLimit(120.0f);
	if (!font.loadFromFile("../res/roboto.ttf"))
	{	
		// TODO log that could not load font
	}

	scoreText.setFont(font);
	deathText.setFont(font);
	deathScore.setFont(font);

	scoreText.setString("0");
	scoreText.setCharacterSize(20);
	scoreText.setColor(sf::Color::White);
	scoreText.setPosition(5, 5);

	deathText.setString("       YOU DIED!!!\nPress space to restart");
	deathText.setCharacterSize(32);
	deathText.setColor(sf::Color::White);
	deathText.setStyle(sf::Text::Bold);
	deathText.setPosition(window.getSize().x / 2 - (deathText.getLocalBounds().width / 2), window.getSize().y / 2 - (deathText.getCharacterSize() / 2));

	deathScore.setString("Score: 0");
	deathScore.setCharacterSize(24);
	deathScore.setColor(sf::Color::White);
	deathScore.setPosition(window.getSize().x / 2 - (deathText.getLocalBounds().width / 2), (window.getSize().y / 2 - (deathText.getCharacterSize() / 2)) + 90);
}

int GameState::gameLoop()
{
	while (window.isOpen())
	{
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			switch (ev.type)
			{
				case sf::Event::Closed:
				{
					window.close();
					break;
				}
				case (sf::Event::KeyPressed):
				{
					eventsManager.dispatch(ev.type, std::make_shared<KeyPressedEvent>(ev.key.code));
					if (ev.key.code == sf::Keyboard::Space && hasDied)
					{
						player.restart();
						obstacleManager.restart();
						hasDied = false;
					}
					break;
				}
				case (sf::Event::KeyReleased):
				{
					eventsManager.dispatch(ev.type, std::make_shared<KeyReleasedEvent>(ev.key.code));
					break;
				}
				default:
				{
					eventsManager.dispatch(ev.type, std::make_shared<BasicEvent>());
				}
			}
		}

		window.clear();

		// calcualte deltaTime
		float deltaTime = static_cast<float>(clock.getElapsedTime().asMicroseconds() - currTime) / 1000.0f;
		currTime = clock.getElapsedTime().asMicroseconds();

		if (!hasDied)
		{
			const std::vector<sf::RectangleShape> obstacles = obstacleManager.update(deltaTime);
			for (const auto& obstacle : obstacles)
			{
				window.draw(obstacle);
			}
		}
		

		PlayerInfo playerInfo = player.update(deltaTime);
		if (playerInfo.playerState == 0x02)
		{
			// Show the death screen with a final score
			hasDied = true;
			deathScore.setString("Final score: " + std::to_string(playerInfo.score));
			window.draw(deathText);
			window.draw(deathScore);
		}
		else
		{
			window.draw(*playerInfo.sprite);
			scoreText.setString("Score: " + std::to_string(playerInfo.score));
			window.draw(scoreText);

		}

		window.display();
	}

	return 0;
}