#include "texture_cubemap.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../utils/stb_image.h"

Texture_CubeMap::Texture_CubeMap(std::vector<const char*> _filepaths, bool _transparency)
{
	if (_filepaths.size() < 6)
	{
		assert("Cubemap contains fewer than 6 textures!");
	}

	stbi_set_flip_vertically_on_load(false);

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < _filepaths.size(); i++)
	{
		unsigned char* data = stbi_load(_filepaths[i], &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << _filepaths[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
}

// use/activate the texture
void Texture_CubeMap::use()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}