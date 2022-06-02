#ifndef ROCKDATA_H
#define ROCKDATA_H

#include <vector>
#include "../utils/definitions.h"
#include "../utils/texture.h"

class RockData {
public:
	const ModelName m_DataType = ROCK_MODEL;			// defines which model this is, used to cast into the respective type later
	const VaoReference m_VAO = ROCK_VAO;			// key value of the corresponding VAO in the VAO map
	const ShaderReference m_Shader = ROCK_SHADER;	// key value of the corresponding shader in the shader map

	const std::vector<VertexData> m_Vertices;		// vertices of the mesh
	const std::vector<unsigned int> m_Indices;		// indices of the mesh

	const std::vector<Texture> m_diffuseMaps;		// diffuse maps
	const std::vector<Texture> m_specularMaps;		// specular maps
	const float m_shininess;						// shininess value

};


#endif