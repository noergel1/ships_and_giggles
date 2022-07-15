#pragma once

#include "definitions.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <regex>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include "texture_2d.h"

// a model loader for .obj files
// can't handle normal maps and height maps at this point
class ModelLoader {

public:
	// takes in the path to an .obj file
	// model needs to consist of a single mesh
	static bool loadModel(	std::string _modelPath, 
							std::vector<VertexData>* _vertices, 
							std::vector<unsigned int>* _indices, 
							std::vector<Texture*>* _textures, 
							glm::mat4 _preTransform					);

	bool loadModel_x3(std::string _modelPath, std::vector<VertexData>* _vertices, std::vector<unsigned int>* _indices, std::vector<Texture*>* _textures, glm::mat4 _preTransform);


private:

	// creates a simplified version of the .obj model
	// speeds up model loading
	// 
	// structure of simplified models:
	// vertex1, normal1, texCoord1, vertex2, normal2, texCoord3,.../diffuseMap1, diffuseMap2,.../specularMap1,...
	// 
	// it is assumed, that the indices are consecutive -> 1,2,3,4,5,6,...
	//
	static bool createModelSimplification(std::string _modelPath, std::vector<VertexData>* _vertices, std::vector<unsigned int>* _indices);

	// looks for simplified model created by createModelSimplification function
	// if it exists, loads it instead of .obj
	// simplified model has the same name as the .obj
	static bool checkForModelSimplification(std::string _modelPath, std::vector<VertexData>* _vertices, std::vector<unsigned int>* _indices);


};