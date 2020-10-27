#pragma once
#include<vector>
#include <SFML/Graphics.hpp>

using namespace sf;

#include "Random.hpp"
#include "ZombieFactory.hpp"

int CreateBackground(VertexArray& rVA, IntRect arena);
std::vector<std::shared_ptr<Zombie>> createHorde(int numZombies, IntRect arena);