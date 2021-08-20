#include "GameState.hpp"
#include "../EventSystem/KeyboardEvent.hpp"

GameState::GameState(unsigned int windowWidth, unsigned int windowHeight) noexcept
	:window{ sf::VideoMode{windowWidth, windowHeight}, "Platformer"}
{

}

GameState::~GameState() noexcept
{

}

void GameState::initialize() noexcept
{
	window.setFramerateLimit(120.0f);
	currTime = clock.getElapsedTime().asMilliseconds();
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
		window.display();
	}

	return 0;
}