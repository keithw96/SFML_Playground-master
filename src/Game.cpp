#include "Game.h"
#include "LevelLoader.h"
/// <summary>
/// @author RP
/// @date June 2016
/// @version 1.0
/// </summary>

// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;
sf::Vector2u viewport;
/// <summary> 
/// 
/// Create a new window and initialise member objects.
/// </summary>
/// 
Game::Game()
: m_window(sf::VideoMode(1440, 900, 32), "SFML Playground", sf::Style::Default)

{	
	int currentLevel = 1;

	if (!LevelLoader::load(currentLevel, m_level))
	{
		return;
	}

	if (!tankTexture.loadFromFile("./resources/images/SpriteSheet.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!backgroundTexture.loadFromFile("./resources/images/Background.jpg"))
	{
		std::string s("error loading texture");
		throw std::exception(s.c_str());
	}

	viewport = m_window.getSize();
	
	m_sprite.setTexture(tankTexture);
	m_backround.setTexture(backgroundTexture);

	
	m_backround.setScale(sf::Vector2f(viewport));

	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height / 2);
	m_sprite.setPosition(viewport.x / 2, viewport.y / 2);

	m_tank.reset(new Tank(tankTexture, m_level.m_tank.m_position));
}

/// <summary>
/// Main game entry point - runs until user quits.
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;

	while (m_window.isOpen())
	{
		sf::Time dt = clock.restart();

		lag += dt.asMilliseconds();

		processEvents();

		while (lag > MS_PER_UPDATE)
		{
			update(MS_PER_UPDATE);
			lag -= MS_PER_UPDATE;
		}
		update(MS_PER_UPDATE);

		render();
	}
}

/// <summary>
/// @brief Check for events
/// 
/// Allows window to function and exit. 
/// Events are passed on to the Game::processGameEvents() method
/// </summary>
void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
		processGameEvents(event);
	}
}

/// <summary>
/// @brief Handle all user input.
/// 
/// Detect and handle keyboard input.
/// </summary>
/// <param name="event">system event</param>
void Game::processGameEvents(sf::Event& event)
{
	// check if the event is a a mouse button release
	if (sf::Event::KeyPressed == event.type)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			m_window.close();
			break;
		case sf::Keyboard::Up:
			m_tank->increaseSpeed();
			break;
		case sf::Keyboard::Down:
			m_tank->decreaseSpeed();
			break;
		case sf::Keyboard::Left:
			m_tank->decreaseRotation(m_tank->m_rotation);
			break;
		case sf::Keyboard::Right:
			m_tank->increaseRotation(m_tank->m_rotation);
			break;
		case sf::Keyboard::X:
			m_tank->increaseRotation(m_tank->m_turretRotation);
			break;
		case sf::Keyboard::Z:
			m_tank->decreaseRotation(m_tank->m_turretRotation);
			break;
		default:
			break;
		}
	}
}

/// <summary>
/// Method to handle all game updates
/// </summary>
/// <param name="time">update delta time</param>
void Game::update(double dt)
{
	m_tank->update(1);
}

/// <summary>
/// @brief draw the window for the game.
/// 
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));
	m_window.draw(m_backround);
	m_tank->render(m_window);
	m_window.display();
}

