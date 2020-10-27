#pragma once
#include "Zombie.hpp"

class Chaser : public Zombie {
public:
	Chaser();

	std::shared_ptr<Zombie> clone()  override {
		return std::make_shared<Chaser>(*this);
	}
	// unique ability
};

