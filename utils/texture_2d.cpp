#include "texture_2d.h"

#include "../utils/stb_image.h"

Texture_2D::Texture_2D(const char* _filepath, bool _transparency, TextureType _textureType, unsigned int _place)
	:m_place(_place)
	,m_textureType(_textureType)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(_filepath, &width, &height, &nrChannels, 0);

	if (data)
	{
		// check for file extension to decide if RGB or RGBA
		std::string filepathString = _filepath;
		if (filepathString.substr(filepathString.find_last_of(".") + 1) == "jpg") {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else if (filepathString.substr(filepathString.find_last_of(".") + 1) == "png") {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			if (_transparency)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}

			glGenerateMipmap(GL_TEXTURE_2D);
		}

	}
	else
	{
		printf("Failed to load texture %s", _filepath);
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture_2D::Texture_2D(unsigned int _width, unsigned int _height, TextureType _textureType, unsigned int _place)
	:m_place(_place)
	,m_textureType(_textureType)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	// NULL as data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);
}

// use/activate the texture
void Texture_2D::use()
{
	glActiveTexture(GL_TEXTURE0 + m_place);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

unsigned int Texture_2D::getID()
{
	return m_ID;
}
