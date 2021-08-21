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

		const std::vector<sf::RectangleShape> obstacles = obstacleManager.update(deltaTime);
		for (const auto& obstacle : obstacles)
		{
			window.draw(obstacle);
		}
		window.draw(player.update(deltaTime));

		window.display();
	}

	return 0;
}