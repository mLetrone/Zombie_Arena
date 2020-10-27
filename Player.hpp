#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
using namespace sf;
#include "TextureHolder.hpp"
#include "Zombie.hpp"

class Player{
private:
	const float START_SPEED = 200;
	const int START_HEALTH = 100;

	Vector2f m_position;
	Sprite m_sprite;
	Texture m_texture;
	Vector2f m_resolution;
	IntRect m_arena;
	int m_tileSize;

	bool m_upPressed = false;
	bool m_downPressed = false;
	bool m_leftPressed = false;
	bool m_rightPressed = false;
	int m_health;
	int m_maxHealth;

	Time m_lastHit;
	
	float m_speed;

public:
	Player();
	Player(const Player& player);
	~Player();

	void spawn(IntRect arena, Vector2f resolution, int tileSize);
	void update(float elapsedTime, Vector2i mousePosition);
	void increaseHealthLevel(int amount);
	void upgradeSpeed();
	void upgradeHealth();
	void resetPlayerStats();

	bool hit(Time timehit, const std::shared_ptr<Zombie>& zomb);
	
	// Getter & setter
	Time getLastHitTime();
	FloatRect getPosition();
	Vector2f getCenter();
	Sprite getSprite();
	float getRotation();
	float getSpeed();
	int getHealth();
	

	void moveRight();
	void stopRight();
	void moveLeft();
	void stopLeft();
	void moveUp();
	void stopUp();
	void moveDown();
	void stopDown();


};

