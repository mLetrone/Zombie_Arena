#include "Pickup.hpp"
#include <string>
#include "../utils/TextureHolder.hpp"
#include "../utils/Random.hpp"

Pickup::Pickup(int type){
	m_type = type;
	std::string texture = (m_type == 1) ? "assets/graphics/health_pickup.png" :
		"assets/graphics/ammo_pickup.png";
	m_value = (m_type == 1) ? HEALTH_START_VALUE : AMMO_START_VALUE;
	m_sprite = Sprite(TextureHolder::getTexture(texture));

	m_sprite.setOrigin(25, 25);
	m_secondToLive = START_SECOND_TO_LIVE;
	m_secondToWait = START_WAIT_TIME;
}

void Pickup::setArena(IntRect arena){
	m_arena.left = arena.left +75;
	m_arena.height = arena.height -75;
	m_arena.top = arena.top +75;
	m_arena.width = arena.width -75;

	spawn();
}

void Pickup::spawn(){
	int x = nb_randint(0, m_arena.width),
		y = nb_randint(0, m_arena.height);

	m_spawned = true;
	m_secondSinceSpawn = 0;
	m_sprite.setPosition(x, y);
}

FloatRect Pickup::getPosition() const{
	return m_sprite.getGlobalBounds();
}

Sprite Pickup::getSprite() const{
	return m_sprite;
}

bool Pickup::isSpawned() const{
	return m_spawned;
}

int Pickup::gotIt(){
	m_spawned = false;
	m_secondSinceDespawn = 0;
	return m_value;
}

void Pickup::update(float dt){
	if (m_spawned)
		m_secondSinceSpawn += dt;
	else
		m_secondSinceDespawn += dt;
	
	if (m_secondSinceSpawn > m_secondToLive&& m_spawned) {
		m_spawned = false;
		m_secondSinceDespawn = 0;
	}
	else if (m_secondSinceDespawn > m_secondToWait && !m_spawned) {
		spawn();
	}
}

void Pickup::upgrade(){
	m_value += (m_type == 1) ? HEALTH_START_VALUE * .5 : AMMO_START_VALUE * .5;
	m_secondToLive += START_SECOND_TO_LIVE / 10;
	m_secondToWait -= START_WAIT_TIME / 10;
}
