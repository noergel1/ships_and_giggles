#pragma once

#include <glad/gl.h> // include glad to get all the required OpenGL headers
#include "definitions.h"

#include <iostream>
#include <string>


class Texture {

public:
	// the texture ID
	unsigned int m_ID;

	// defines if it's diffuse/specular/normal/.. map
	TextureType m_textureType;

	// constructor reads and builds the texture
	Texture(const char* filepath, bool transparency, TextureType _textureType);

	// use/activate the texture
	void use(int place);



};
