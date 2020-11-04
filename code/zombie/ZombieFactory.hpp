#pragma once

#include <map>

using namespace std;
#include "Zombie.hpp"
#include "Bloater.hpp"
#include "Chaser.hpp"
#include "Crawler.hpp"

class ZombieFactory{
public:
	
	typedef map<int, std::shared_ptr<Zombie>>  FactoryMap;

	FactoryMap m_factoryMap;
public:
	ZombieFactory();
	~ZombieFactory();
	void registerZombie(const int& key, std::shared_ptr<Zombie> zom);
	std::shared_ptr<Zombie> create(const int& zombKey);
};

