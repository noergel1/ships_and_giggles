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

	const unsigned int m_width;
	const unsigned int m_height;

	void clear();

	void bind();
	void unbind();


	void deleteBuffer();

	void createColorTextureAttachement( unsigned int _slot );
	void createDepthTextureAttachement();
	void createStencilTextureAttachement();
	void createDepthStencilTextureAttachement();
	void createDepthRenderbufferAttachement();
	void createDepthStencilRenderbufferAttachement();

	void checkFramebufferComplete();

private:
	unsigned int m_framebufferID;
};