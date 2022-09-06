#include "renderer.h"


	Renderer::Renderer(GameSettings _settings)
		:m_settings(_settings)
	{
		SetupShaders();
	}

	// creates the shaders and puts the key/value pairs into the shader map
	bool Renderer::SetupShaders()
	{
		struct ShaderFiles {
			std::string vs = "";
			std::string fs = "";
			std::string gs = "";
		};

		boost::regex shaderRegex;
		std::map<const std::string, ShaderFiles> sortedShaderfiles;

		//new
		try {
			shaderRegex = boost::regex("shader\\\\\\w*(VS|FS|GS).glsl");
		}
		catch (const boost::regex_error err) {
			std::cout << err.what() << '\n';
			exit(1);
		};

		for (const auto& dir_entry : boost::filesystem::directory_iterator( "shader" )) {
			std::string shaderPath = dir_entry.path().string();
			if (!boost::regex_match( shaderPath, shaderRegex )) {
				std::cout << shaderPath << " didnt match regex" << '\n';
				continue;
			}

			std::string shaderTag = shaderPath.substr( shaderPath.length() - 7, 2 );
			std::string shaderTitle = shaderPath.substr(0, shaderPath.length() - 7);
			shaderTitle.replace( shaderTitle.begin(), shaderTitle.begin()+7, "");

			if (shaderTag == "VS") {
				sortedShaderfiles[shaderTitle].vs = "shader/" + shaderTitle + "VS.glsl";
			}
			else if (shaderTag == "FS") {
				sortedShaderfiles[shaderTitle].fs = "shader/" + shaderTitle + "FS.glsl";
			}
			else if (shaderTag == "GS") {
				sortedShaderfiles[shaderTitle].gs = "shader/" + shaderTitle + "GS.glsl";
			}
		}
		
		for (auto const& entry : sortedShaderfiles) {
			std::string shaderTitle = entry.first;
			ShaderFiles shaderFiles = entry.second;
			
			if (shaderFiles.vs.empty() || shaderFiles.fs.empty()) {
				std::cout << "Shader \"" + shaderTitle + "\" vs or fs missing \n";
				continue;
			}
			
			unsigned int shader;
			if (!shaderFiles.gs.empty()) {
				shader = Shader::createShader( shaderFiles.vs.c_str(), shaderFiles.fs.c_str(), shaderFiles.gs.c_str() );
			}
			else {
				shader = Shader::createShader( shaderFiles.vs.c_str(), shaderFiles.fs.c_str() );
			}

			m_shaders.insert( std::pair<const std::string, const unsigned int>( shaderTitle, shader ) );

		}

		return true;
	}



	void Renderer::renderScene(const std::map<ModelName, std::vector<Entity*>> _entities)
	{
		// go through all key/value pairs in the _entities map
		for (auto const& _entityVector : _entities) {

			const ModelName modelName = _entityVector.first;
			const ModelData* modelData = m_models[modelName];
			std::vector<Entity*> entities = _entityVector.second;

			// bind vao
			BindVao(modelData->m_VAO);

			// get data
			const unsigned int modelShader = m_shaders[modelData->m_shader];
			std::vector<Texture*> modelTextures = modelData->m_textures;
			const unsigned int indiceCount = modelData->m_indiceCount;

			//render
			// set the corresponding textures
			for (int i = 0; i < modelData->m_textures.size(); i++) {
				Texture* curTexture = modelData->m_textures[i];
				curTexture->use(i);
			}

			for (auto const& entity : entities) {
				// render function executed for every entity
				modelRender(entity, modelShader, indiceCount);
			}
		}

		Shader::useShader( 0 );
	}

	void Renderer::renderScene( const std::map<ModelName, std::vector<Entity*>> _entities, std::vector<ModelName> _exclude ) {
				// go through all key/value pairs in the _entities map
		for (auto const& _entityVector : _entities) {

			const ModelName modelName = _entityVector.first;

			// skip the models which are to be excluded
			if (std::find( _exclude.begin(), _exclude.end(), modelName ) != _exclude.end()) {
				continue;
			}

			const ModelData* modelData = m_models[modelName];
			std::vector<Entity*> entities = _entityVector.second;

			// bind vao
			BindVao( modelData->m_VAO );

			// get data
			const unsigned int modelShader = m_shaders[modelData->m_shader];
			std::vector<Texture*> modelTextures = modelData->m_textures;
			const unsigned int indiceCount = modelData->m_indiceCount;

			// render
			//----------

			// activate shader
			Shader::useShader( modelShader );

			// set the corresponding textures
			for (int i = 0; i < modelData->m_textures.size(); i++) {
				Texture* curTexture = modelData->m_textures[i];
				curTexture->use( i );
			}

			for (auto const& entity : entities) {
				// render function executed for every entity
				modelRender( entity, modelShader, indiceCount );
			}
		}

		Shader::useShader( 0 );
	}

	void Renderer::renderScene( const std::map<ModelName, std::vector<Entity*>> _entities, std::string _shaderName ) {
		const unsigned int modelShader = m_shaders[_shaderName];

				// go through all key/value pairs in the _entities map
		for (auto const& _entityVector : _entities) {

			const ModelName modelName = _entityVector.first;

			const ModelData* modelData = m_models[modelName];
			std::vector<Entity*> entities = _entityVector.second;

			// bind vao
			BindVao( modelData->m_VAO );

			// get data

			std::vector<Texture*> modelTextures = modelData->m_textures;
			const unsigned int indiceCount = modelData->m_indiceCount;

			// render
			//----------

			// activate shader
			Shader::useShader( modelShader );

			// set the corresponding textures
			for (int i = 0; i < modelData->m_textures.size(); i++) {
				Texture* curTexture = modelData->m_textures[i];
				curTexture->use( i );
			}

			for (auto const& entity : entities) {
				// render function executed for every entity
				modelRender( entity, modelShader, indiceCount );
			}
		}

		Shader::useShader( 0 );
	}

	void Renderer::renderScene( const std::map<ModelName, std::vector<Entity*>> _entities, std::string _shaderName, std::vector<ModelName> _exclude ) {

		const unsigned int modelShader = m_shaders[_shaderName];

						// go through all key/value pairs in the _entities map
		for (auto const& _entityVector : _entities) {

			const ModelName modelName = _entityVector.first;
			
			// skip the models which are to be excluded
			if (std::find( _exclude.begin(), _exclude.end(), modelName ) != _exclude.end()) {
				continue;
			}

			const ModelData* modelData = m_models[modelName];
			std::vector<Entity*> entities = _entityVector.second;

			// bind vao
			BindVao( modelData->m_VAO );

			// get data
			std::vector<Texture*> modelTextures = modelData->m_textures;
			const unsigned int indiceCount = modelData->m_indiceCount;

			// render
			//----------

			// activate shader
			Shader::useShader( modelShader );

			// set the corresponding textures
			for (int i = 0; i < modelData->m_textures.size(); i++) {
				Texture* curTexture = modelData->m_textures[i];
				curTexture->use( i );
			}

			for (auto const& entity : entities) {
				// render function executed for every entity
				modelRender( entity, modelShader, indiceCount );
			}
		}

		Shader::useShader( 0 );
	}

	void Renderer::renderEntities( ModelName _modelName, const std::vector<Entity*> _entities ) {
			const ModelData* modelData = m_models[_modelName];

			// bind vao
			BindVao( modelData->m_VAO );

			// get data
			const unsigned int modelShader = m_shaders[modelData->m_shader];
			std::vector<Texture*> modelTextures = modelData->m_textures;
			const unsigned int indiceCount = modelData->m_indiceCount;

			// render
			//----------

			// activate shader
			Shader::useShader( modelShader );

			// set the corresponding textures
			for (int i = 0; i < modelData->m_textures.size(); i++) {
				Texture* curTexture = modelData->m_textures[i];
				curTexture->use( i );
			}

			for (auto const& entity : _entities) {
				// render function executed for every entity
				modelRender( entity, modelShader, indiceCount );
			}

			Shader::useShader( 0 );
	}

	void Renderer::renderEntities( ModelName _modelName, const std::vector<Entity*> _entities, std::string _shaderName ) {
		const ModelData* modelData = m_models[_modelName];

		// bind vao
		BindVao( modelData->m_VAO );

		// get data
		const unsigned int modelShader = m_shaders[_shaderName];
		std::vector<Texture*> modelTextures = modelData->m_textures;
		const unsigned int indiceCount = modelData->m_indiceCount;

		// render
		//----------

		// activate shader
		Shader::useShader( modelShader );

		// set the corresponding textures
		for (int i = 0; i < modelData->m_textures.size(); i++) {
			Texture* curTexture = modelData->m_textures[i];
			curTexture->use( i );
		}

		for (auto const& entity : _entities) {
			// render function executed for every entity
			modelRender( entity, modelShader, indiceCount );
		}

		Shader::useShader( 0 );
	}

	
	void Renderer::renderColliders( const std::map<ModelName, std::vector<Entity*>> _entities, std::map<ModelName, ModelCollider> _modelColliders, std::vector<ModelName> _exclude ) {
		const unsigned int modelShader = m_shaders["showColliders"];

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// go through all key/value pairs in the _entities map
		for (auto const& _entityVector : _entities) {

			const ModelName modelName = _entityVector.first;

			// skip the models which are to be excluded
			if (std::find( _exclude.begin(), _exclude.end(), modelName ) != _exclude.end()) {
				continue;
			}

			std::vector<Entity*> entities = _entityVector.second;

			// skip if the model has no collider
			if (_modelColliders.find(modelName) == _modelColliders.end()) continue;

			ColliderType curColliderType = _modelColliders[modelName].colliderType;
			ColliderData curCollider = m_colliders[curColliderType];
			Entity colliderTransform = _modelColliders[modelName].diffToModel;

			// bind vao
			BindVao( curCollider.vao );

			// get data
			const unsigned int indiceCount = curCollider.indiceCount;


			// render
			//----------

			// activate shader
			Shader::useShader( modelShader );

			for (auto const& entity : entities) {

				// render function executed for every entity
				// collider matrix
				glm::mat4 colliderMat = glm::mat4( 1.0f );


								//translate
				colliderMat = createSRTMat4( colliderTransform );


				// model matrix
				glm::mat4 modelMat = createSRTMat4( *entity );



				Shader::setMat4( modelShader, "model", modelMat * colliderMat );


				// draw
				glDrawElements(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, 0);
			}
		}

		if(!m_settings.ENABLE_POLYGONMODE) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		Shader::useShader( 0 );
	}

	void Renderer::modelRender( const Entity* _entity, unsigned int _shader, unsigned int _indiceCount )
	{

		glm::mat4 model = createSRTMat4( *_entity );



		Shader::setMat4( _shader, "model", model );


		// draw
		glDrawElements(GL_TRIANGLES, _indiceCount, GL_UNSIGNED_INT, 0);
	}

	bool Renderer::shutdownRenderer()
	{

		// free all VAOS
		std::vector<unsigned int> deletedVaos;
		for (auto const& model : m_models)
		{
			const ModelData* modelData = model.second;

			if (modelData == NULL) continue;

			unsigned int modelVao = modelData->m_VAO;
			if (std::find(deletedVaos.begin(), deletedVaos.end(), modelVao) != deletedVaos.end())
			{
				// VAO already in deletedVaos
				continue;
			}
			else
			{
				deletedVaos.push_back(modelVao);
			}
		}

		// should also free all VBOs aswell

		/////////
		////////
		///////



		// delete all the VAOs at once
		glDeleteVertexArrays(deletedVaos.size(), &deletedVaos[0]);



		// free all shaders
		for (auto const& shader : m_shaders)
		{
			unsigned int shaderId = shader.second;
			glDeleteProgram(shaderId);
		}

		return false;
	};


	bool Renderer::BindVao(unsigned int _vao)
	{
		glBindVertexArray(_vao);

		return true;
	}

	bool Renderer::AddNewModel(ModelName _modelName, const unsigned int _vao, const unsigned int _indiceCount, const std::string _shader, const std::vector<Texture*> _textures, const float _shininess)
	{
		ModelData* newModel = new ModelData();
		//_vao, _indiceCount, _shader, _textures, _shininess
		newModel->m_VAO = _vao;
		newModel->m_indiceCount = _indiceCount;
		newModel->m_shader = _shader;
		newModel->m_textures = _textures;
		newModel->m_shininess = _shininess;
		m_models.insert(std::pair<ModelName, const ModelData*>(_modelName, newModel));

		return true;
	}

	bool Renderer::AddNewModel( ModelName _modelName, ModelData* _modelData ) {
		m_models.insert( std::pair<ModelName, const ModelData*>( _modelName, _modelData ) );
		return true;
	}

	void Renderer::AddColliderVao( ColliderType _colliderType, unsigned int _vao, unsigned int _indiceCount ) {
		ColliderData colliderData = ColliderData( {
				_vao,
				_indiceCount
			} );

		m_colliders[_colliderType] = colliderData;
	}

	unsigned int Renderer::getShaderID( std::string _shaderName ) {
		return m_shaders[_shaderName];
	}

	const ModelData* Renderer::getModelData( ModelName _modelName ) {
		return m_models[_modelName];
	}
