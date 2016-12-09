#pragma once
#include "bullet.h"
#include "KeyHandler.h"
#include <math.h>
#include <SFML\Graphics\Rect.hpp>

double const Bullet::DEG_TO_RAD = thor::Pi / 180.0f;

Bullet::Bullet(double rotation, sf::Vector2f position, sf::Texture const & texture)
{
	m_sprite.setTexture(texture);
	m_rotation = rotation;
	m_sprite.setPosition(position);
	m_position = position;
	m_alive = true;
	m_speed = 500;
	timeAlive = 5;
}

void Bullet::update(double dt)
{
	if (m_alive)
	{
		float xPosition = m_sprite.getPosition().x + cos(DEG_TO_RAD * m_rotation) * m_speed * (dt / 1000);
		float yPosition = m_sprite.getPosition().y + sin(DEG_TO_RAD * m_rotation) * m_speed * (dt / 1000);

		m_position = sf::Vector2f(xPosition, yPosition);
		m_sprite.setPosition(sf::Vector2f(m_position));

		sf::IntRect baseRect(8, 178, 8, 8);
		m_sprite.setTextureRect(baseRect);
		m_sprite.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);
		m_sprite.setRotation(m_rotation - 90);
	}
}

void Bullet::render(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}