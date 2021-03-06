#include "Tank.h"
#include <math.h>
#include "KeyHandler.h"
#include <iostream>
double const Tank::DEG_TO_RAD = thor::Pi / 180.0f;

Tank::Tank(sf::Texture const & texture, sf::Vector2f const & pos)
: m_texture(texture)
{
	initSprites(pos);
	m_speed = 0;
	m_rotation = 0;
	m_turretRotation = 0;
	m_timeSinceShoot = 0;
}

void Tank::update(double dt, KeyHandler passedKeyhandler)
{
	m_timeSinceShoot += dt / 1000;

	float xPosition = m_tankBase.getPosition().x + cos(DEG_TO_RAD * m_rotation) * m_speed * (dt / 1000);
	float yPosition = m_tankBase.getPosition().y + sin(DEG_TO_RAD * m_rotation) * m_speed * (dt / 1000);
	m_tankBase.setPosition(xPosition, yPosition);
	m_turret.setPosition(xPosition, yPosition);
	m_tankBase.setRotation(m_rotation);
	m_turret.setRotation(m_rotation + m_turretRotation);

	handleKeyInput(passedKeyhandler);

		//Iterator that takes the beginning position of the loop.
	for (auto bulletIteration = m_bulletList.begin(); bulletIteration != m_bulletList.end(); )
	{
		//m_tankBullets[i].update(dt);
		auto & bulletPtr = *bulletIteration;
		if (!bulletPtr->m_alive)
		{
			bulletIteration = m_bulletList.erase(bulletIteration);
		}
		else
		{
			std::cout << "Updating bullet" << std::endl;
			bulletPtr->update(dt);
			bulletIteration++;
		}
	}
}

void Tank::render(sf::RenderWindow & window) 
{
	window.draw(m_tankBase);
	window.draw(m_turret);
	for (auto bulletIteration = m_bulletList.begin(); bulletIteration != m_bulletList.end(); bulletIteration++)
	{
		auto &bulletPtr = *bulletIteration;
		bulletPtr->render(window);
	}
}

void Tank::initSprites(sf::Vector2f const & pos)
{
	// Initialise the tank base
	m_tankBase.setTexture(m_texture);
	sf::IntRect baseRect(2, 43, 79, 43);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);
	m_tankBase.setPosition(pos);

	// Initialise the turret
	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(19, 1, 83, 31);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);
	m_turret.setPosition(pos);
}

void Tank::handleKeyInput(KeyHandler keyhandler)
{
	
	if (keyhandler.isPressed(sf::Keyboard::Up))
	{
		increaseSpeed();
	}

	if (keyhandler.isPressed(sf::Keyboard::Down))
	{
		decreaseSpeed();
	}

	if (keyhandler.isPressed(sf::Keyboard::Right))
	{
		increaseRotation(m_rotation);
	}

	if (keyhandler.isPressed(sf::Keyboard::Left))
	{
		decreaseRotation(m_rotation);
	}

	if (keyhandler.isPressed(sf::Keyboard::Z))
	{
		decreaseRotation(m_turretRotation);
	}

	if (keyhandler.isPressed(sf::Keyboard::X))
	{
		increaseRotation(m_turretRotation);
	}
	if (keyhandler.isPressed(sf::Keyboard::Space) && m_timeSinceShoot > MIN_TIME_BETWEEN_SHOTS)
	{
		m_timeSinceShoot = 0;
		shoot();
		shotsFired++;
	}
}

double Tank::convertToRadians(double degrees)
{
	return degrees * DEG_TO_RAD;
}

void Tank::increaseSpeed()
{
	if (m_speed < 100.0)
	{
		m_speed += 5;
	}
}

void Tank::decreaseSpeed()
{
	if (m_speed > -100.0)
	{
		m_speed -= 5;
	}
}

void Tank::increaseRotation(double &part)
{
	part += 1;
	if (part == 360.0)
	{
		part = 0;
	}
}

void Tank::decreaseRotation(double &part)
{
	part -= 1;
	if (part == 0.0)
	{
		part = 359.0;
	}
}

sf::Sprite const & Tank::getTurretSprite() const
{
	return m_turret;
}

sf::Sprite const & Tank::getTankBaseSprite() const
{
	return m_tankBase;
}
void Tank::shoot()
{
	sf::Vector2f pos = sf::Vector2f(m_turret.getPosition().x + (cos(m_rotation *DEG_TO_RAD)), m_turret.getPosition().y + (sin(m_rotation * DEG_TO_RAD)));
	auto bulletPtr = std::make_unique<Bullet>(m_turret.getRotation(), pos, m_texture);
	m_bulletList.push_back(std::move(bulletPtr));
}

