#pragma once

/**
	Create a class TextureHolder with the singleton pattern
**/
#include <SFML/Graphics.hpp>
using namespace sf;

#include <map>
#include <memory>
using namespace std;


// TODO: refacto the singleton pattern

class TextureHolder{
private:
	map<string, Texture> m_textures;
	static unique_ptr<TextureHolder> m_s_instance;

public:
	TextureHolder();

	static Texture& getTexture(string const& filename);
	// setter for the map ?
};

