#ifndef SHIPDATA_H
#define SHIPDATA_H

#include "model_data.h"
#include "../utils/simple_model_loader.h"
#include "../utils/dataprovider.h"

class ShipData : public ModelData {
public:

	ShipData();

	const VaoReference m_VAO = SHIP_VAO;			// key value of the corresponding VAO in the VAO map
	const ShaderReference m_Shader = SHIP_SHADER;	// key value of the corresponding shader in the shader map

private:

	std::vector<VertexData> m_Vertices;				// vertices of the mesh
	std::vector<unsigned int> m_Indices;			// indices of the mesh

	std::vector<Texture> m_diffuseMaps;				// diffuse maps
	std::vector<Texture> m_specularMaps;			// specular maps
	float m_shininess;								// shininess value
};


#endif