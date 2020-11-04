#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Pickup{
private:
	// voir pour refacto héritage
	const int HEALTH_START_VALUE = 50; // health item
	const int AMMO_START_VALUE = 12; // ammo item
	const int START_WAIT_TIME = 10;
	const int START_SECOND_TO_LIVE = 5;

	Sprite m_sprite;

	IntRect m_arena;
	int m_value;
	int m_type;

	bool m_spawned;
	float m_secondSinceSpawn,
		m_secondSinceDespawn,
		m_secondToLive,
		m_secondToWait;

public:
	Pickup(int type);

	void setArena(IntRect arena);
	void spawn();
	FloatRect getPosition() const;
	Sprite getSprite() const;
	bool isSpawned() const;
	int gotIt();
	void update(float dt);
	void upgrade();
};

