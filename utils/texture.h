#pragma once

#include <glad/gl.h> // include glad to get all the required OpenGL headers
#include "definitions.h"

#include <iostream>
#include <string>


enum class TextureType {
	DIFFUSE,
	SPECULAR,
	NORMAL,
	CUBEMAP_FACE
};

class Texture {

public:
	// use/activate the texture
	virtual void use() = 0;
};
