#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "../utils/texture_2d.h"

class Framebuffer {
public:
	Framebuffer(int _width, int _height);

	Texture_2D m_texture;

	void clear();

	void bind();
	void unbind();

	void bindTexture();
	void unbindTexture();

	void deleteBuffer();

private:
	unsigned int m_framebufferID;
};