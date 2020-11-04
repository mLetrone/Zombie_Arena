#include "ZombieFactory.hpp"

void ZombieFactory::registerZombie(const int& key, std::shared_ptr<Zombie> zomb) {
	if (m_factoryMap.find(key) == m_factoryMap.end())
		m_factoryMap[key] = zomb;
}

ZombieFactory::ZombieFactory() {
	ZombieFactory::registerZombie(0, std::make_shared<Chaser>());
	ZombieFactory::registerZombie(1, std::make_shared<Crawler>());
	ZombieFactory::registerZombie(2, std::make_shared<Bloater>());
}
std::shared_ptr<Zombie> ZombieFactory::create(const int& key) {
	FactoryMap::const_iterator it = m_factoryMap.find(key);
	if (it != m_factoryMap.end()) {
		return (it->second)->clone();
	}
	return nullptr;
}

ZombieFactory::~ZombieFactory() {
	m_factoryMap.clear();
}