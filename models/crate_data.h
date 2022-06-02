//#ifndef CRATEDATA_H
//#define CRATEDATA_H
//
//<<<<<<< Updated upstream
//#include <vector>
//#include "../utils/definitions.h"
//#include "../utils/texture.h"
//=======
//#include "model_data.h"
//#include "../utils/dataprovider.h"
//>>>>>>> Stashed changes
//
//class CrateData : public ModelData {
//public:
//
//	CrateData(std::vector<Texture> _diffuseMaps, std::vector<Texture> _specularMaps, float _shininess)
//		: m_diffuseMaps(_diffuseMaps)
//		, m_specularMaps(_specularMaps)
//		, m_Vertices(cubeVertices)
//		, m_Indices(cubeIndices)
//		, m_shininess(_shininess)
//	{
//	};
//
//	const VaoReference m_VAO = CUBE_VAO;			// key value of the corresponding VAO in the VAO map
//	const ShaderReference m_Shader = CRATE_SHADER;	// key value of the corresponding shader in the shader map
//
//<<<<<<< Updated upstream
//	const std::vector<VertexData> m_Vertices;		// vertices of the mesh
//	const std::vector<unsigned int> m_Indices;		// indices of the mesh
//
//	std::vector<Texture> m_diffuseMaps;		// diffuse maps
//	std::vector<Texture> m_specularMaps;		// specular maps
//	float m_shininess;						// shininess value
//=======
//
//private:
//	std::vector<VertexData> m_Vertices;				// vertices of the mesh
//	std::vector<unsigned int> m_Indices;			// indices of the mesh
//
//	std::vector<Texture> m_diffuseMaps;				// diffuse maps
//	std::vector<Texture> m_specularMaps;			// specular maps
//	float m_shininess;								// shininess value
//>>>>>>> Stashed changes
//};
//
//
//
//#endif