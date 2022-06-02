#pragma once

#include <vector>
#include "../utils/definitions.h"
#include "../utils/texture.h"

class ModelData {
public:
	ModelData(unsigned int _vao, unsigned int _indiceCount, ShaderReference _shader, std::vector<Texture> _textures, float _shininess);

	std::vector<Texture> getTextures();
	float getShininess();

	const unsigned int m_VAO;			// key value of the corresponding VAO in the Renderer class VAO map 
	const ShaderReference m_Shader;		// key value of the corresponding shader in the Renderer class shader map 

	const unsigned int m_indiceCount;


private:

	std::vector<Texture> m_textures;				// textures of the mesh
	float m_shininess;								// shininess value

};
