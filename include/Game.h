#pragma once
#include "Tank.h"
#include <SFML/Graphics.hpp>
#include "LevelLoader.h"
#include "KeyHandler.h"
#include "bullet.h"
/// <summary>
/// @brief Main class for the SFML Playground project.
/// 
/// This will be a single class framework for learning about SFML.
/// </summary>

/// <summary>
/// enum for gamestate transitions
/// </summary>
enum class
	Gamestate
{
	splash,
	game,
	endGame
};

class Game
{
public:
	Game();	
	void run();
protected:	
	Gamestate m_currentGamestate;
	
	void update(double dt);
	void render();

	void processEvents();	
	void processGameEvents(sf::Event&);

	void generateWalls();
	void generateTarget();

	void checkWindowBoundary();
	void checkTargetBulletCollision();

	bool checkTankWallCollision();

	void timerControl(double dt);

	bool spacePressed;

	// main window

	LevelData m_level;
	KeyHandler m_keyHandler;

	int score;
	float timer;

	double hitCount;
	double accuracy;

	sf::Text timeLeft;
	sf::Text splashText;
	sf::Text scoreText;
	sf::Text endGameText;
	sf::Text totalScore;
	sf::Text accuracyPercentage;
	sf::Font m_arial;

	sf::RenderWindow m_window;
	sf::Sprite m_sprite;
	sf::Sprite m_backround;

	sf::Texture m_targetTexture;
	sf::Texture tankTexture;
	sf::Texture backgroundTexture;
	sf::Event m_event;

	sf::Vector2f position;

	std::vector<sf::Sprite>  m_sprites;

	std::unique_ptr<Tank> m_tank;
	

	std::vector<std::unique_ptr<sf::Sprite>> m_wallSprites;
	sf::Sprite m_targetSprite;
};
