#pragma once
#include <array>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#pragma comment (lib, "legacy_stdio_definitions.lib")
#include "../EventSystem/Event.hpp"
#include "../Controller/Player.hpp"
#include "../Obstacles/ObstaclesManager.hpp"

/*
Manages the whole game things such as spawning the player
Main game loop etc
*/
class GameState
{
public:
	GameState(unsigned int windowWidth, unsigned int windowHeight) noexcept;
	~GameState() noexcept;

	int gameLoop();

private:
	void initialize() noexcept;

	void updateVoiceSounds(float deltaTime);
private:
	sf::RenderWindow window;
	EventsManager eventsManager;

	sf::SoundBuffer lossSoundBuffer;
	sf::Sound lossSound;
	bool hasPlayerLossSound = false;

	std::array<sf::SoundBuffer, 3> voicesBuffer;
	std::array<sf::Sound, 3> voices;
	float playNextVoiceSound = 0.0f;
	int playNextSoundIndex = 0;

	sf::Music backgroundMusic;

	sf::Texture backgroundTex;
	float backgroundMove = 0.0f;
	sf::Sprite background;
	sf::Shader backgroundShader;

	sf::Font font;
	sf::Text scoreText;

	sf::Text deathText;
	sf::Text deathScore;

	Player player;
	ObstacleManager obstacleManager;

	sf::Clock clock;
	sf::Int32 currTime;

	bool hasDied = false;
};