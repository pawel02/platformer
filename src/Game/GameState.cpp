#include <iostream>
#include <random>
#include "GameState.hpp"
#include "../EventSystem/KeyboardEvent.hpp"

GameState::GameState(unsigned int windowWidth, unsigned int windowHeight) noexcept
	:window{ sf::VideoMode{windowWidth, windowHeight}, "Platformer"},
	currTime{ clock.getElapsedTime().asMilliseconds() },
	obstacleManager{{windowWidth, windowHeight}},
	player{ {windowWidth, windowHeight}, &eventsManager, {45, 45}, 0.5f, &obstacleManager }
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

	deathText.setString("       YOU DIED GG!!!\nPress space to restart");
	deathText.setCharacterSize(32);
	deathText.setColor(sf::Color::White);
	deathText.setStyle(sf::Text::Bold);
	deathText.setPosition(window.getSize().x / 2 - (deathText.getLocalBounds().width / 2), window.getSize().y / 2 - (deathText.getCharacterSize() / 2));

	deathScore.setString("Score: 0");
	deathScore.setCharacterSize(24);
	deathScore.setColor(sf::Color::White);
	deathScore.setPosition(window.getSize().x / 2 - (deathText.getLocalBounds().width / 2), (window.getSize().y / 2 - (deathText.getCharacterSize() / 2)) + 90);

	// create the background
	if (!backgroundTex.loadFromFile("../res/background.png"))
	{
		// TODO log unable to load file
		std::cout << "unable to load background\n";
	}
	backgroundTex.setRepeated(true);
	background.setTexture(backgroundTex);

	// load in the background shader
	if (!backgroundShader.loadFromFile("../res/shaders/background.vert", sf::Shader::Vertex))
	{
		// TODO
	}

	if (!backgroundShader.loadFromFile("../res/shaders/background.fs", sf::Shader::Fragment))
	{
		// TODO
	}
	backgroundShader.setUniform("tex", sf::Shader::CurrentTexture);
	backgroundShader.setUniform("deltaTime", backgroundMove);

	// initialize the loss sounds
	if (!lossSoundBuffer.loadFromFile("../res/loss.wav"))
	{
		// TODO
	}
	lossSound.setBuffer(lossSoundBuffer);

	// load all the voices
	for (size_t i = 1; i < 4; i++)
	{
		if (!voicesBuffer[i -1].loadFromFile("../res/voice" + std::to_string(i) + ".wav"))
		{
			// TODO
		}
		voices[i - 1].setBuffer(voicesBuffer[i - 1]);
		voices[i - 1].setVolume(6.0f);
	}

	//load background music and play it
	if (!backgroundMusic.openFromFile("../res/background_music.wav"))
	{
		// TODO
	}
	backgroundMusic.play();
	backgroundMusic.setVolume(4.0f);
}

void GameState::updateVoiceSounds(float deltaTime)
{
	if (playNextVoiceSound <= 0.0f)
	{
		std::random_device r;
		std::default_random_engine e1(r());
		std::uniform_int_distribution<int> uniform_dist(2000, 5000);
		playNextVoiceSound = uniform_dist(e1);

		std::random_device r1;
		std::default_random_engine e(r1());
		std::uniform_int_distribution<int> uniform_dist2(0, voices.size() - 1);
		playNextSoundIndex = uniform_dist2(e);
		voices[playNextSoundIndex].play();
	}

	playNextVoiceSound -= deltaTime;

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
						hasPlayerLossSound = false;
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

		backgroundMove += deltaTime * 0.0001f;
		backgroundShader.setUniform("deltaTime", backgroundMove);
		// draw the background first
		window.draw(background, &backgroundShader);


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

			if (!hasPlayerLossSound)
			{
				lossSound.play();
				hasPlayerLossSound = true;
			}
		}
		else
		{
			window.draw(*playerInfo.sprite);
			scoreText.setString("Score: " + std::to_string(playerInfo.score));
			window.draw(scoreText);

		}
		updateVoiceSounds(deltaTime);

		if (backgroundMusic.getDuration() == backgroundMusic.getPlayingOffset())
		{
			backgroundMusic.play();
		}

		window.display();
	}

	return 0;
}