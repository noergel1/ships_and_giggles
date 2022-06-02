#pragma once

#include <unordered_map>
#include <map>
#include <numeric>

#include "shader.h"
#include "definitions.h"
#include "camera.h"
#include "dataprovider.h"
#include "simple_model_loader.h"

#include "../models/model_data.h"

class Renderer {

public:

	Renderer(Camera* _camera);
	//bool DrawStandardObject(ModelName _modelName, std::vector<Entity> _entities);
	bool Draw(std::map<std::string, std::vector<Entity>> _entities);


private:

	Camera* m_camera;

	// model data holds VAO and shader references, aswell as diffuse/specular maps and shininess
	std::unordered_map<std::string, ModelData*> m_models;			// map for models

	// maps hold all shader and VAO references
	std::unordered_map<ShaderReference, Shader*> m_shaders;		// map for shaders
	std::unordered_map<VaoReference, unsigned int> m_baseVaos;		// map for VAOs
	std::unordered_map<std::string, unsigned int> m_modelVaos;		// map for VAOs


	bool SetupShaders();
	bool SetShaderVariables(ShaderReference _shaderRef);

	bool SetupBaseVaos();
	unsigned int createVao(const std::vector<VertexData>& _vertices, const std::vector<unsigned int>& _indices);
	unsigned int createVao(const std::vector<VertexData>& _vertices);
	bool BindVao(unsigned int _vao);

	bool SetupModelData();

	// functions for creating the different model data
	//bool AddNewModel(std::string _modelName, VaoReference _vao, ShaderReference _shader, std::vector<VertexData> _vertices,
	//	std::vector<unsigned int> _indices, std::vector<Texture> _textures, float _shininess);
	bool AddNewModel(std::string _modelName, VaoReference _vao, ShaderReference _shader, std::vector<Texture> _textures, float _shininess);
	bool AddNewModel(std::string _modelName, std::vector<VertexData> _vertices, std::vector<unsigned int> _indices, ShaderReference _shader, std::vector<Texture> _textures, float _shininess);
	bool AddNewModel(std::string _modelName, std::vector<VertexData> _vertices, ShaderReference _shader, std::vector<Texture> _textures, float _shininess);
	bool AddCrateModel();
	bool AddShipModel();

	unsigned int getBaseModelIndiceSize(VaoReference _vaoReference);
};
