#pragma once
#include "Tank.h"
#include <SFML/Graphics.hpp>
#include "LevelLoader.h"


/// <summary>
/// @brief Main class for the SFML Playground project.
/// 
/// This will be a single class framework for learning about SFML.
/// </summary>

class Game
{
public:
	Game();	
	void run();
protected:	
	void update(double dt);
	void render();
	void processEvents();	
	void processGameEvents(sf::Event&);
	
	// main window

	LevelData m_level;
	

	sf::RenderWindow m_window;
	sf::Sprite m_sprite;
	sf::Sprite m_backround;
	sf::Texture tankTexture;
	sf::Texture backgroundTexture;
	sf::Event m_event;
	std::vector<sf::Sprite>  m_sprites;
	std::unique_ptr<Tank> m_tank;
};
