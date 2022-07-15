#pragma once

#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Renderbuffer {
public:
	Renderbuffer(int _width, int _height);

	void clear();

	void bind();
	void unbind();

	void deleteBuffer();

private:
	unsigned int m_renderbufferID;
};