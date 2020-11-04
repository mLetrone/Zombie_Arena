#include "Bullet.hpp"

Bullet::Bullet(){
	m_bulletShape.setSize(Vector2f(2, 2));
	m_speed = 1000;
	m_isShooted = false;
	m_damage = 1;
}

Bullet::Bullet(const float& damage, const float& speed, const float& height, const float& width){
	m_bulletShape.setSize(Vector2f(height, width));
	m_speed = speed;
	m_damage = damage;
	m_isShooted = false;
}

Bullet::Bullet(const Bullet& b):m_bulletShape(b.m_bulletShape), m_damage(b.m_damage),
m_position(b.m_position), m_speed(b.m_speed), m_range(b.m_range){
	m_isShooted = false;
}

void Bullet::stop(){
	m_isShooted = false;
}

bool Bullet::isShooted() const
{
	return m_isShooted;
}

void Bullet::shoot(const float& startX, const float& startY, const float& targetX, const float& targetY){
	m_isShooted = true;
	m_position.x = startX;
	m_position.y = startY;

	m_dx = targetX - startX;
	m_dy = targetY - startY;
	m_range = 1000;
	m_minX = startX - m_range;
	m_maxX = startX + m_range;
	m_minY = startY - m_range;
	m_maxY = startY + m_range;

	m_bulletShape.setPosition(m_position);
}

FloatRect Bullet::getPOsition() const
{
	return m_bulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape() const
{
	return m_bulletShape;
}

void Bullet::update(float dt)
{
	float dist = hypot(m_dx, m_dy);
	m_position.x += m_speed * dt * m_dx / dist;
	m_position.y += m_speed * dt * m_dy / dist;

	m_bulletShape.setPosition(m_position);

	if (m_position.x < m_minX || m_position.x > m_maxX ||
		m_position.y < m_minY || m_position.y > m_maxY)
	{
		m_isShooted = false;
	}
}
/*
+----------+            +----------------+		
|  Bullet  |----------<>|      Weapon    |		  +----------+
+----------+            | clip and spare |------<>|  Player  |
						+----------------+		  +----------+
							
*/