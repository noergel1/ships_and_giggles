#pragma once

#include <unordered_map>
#include <map>
#include <numeric>
#include <boost/filesystem/directory.hpp>
#include <boost/regex.hpp>

#include "shader.h"
#include "definitions.h"
#include "camera.h"
#include "dataprovider.h"
#include "simple_model_loader.h"
#include "helpers.h"
#include "framebuffer.h"
#include "renderbuffer.h"

#include "../models/model_data.h"

const unsigned int REFLECTION_WIDTH = 320;
const unsigned int REFLECTION_HEIGHT = 180;
const unsigned int REFRACTION_WIDTH = 1280;
const unsigned int REFRACTION_HEIGHT = 720;

struct RenderVariables {
	Framebuffer framebuffer_postprocessing;
	Framebuffer framebuffer_waterReflection;
	Framebuffer framebuffer_waterRefraction;
};

class Renderer {

public:

	Renderer(Camera* _camera, GameSettings _settings);
	bool shutdownRenderer();

	bool Draw(std::map<ModelName, std::vector<Entity>> _entities);

	bool AddNewModel( ModelName _modelName, const unsigned int _vao, const unsigned int _indiceCount, const std::string _shader, const std::vector<Texture*> _textures, const float _shininess );
	bool AddNewModel( ModelName _modelName, ModelData* _modelData );

private:

	Camera* m_camera;
	GameSettings m_settings;

	// model data holds VAO and shader references, aswell as diffuse/specular maps and shininess
	std::map<ModelName, const ModelData*> m_models;

	// maps hold all shader and VAO references
	std::map<const std::string, const unsigned int> m_shaders;		// map for shaders


	bool SetupShaders();
	bool SetUniforms();
	bool SetRenderVariables();

	bool BindVao(unsigned int _vao);

	// render functions
	RenderVariables m_renderVariables;

	void preRenderScene(std::map<ModelName, std::vector<Entity>> _entities);
	void renderScene(std::map<ModelName, std::vector<Entity>> _entities);
	void renderScene(std::map<ModelName, std::vector<Entity>> _entities, ModelName _exclude);
	void postRenderScene(std::map<ModelName, std::vector<Entity>> _entities);

	void modelPreRender(ModelName _modelname, const  ModelData* _modelData);
	void modelRender( Entity _entity, ModelName _modelName, unsigned int _shader, std::vector<Texture*> _textures, unsigned int _indiceCount );
	void modelPostRender(ModelName _modelname, const  ModelData* _modelData);


};
