#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
using namespace sf;

class Zombie{
protected:
	Vector2f m_position;
	Sprite m_sprite;
	float m_health;
	float m_speed;
	bool m_isAlive;
	float m_damage;

public:
	bool hit();
	bool isAlive();
	void spawn(const float& startX, const float& startY);

	FloatRect getPosition() const { return m_sprite.getGlobalBounds(); }
	Sprite getSprite() const { return m_sprite; }
	float getDamage() const { return m_damage; }
	void update(const float& dt, const Vector2f& playerLocation);
	virtual std::shared_ptr<Zombie> clone() = 0;
};

