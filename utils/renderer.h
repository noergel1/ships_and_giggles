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



class Renderer {

public:

	Renderer(Camera* _camera, GameSettings _settings);
	bool shutdownRenderer();


	void renderScene( std::map<ModelName, std::vector<Entity>> _entities );
	void renderScene( std::map<ModelName, std::vector<Entity>> _entities, std::vector<ModelName> _exclude );
	void renderScene( std::map<ModelName, std::vector<Entity>> _entities, std::string _shaderName);
	void renderScene( std::map<ModelName, std::vector<Entity>> _entities, std::string _shaderName, std::vector<ModelName> _exclude );

	void renderEntities(ModelName _modelName, std::vector<Entity> _entities);
	void renderEntities(ModelName _modelName, std::vector<Entity> _entities, std::string _shaderName );


	bool AddNewModel( ModelName _modelName, const unsigned int _vao, const unsigned int _indiceCount, const std::string _shader, const std::vector<Texture*> _textures, const float _shininess );
	bool AddNewModel( ModelName _modelName, ModelData* _modelData );

	unsigned int getShaderID( std::string _shaderName );
	const ModelData* getModelData( ModelName _modelName );
	bool BindVao(unsigned int _vao);

private:
	void modelRender( Entity _entity, unsigned int _shader, unsigned int _indiceCount );

	Camera* m_camera;
	GameSettings m_settings;

	// model data holds VAO and shader references, aswell as diffuse/specular maps and shininess
	std::map<ModelName, const ModelData*> m_models;

	// maps hold all shader and VAO references
	std::map<const std::string, const unsigned int> m_shaders;		// map for shaders


	bool SetupShaders();







};
