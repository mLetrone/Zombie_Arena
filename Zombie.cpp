#include "Zombie.hpp"
#include <cmath>


void Zombie::spawn(const float& startX, const float& startY) {
	m_position.x = startX;
	m_position.y = startY;

	m_sprite.setOrigin(25, 25);
	m_sprite.setPosition(m_position);
}
bool Zombie::hit() {
	m_health--;

	if (m_health == 0) {
		m_isAlive = false;
		m_sprite.setTexture(TextureHolder::getTexture("graphics/blood.png"));
		return true;
	}
	return false;
}

bool Zombie::isAlive() {
	return m_isAlive;
}

void Zombie::update(const float& dt, const Vector2f& playerLocation) {
	float dx = playerLocation.x - m_position.x;
	float dy = playerLocation.y - m_position.y;

	float dist = hypot(dx, dy);
	m_position.x += dt * m_speed * dx / dist;
	m_position.y += dt * m_speed * dy / dist;
	
	m_sprite.setPosition(m_position);

	float angle = (atan2(playerLocation.y - m_position.y, 
		playerLocation.x - m_position.x) * 180) / double (3.141);
	m_sprite.setRotation(angle);
}
