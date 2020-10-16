#include "Chaser.hpp"

#define HEALTH 1
#define SPEED 80

Chaser::Chaser() {
	m_speed = SPEED;
	m_health = HEALTH;
	m_sprite = Sprite(TextureHolder::getTexture("graphics/chaser.png"));
}
