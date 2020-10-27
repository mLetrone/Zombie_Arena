#pragma once
#include "Zombie.hpp"

class Crawler : public Zombie{
public:
	Crawler();

	std::shared_ptr<Zombie> clone()  override {
		return std::make_shared<Crawler>(*this);
	}
	//unique ability
};

