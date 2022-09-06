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

#include "../models/model_data.h"



class Renderer {

	struct ColliderData {
		unsigned int vao;
		unsigned int indiceCount;
	};

public:

	Renderer(GameSettings _settings);
	bool shutdownRenderer();


	void renderScene( const std::map<ModelName, std::vector<Entity*>> _entities );
	void renderScene( const std::map<ModelName, std::vector<Entity*>> _entities, std::vector<ModelName> _exclude );
	void renderScene( const std::map<ModelName, std::vector<Entity*>> _entities, std::string _shaderName);
	void renderScene( const std::map<ModelName, std::vector<Entity*>> _entities, std::string _shaderName, std::vector<ModelName> _exclude );

	void renderEntities(ModelName _modelName, const std::vector<Entity*> _entities);
	void renderEntities(ModelName _modelName, const std::vector<Entity*> _entities, std::string _shaderName );


	void renderColliders( const std::map<ModelName, std::vector<Entity*>> _entities, std::map<ModelName, ModelCollider> _modelColliders, std::vector<ModelName> _exclude );


	bool AddNewModel( ModelName _modelName, const unsigned int _vao, const unsigned int _indiceCount, const std::string _shader, const std::vector<Texture*> _textures, const float _shininess );
	bool AddNewModel( ModelName _modelName, ModelData* _modelData );

	void AddColliderVao( ColliderType _colliderType, unsigned int _vao, unsigned int _indiceCount );

	unsigned int getShaderID( std::string _shaderName );
	const ModelData* getModelData( ModelName _modelName );
	bool BindVao(unsigned int _vao);

private:
	void modelRender( const Entity* _entity, unsigned int _shader, unsigned int _indiceCount );

	GameSettings m_settings;

	// model data holds VAO and shader references, aswell as diffuse/specular maps and shininess
	std::map<ModelName, const ModelData*> m_models;

	// vaos for the different collider type
	std::map<ColliderType, ColliderData> m_colliders;

	// maps hold all shader and VAO references
	std::map<const std::string, const unsigned int> m_shaders;		// map for shaders


	bool SetupShaders();







};
