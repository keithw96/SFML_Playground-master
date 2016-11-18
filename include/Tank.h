#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Math.hpp>
/// <summary>
/// @brief A simple tank controller.
/// 
/// This class will manage all tank movement and rotations.
/// </summary>
class Tank
{
	
	double static const DEG_TO_RAD;
public:	
	double m_speed;
	double m_rotation;
	double m_turretRotation;
	Tank(sf::Texture const & texture, sf::Vector2f const & pos);
	void update(double dt);
	void render(sf::RenderWindow & window);
	void increaseSpeed();
	void decreaseSpeed();
	void increaseRotation(double &part);
	void decreaseRotation(double &part);
	double convertToRadians(double degrees);
private:
	void initSprites(sf::Vector2f const & pos);
	sf::Sprite m_tankBase;
	sf::Sprite m_turret;
	sf::Texture const & m_texture;	
};
