#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Math.hpp>
#include "KeyHandler.h"
#include "bullet.h"
#include <memory>
/// <summary>
/// @brief A simple tank controller.
/// 
/// This class will manage all tank movement and rotations.
/// </summary>
class Tank
{
	double static const DEG_TO_RAD;
public:	
	std::vector<std::unique_ptr<Bullet>> m_bulletList;
	int m_maxBullets;
	double m_speed;
	double m_rotation;
	double m_turretRotation;
	double convertToRadians(double degrees);

	Tank(sf::Texture const & texture, sf::Vector2f const & pos);

	void update(double dt, KeyHandler m_keyhandler);
	void render(sf::RenderWindow & window);
	void increaseSpeed();
	void decreaseSpeed();
	void handleKeyInput(KeyHandler m_keyhandler);
	void increaseRotation(double &part);
	void decreaseRotation(double &part);

	double shotsFired;
	

	sf::Sprite const & getTurretSprite() const;
	sf::Sprite const & getTankBaseSprite() const;

private:
	void initSprites(sf::Vector2f const & pos);
	sf::Sprite m_tankBase;
	sf::Sprite m_turret;
	sf::Texture const & m_texture;
	float m_timeSinceShoot;
	const float MIN_TIME_BETWEEN_SHOTS = 1;
	void shoot();
};
