#include "Crawler.hpp"
#define HEALTH 3
#define SPEED 20
#define DAMAGE 1

Crawler::Crawler() {
	m_speed = SPEED;
	m_health = HEALTH;
	m_sprite = Sprite(TextureHolder::getTexture("graphics/crawler.png"));
	m_damage = DAMAGE;
}