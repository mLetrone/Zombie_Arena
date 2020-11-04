#pragma once
#include "Zombie.hpp"

class Bloater : public Zombie{
public:
	 Bloater();

	 std::shared_ptr<Zombie> clone()  override {
		 return std::make_shared<Bloater>(*this);
	 }
	 // unique ability
};

