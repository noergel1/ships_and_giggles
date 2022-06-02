#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../utils/stb_image.h"



	Texture::Texture(const char* filepath, bool transparency, TextureType _textureType)
	{
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		if (data)
		{
			// check for file extension to decide if RGB or RGBA
			std::string filepathString = filepath;
			if (filepathString.substr(filepathString.find_last_of(".") + 1) == "jpg") {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else if (filepathString.substr(filepathString.find_last_of(".") + 1) == "png") {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

				if (transparency)
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
			printf("Failed to load texture %s", filepath);
		}

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_textureType = _textureType;
	}

	// use/activate the texture
	void Texture::use(int place)
	{
		glActiveTexture(GL_TEXTURE0 + place);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}


