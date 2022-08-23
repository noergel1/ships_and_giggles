#pragma once

#include <vector>
#include "../utils/definitions.h"
#include "../utils/texture.h"

struct ModelData {

	unsigned int m_VAO;			// key value of the corresponding VAO in the Renderer class VAO map 
	std::string m_shader;		// key value of the corresponding shader in the Renderer class shader map 


	unsigned int m_indiceCount;

	std::vector<Texture*> m_textures;				// textures of the mesh
	float m_shininess;								// shininess value
};
