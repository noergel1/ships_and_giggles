#pragma once

#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Framebuffer {
public:
	Framebuffer(int _width, int _height);

	void clear();

	void bind();
	void unbind();

	void bindTexture(int _textureSlot);
	void unbindTexture();

	void deleteBuffer();

private:
	unsigned int m_framebufferID;
	unsigned int m_textureID;
};