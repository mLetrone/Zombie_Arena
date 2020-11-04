#include <assert.h>

#include "TextureHolder.hpp"

unique_ptr<TextureHolder> TextureHolder::m_s_instance = nullptr;

TextureHolder::TextureHolder() {
	assert(m_s_instance == nullptr);
	m_s_instance = make_unique<TextureHolder>(*this);
}


Texture& TextureHolder::getTexture(string const& filename){
	// Get a reference to m_Textures using m_s_Instance
	auto& m = m_s_instance->m_textures;
	// auto is the equivalent of map<string, Texture>
	// Create an iterator to hold a key-value-pair (kvp)
	// and search for the required kvp
	// using the passed in file name
	auto keyValuePair = m.find(filename);
	// auto is equivalent of map<string, Texture>::iterator
	// Did we find a match?
	if (keyValuePair != m.end())
	{
		// Yes
		// Return the texture,
		// the second part of the kvp, the texture
		return keyValuePair->second;
	}
	else
	{
		// File name not found
		// Create a new key value pair using the filename
		auto& texture = m[filename];
		// Load the texture from file in the usual way
		texture.loadFromFile(filename);
		// Return the texture to the calling code
		return texture;
	}
}


