#pragma once

#include <unordered_map>
#include <map>
#include <numeric>

#include "shader.h"
#include "definitions.h"
#include "camera.h"
#include "dataprovider.h"
#include "simple_model_loader.h"
#include "helpers.h"

#include "../models/model_data.h"

class Renderer {

public:

	Renderer(Camera* _camera, GameSettings _settings);
	bool shutdownRenderer();

	bool Draw(std::map<ModelName, std::vector<Entity>> _entities);

	bool AddNewModel(ModelName _modelName, const unsigned int _vao, const unsigned int _indiceCount, const ShaderReference _shader, const std::vector<Texture*> _textures, const float _shininess);

private:

	Camera* m_camera;
	GameSettings m_settings;

	// model data holds VAO and shader references, aswell as diffuse/specular maps and shininess
	std::map<ModelName, const ModelData*> m_models;

	// maps hold all shader and VAO references
	std::unordered_map<ShaderReference, const unsigned int> m_shaders;		// map for shaders


	bool SetupShaders();

	bool BindVao(unsigned int _vao);

	// pre & post render functions
	void preRender(ModelName _modelname, const  ModelData* _modelData, glm::mat4* _model, glm::mat4* _projection, glm::mat4* _view, const unsigned int& _shader);
	void render(Entity _entity, ModelName _modelname, const  ModelData* _modelData, glm::mat4* _model, glm::mat4* _projection, glm::mat4* _view, const unsigned int& _shader);
	void postRender(ModelName _modelname, const  ModelData* _modelData, glm::mat4* _model, glm::mat4* _projection, glm::mat4* _view, const unsigned int& _shader);
};
