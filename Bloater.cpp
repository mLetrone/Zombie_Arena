#include "Bloater.hpp"

#define HEALTH 5
#define SPEED 40

Bloater::Bloater() {
	m_speed = SPEED;
	m_health = HEALTH;
	m_sprite = Sprite(TextureHolder::getTexture("graphics/bloater.png"));
}
