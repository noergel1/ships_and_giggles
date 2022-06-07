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
	bool shutdownRenderer();

	bool Draw(std::map<ModelName, std::vector<Entity>> _entities);

	bool AddNewModel(ModelName _modelName, const unsigned int _vao, const unsigned int _indiceCount, const ShaderReference _shader, const std::vector<Texture> _textures, const float _shininess);

private:

	Camera* m_camera;

	// model data holds VAO and shader references, aswell as diffuse/specular maps and shininess
	std::map<ModelName, const ModelData*> m_models;

	// maps hold all shader and VAO references
	std::unordered_map<ShaderReference, const unsigned int> m_shaders;		// map for shaders


	bool SetupShaders();
	bool SetShaderVariables(ShaderReference _shaderRef);

	bool BindVao(unsigned int _vao);

};
