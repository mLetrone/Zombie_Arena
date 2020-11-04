#pragma once

#include <SFML/Graphics.hpp>
using namespace sf;
class Bullet{
private:
	Vector2f m_position;
	float m_damage;
	RectangleShape m_bulletShape;
	
	bool m_isShooted;
	float m_speed;

	float m_maxX,
		m_maxY,
		m_minX,
		m_minY;

	float m_range;
	float m_dx,
		m_dy;

public:
	Bullet();
	Bullet(const float& damage, const float& speed, const float& height=2., const float& width=2.);
	Bullet(const Bullet& b);

	void stop();
	bool isShooted()const;
	void shoot(const float& startX, const float& startY, const float& targetX, const float& targetY);
	FloatRect getPOsition()const;
	RectangleShape getShape()const;
	void update(float dt);


};

