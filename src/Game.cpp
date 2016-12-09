#include "Game.h"
#include "LevelLoader.h"
#include "CollisionDetector.h"
/// <summary>
/// @author RP
/// @date June 2016
/// @version 1.0
/// </summary>

// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;
sf::Vector2u viewport;

/// <summary> 
/// Create a new window and initialise member objects.
/// </summary>
Game::Game()
	: m_window(sf::VideoMode(1440, 900, 32), "SFML Playground", sf::Style::Default),
	m_currentGamestate(Gamestate::splash)
{	
	m_window.setVerticalSyncEnabled(true);

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

	if (!m_targetTexture.loadFromFile("./resources/images/targetSprite.jpeg"))
	{
		std::cout << "problem loading file" << std::endl;
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	if (!m_arial.loadFromFile("./resources/fonts/verdana.ttf"))
	{
		std::cout << "problem loading font file" << std::endl;
	}

	/// <summary>
	/// define text
	/// </summary>
	scoreText.setFont(m_arial);
	scoreText.setColor(sf::Color::Red);
	scoreText.setCharacterSize(20);
	scoreText.setPosition(1300, 30);

	splashText.setFont(m_arial);
	splashText.setColor(sf::Color::Red);
	splashText.setCharacterSize(50);
	splashText.setPosition(500, 450);
	splashText.setString("Press Space To Continue");

	endGameText.setFont(m_arial);
	endGameText.setColor(sf::Color::Red);
	endGameText.setCharacterSize(50);
	endGameText.setPosition(500, 450);
	endGameText.setString("your score is: " + std::to_string(score) + "\n press space to exit");

	timeLeft.setFont(m_arial);
	timeLeft.setCharacterSize(20);
	timeLeft.setPosition(sf::Vector2f(20, 30));
	timeLeft.setColor(sf::Color::Red);

	totalScore.setFont(m_arial);
	totalScore.setCharacterSize(20);
	totalScore.setPosition(sf::Vector2f(20, 30));
	totalScore.setColor(sf::Color::Red);

	accuracyPercentage.setFont(m_arial);
	accuracyPercentage.setCharacterSize(20);
	accuracyPercentage.setPosition(sf::Vector2f(20, 500));
	accuracyPercentage.setColor(sf::Color::Red);

	viewport = m_window.getSize();

	timer = 60.0f;
	score = 0;
	spacePressed = false;

	m_sprite.setTexture(tankTexture);
	m_backround.setTexture(backgroundTexture);

	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height / 2);

	generateWalls();

	generateTarget();

	m_backround.setScale(sf::Vector2f(viewport));

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
		else if (m_keyHandler.isPressed(sf::Keyboard::Space))
		{
			spacePressed = true;
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
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		m_keyHandler.updateKey(event.key.code, true);
		break;
	case sf::Event::KeyReleased:
		m_keyHandler.updateKey(event.key.code, false);
		break;
	}
}
/// <summary>
/// Method to handle all game updates
/// enum to control gamestate
/// </summary>
/// <param name="time">update delta time</param>
void Game::update(double dt)
{
	switch (m_currentGamestate)
	{
	case::Gamestate::splash:
		if (spacePressed == true)
		{
			m_currentGamestate = Gamestate::game;
		}
		break;
	case::Gamestate::game:
		m_tank->update(dt, m_keyHandler);

		if (checkTankWallCollision())
		{
			m_tank->m_speed = -1;
		}

		checkTargetBulletCollision();

		checkWindowBoundary();

		if (timer <= 0)
		{
			m_currentGamestate = Gamestate::endGame;
		}
		
		timeLeft.setString("Time Left: " + std::to_string((int)timer));
		scoreText.setString("score: " + std::to_string(score));

		timerControl(dt);

		break;

	case::Gamestate::endGame:
		if (m_keyHandler.isPressed(sf::Keyboard::Space))
		{
			m_window.close();
		}
		break;

	default:
		break;
	}
}

/// <summary>
/// @brief draw the window for the game.
/// enum to control gamestates
/// </summary>
void Game::render()
{
	switch (m_currentGamestate)
	{
	case::Gamestate::splash:
		m_window.clear(sf::Color(0, 0, 0, 0));
		m_window.draw(m_backround);
		m_window.draw(splashText);

		m_window.display();
		break;
	case::Gamestate::game:
		m_window.clear(sf::Color(0, 0, 0, 0));
		m_window.draw(m_backround);
		m_tank->render(m_window);

		//draws each sprite in m_wallSprites
		for (auto &c : m_wallSprites)
		{
			m_window.draw(*c);
		}

		//draws targets
		m_window.draw(m_targetSprite);

		m_window.draw(timeLeft);
		m_window.draw(scoreText);

		m_window.display();
		break;
	case::Gamestate::endGame:
		m_window.clear(sf::Color(0, 0, 0, 0));

		if (!hitCount == 0 && !m_tank->shotsFired == 0 )
		{
			accuracy = ((hitCount / m_tank->shotsFired) * 100);
		}
		else
		{
			accuracy = 0;
		}
		accuracyPercentage.setString("Accuracy: " +  std::to_string((int)accuracy));

		totalScore.setString("Total Score: " + std::to_string((score + accuracy)));

		m_window.draw(m_backround);
		m_window.draw(endGameText);
		m_window.draw(accuracyPercentage);
		m_window.draw(totalScore);
		m_window.draw(accuracyPercentage);

		m_window.display();
		break;
	default:
		break;
	}
}

/// <summary>
/// creates a new sprite each pass through the loop
/// sets the values for each of variables in m_obstacles
/// then pushes the sprite into the array of sprites: m_wallSprites
/// </summary>
void Game::generateWalls()
{
	sf::IntRect wallRect(2, 129, 33, 23);
	// Create the Walls 
	for (ObstacleData const &obstacle : m_level.m_obstacles)
	{
		std::unique_ptr<sf::Sprite> sprite(new sf::Sprite());
		sprite->setTexture(tankTexture);
		sprite->setTextureRect(wallRect);
		sprite->setOrigin(wallRect.width / 2.0, wallRect.height / 2.0);
		sprite->setPosition(obstacle.m_position);
		sprite->setRotation(obstacle.m_rotation);
		m_wallSprites.push_back(std::move(sprite));
	}
}

/// <summary>
/// creates a new sprite each pass through the loop
/// stes the sprites values
/// and then pushes the sprite into the sprite array
/// </summary>
void Game::generateTarget()
{
	sf::IntRect targetRect(10, 10, 60, 60);

	int offset = rand() % 900  + 1;
 
	for (Targetdata &target : m_level.m_target)
	{
		m_targetSprite.setPosition(target.m_position);
		m_targetSprite.setTexture(m_targetTexture);
		m_targetSprite.setTextureRect(targetRect);
		m_targetSprite.setOrigin(targetRect.width / 2.0, targetRect.height / 2.0);
		m_targetSprite.setPosition(target.m_position.x + offset, target.m_position.y + offset);
		m_targetSprite.setRotation(target.m_rotation);
	}
}

/// <summary>
/// bool method that checks for collision between the turret sprite and each of the obstacles
/// returns true if collision is dectected
/// </summary>
/// <returns></returns>
bool Game::checkTankWallCollision()
{
	/// <summary>
	/// checks for collision between the bullet and the obstacles
	/// sets the relevant bullet's alive value to false if collision is dectected
	/// </summary>
	/// <returns></returns>
	for (std::unique_ptr<sf::Sprite> const & sprite : m_wallSprites)
	{
		for (auto bulletIteration = m_tank->m_bulletList.begin(); bulletIteration != m_tank->m_bulletList.end(); bulletIteration++)
		{
			auto &bulletPtr = *bulletIteration;
			if (CollisionDetector::collision(bulletPtr->m_sprite, *sprite))
			{
				if (score < 0)
				{
					score -= 50;
				}
				bulletPtr->m_alive = false;
			}
		}

		if (CollisionDetector::collision(m_tank->getTurretSprite(), *sprite))
		{
			return true;
		}

		if (CollisionDetector::collision(m_tank->getTankBaseSprite(), *sprite))
		{
			return true;
		}
	}
	return false;
}

/// <summary>
/// checks for collision between targets and bullets
/// kills both is collision is dectected
/// </summary>
void Game::checkTargetBulletCollision()
{
		for (auto bulletIteration = m_tank->m_bulletList.begin(); bulletIteration != m_tank->m_bulletList.end(); bulletIteration++)
		{
			auto &bulletPtr = *bulletIteration;
			if (CollisionDetector::collision(bulletPtr->m_sprite, m_targetSprite))
			{
				bulletPtr->m_alive = false;
				hitCount++;
				score = score + 100;

				generateTarget();
			}
		}
	}

/// <summary>
/// checks if th bullet's position is outside the window
/// kills the bullet if the x or y of the bullet is outside of the window
/// </summary>
void Game::checkWindowBoundary()
{
	for (auto bulletIteration = m_tank->m_bulletList.begin(); bulletIteration != m_tank->m_bulletList.end(); bulletIteration++)
	{
		auto &bulletPtr = *bulletIteration;
		if (bulletPtr->m_position.x < 0 || bulletPtr->m_position.x > viewport.x || bulletPtr->m_position.y < 0 || bulletPtr->m_position.y > viewport.y)
		{
			bulletPtr->m_alive = false;
			if (score < 0)
			{
				score -= 50;
			}
		}
	}
}

/// <summary>
/// updates the timer
/// </summary>
/// <param name="dt"></param>
void Game::timerControl(double dt)
{
	timer -= dt / 1200;
}


