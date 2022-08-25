#pragma once

#include <glad/gl.h> // include glad to get all the required OpenGL headers
#include "definitions.h"

#include <iostream>
#include <string>


enum class TextureType {
	RGB,
	RGBA,
	DEPTH,
	STENCIL
};

enum class ClampMethod {
	CLAMP_TO_EDGE,
	REPEAT
};

class Texture {

public:
	// use/activate the texture
	virtual void use(unsigned int _place) = 0;
};
