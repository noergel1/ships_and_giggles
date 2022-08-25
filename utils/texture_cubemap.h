#pragma once
#include "texture.h"

class Texture_CubeMap : public Texture {

public:
	// constructor reads and builds the texture
	Texture_CubeMap(std::vector<const char*> _filepaths, bool _transparency);

	// use/activate the texture
	void use(unsigned int _place = 0);

protected:
	// the texture ID
	unsigned int m_ID;
};