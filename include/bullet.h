#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Math.hpp>
#include "KeyHandler.h"
class Bullet
{
	double static const DEG_TO_RAD;
public:
	Bullet(double rotation, sf::Vector2f position, sf::Texture const & texture);

	double m_speed;
	double m_rotation;
	double timeAlive;
	sf::Vector2f m_position;
	sf::Sprite m_sprite;
	bool m_alive;

	void render(sf::RenderWindow & window);
	void update(double dt);
private:
};