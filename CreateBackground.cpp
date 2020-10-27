#include "Zombie_Arena.hpp"
#include <iostream>

#define TILE_SIZE 50
#define TILE_TYPES 3
#define VERTEX_IN_QUAD 4

int CreateBackground(VertexArray& rVA, IntRect arena) {

	int worldWidth = arena.width / TILE_SIZE;
	int worldHeight = arena.height / TILE_SIZE;

	// set the primitive type of, we are using
	rVA.setPrimitiveType(Quads);
	// set size of the vertex array
	rVA.resize(worldWidth * worldHeight * VERTEX_IN_QUAD);

	int currentVertex = 0;

	for (int w = 0; w < worldWidth; w++)
	{
		for (int h = 0; h < worldHeight; h++)
		{
			rVA[currentVertex + 0].position =
				Vector2f(w * TILE_SIZE, h * TILE_SIZE);
			rVA[currentVertex + 1].position =
				Vector2f((w * TILE_SIZE) + TILE_SIZE, h * TILE_SIZE);
			rVA[currentVertex + 2].position =
				Vector2f((w * TILE_SIZE) + TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);
			rVA[currentVertex + 3].position =
				Vector2f(w * TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);

			// define texture position
			// grass, stone, bush or wall
			if (h == 0 || h == worldHeight - 1 ||
				w == 0 || w == worldWidth - 1) {
				rVA[currentVertex + 0].texCoords =
					Vector2f(0, 0 + TILE_SIZE * TILE_TYPES);
				rVA[currentVertex + 1].texCoords =
					Vector2f(TILE_SIZE, 0 + TILE_SIZE * TILE_TYPES);
				rVA[currentVertex + 2].texCoords =
					Vector2f(TILE_SIZE, TILE_SIZE + TILE_SIZE * TILE_TYPES);
				rVA[currentVertex + 3].texCoords =
					Vector2f(0, TILE_SIZE + TILE_SIZE * TILE_TYPES);
			}
			else {
				// Use a random texture
				int mOrG = nb_randint(0, TILE_TYPES);
				int verticalOffset = mOrG * TILE_SIZE;

				rVA[currentVertex + 0].texCoords =
					Vector2f(0, 0 + verticalOffset);
				rVA[currentVertex + 1].texCoords =
					Vector2f(TILE_SIZE, 0 + verticalOffset);
				rVA[currentVertex + 2].texCoords =
					Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset);
				rVA[currentVertex + 3].texCoords =
					Vector2f(0, TILE_SIZE + verticalOffset);

			}

			currentVertex += VERTEX_IN_QUAD;
		}
	}

	return TILE_SIZE;
}

std::vector<std::shared_ptr<Zombie>> createHorde(int numZombies, IntRect arena) {
	std::vector<std::shared_ptr<Zombie>> zombies;
	zombies.resize(numZombies);
	ZombieFactory factory;
	int maxY = arena.height - 20;
	int minY = arena.top + 20;
	int maxX = arena.width - 20;
	int minX = arena.left - 20;
	for (int i = 0; i < numZombies; i++) {
		//Which side the zombie will spawn
		int side = nb_randint(0, 3);
		float x, y;
		switch (side)
		{
		case 0: //left
			x = minX;
			y = float(nb_randint(minY, maxY));
			break;
		case 1: //Right
			x = maxX;
			y = float(nb_randint(minY, maxY));
			break;
		case 2: //top
			x = float(nb_randint(minX, maxX));
			y = minY;
			break;
		case 3: //bottom
			x = float(nb_randint(minX, maxX));
			y = maxY;
		}
		std::cout << "ERROR createHord " << side << " " << i << std::endl;
		zombies[i] = factory.create(nb_randint(0,2));
		zombies[i]->spawn(x, y);
	}
	return zombies;
}