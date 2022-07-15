#pragma once
#include "texture.h"

class Texture_2D : public Texture {

public:
	// defines if it's diffuse/specular/normal/.. map
	TextureType m_textureType;

	// constructor reads and builds the texture
	Texture_2D(const char* _filepath, bool _transparency, TextureType _textureType, unsigned int _place);

	// use/activate the texture
	void use();

protected:
	// the texture ID
	unsigned int m_ID;
	const unsigned int m_place;
};
