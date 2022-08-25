#pragma once
#include "texture.h"

class Texture_2D : public Texture {

public:
	unsigned int getID();

	// constructor reads and builds the texture
	Texture_2D(const char* _filepath, bool _transparency);
	// constructor for framebuffers -> NULL as data
	Texture_2D(unsigned int _width, unsigned int _height, TextureType _textureType);

	// use/activate the texture
	void use(unsigned int _place);

protected:
	// the texture ID
	unsigned int m_ID;
};
