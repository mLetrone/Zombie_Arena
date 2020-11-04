#pragma once
#include<vector>
#include <SFML/Graphics.hpp>

using namespace sf;

class Zombie;

int CreateBackground(VertexArray& rVA, IntRect arena);
std::vector<std::shared_ptr<Zombie>> createHorde(int numZombies, IntRect arena);