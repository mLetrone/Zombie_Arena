#include "Bloater.hpp"
#include "../utils/TextureHolder.hpp"

#define HEALTH 5
#define SPEED 40
#define DAMAGE 3

Bloater::Bloater() {
	m_speed = SPEED;
	m_health = HEALTH;
	m_sprite = Sprite(TextureHolder::getTexture("assets/graphics/bloater.png"));
	m_damage = 3;
}
