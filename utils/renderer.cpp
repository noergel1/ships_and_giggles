#include "renderer.h"


	Renderer::Renderer(Camera* _camera, GameSettings _settings)
		:m_camera(_camera)
		,m_settings(_settings)

		// using a struct for framebuffer, so they don't have to be created each frame
		,m_renderVariables({
			/*framebuffer_postprocessing:  */	Framebuffer(_settings.SCR_WIDTH, _settings.SCR_HEIGHT),
			
			/*framebuffer_waterReflection: */	Framebuffer(REFLECTION_WIDTH, REFLECTION_HEIGHT ),
			/*framebuffer_waterRefraction: */	Framebuffer( REFRACTION_WIDTH, REFRACTION_HEIGHT ),


		})
	{
		SetupShaders();
		SetUniforms();
		SetRenderVariables();
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

	bool Renderer::SetUniforms() {

		//standard shader
		unsigned int standardShader = m_shaders["standard"];
		Shader::useShader( standardShader );
		// camera position for light calculation
		Shader::setVec3( standardShader, "viewPos", m_camera->Position );

		// set material sampler slots
		Shader::setInt( standardShader, "material1.diffuse", 0 );
		Shader::setInt( standardShader, "material1.specular", 1 );
		Shader::setFloat( standardShader, "material1.shininess", 16.0f );

		Shader::useShader( 0 );

				//standard shader
		unsigned int waterShader = m_shaders["water"];
		Shader::useShader( waterShader );
// camera position for light calculation
		Shader::setVec3( waterShader, "viewPos", m_camera->Position );
		Shader::useShader( 0 );

		return false;
	}

	bool Renderer::SetRenderVariables() {
		//postprocessing
		m_renderVariables.framebuffer_postprocessing.createDepthStencilRenderbufferAttachement();

		// water reflection
		m_renderVariables.framebuffer_waterReflection.createDepthRenderbufferAttachement();

		// water refraction
		m_renderVariables.framebuffer_waterRefraction.createDepthTextureAttachement();

		return false;
	}

	bool Renderer::Draw(std::map<ModelName, std::vector<Entity>> _entities)
	{
		preRenderScene(_entities);
		renderScene(_entities);
		postRenderScene(_entities);

		return true;
	}



	void Renderer::preRenderScene(std::map<ModelName, std::vector<Entity>> _entities)
	{
		// render on texture framebuffer if postprocessing is enabled
		if (m_models[ModelName::POSTPROCESSING])
		{
			m_renderVariables.framebuffer_postprocessing.bind();
			// make sure to clear the framebuffer's content

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//m_renderVariables.framebuffer_postprocessing.unbind();
		}

		// enable testing
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_STENCIL_TEST );
		glEnable( GL_CULL_FACE );

		// reset depth test
		glDepthFunc( GL_LESS );
		glDepthMask( GL_TRUE );

		// reset face culling
		glFrontFace( GL_CCW );
		glCullFace( GL_BACK );
	}

	void Renderer::renderScene(std::map<ModelName, std::vector<Entity>> _entities)
	{
		// go through all key/value pairs in the _entities map
		for (auto const& _entityVector : _entities) {

			const ModelName modelName = _entityVector.first;
			const ModelData* modelData = m_models[modelName];
			std::vector<Entity> entities = _entityVector.second;

			// bind vao
			BindVao(modelData->m_VAO);

			// get data
			const unsigned int modelShader = m_shaders[modelData->m_shader];
			std::vector<Texture*> modelTextures = modelData->m_textures;
			const unsigned int indiceCount = modelData->m_indiceCount;

			//render
			modelPreRender(modelName, modelData);

			for (auto const& entity : entities) {
				// render function executed for every entity
				modelRender(entity, modelName, modelShader, modelTextures, indiceCount);
			}

			modelPostRender(modelName, modelData);
		}
	}

	void Renderer::renderScene( std::map<ModelName, std::vector<Entity>> _entities, ModelName _exclude ) {
				// go through all key/value pairs in the _entities map
		for (auto const& _entityVector : _entities) {

			const ModelName modelName = _entityVector.first;

			if (modelName == _exclude) continue;

			const ModelData* modelData = m_models[modelName];
			std::vector<Entity> entities = _entityVector.second;

			// bind vao
			BindVao( modelData->m_VAO );

			// get data
			const unsigned int modelShader = m_shaders[modelData->m_shader];
			std::vector<Texture*> modelTextures = modelData->m_textures;
			const unsigned int indiceCount = modelData->m_indiceCount;

			//render
			modelPreRender( modelName, modelData );

			for (auto const& entity : entities) {
				// render function executed for every entity
				modelRender( entity, modelName, modelShader, modelTextures, indiceCount );
			}

			modelPostRender( modelName, modelData );
		}
	}

	void Renderer::postRenderScene(std::map<ModelName, std::vector<Entity>> _entities)
	{
		if (m_settings.SHOW_NORMALS) {
			for (auto const& _entityVector : _entities) {

				const ModelName modelName = _entityVector.first;
				const ModelData* modelData = m_models[modelName];

				if (modelName == ModelName::SKYBOX) continue;

				std::vector<Entity> entities = _entityVector.second;

				// bind vao
				BindVao( modelData->m_VAO );

				// get data
				const unsigned int modelShader = modelName == ModelName::WATER ? m_shaders["waterNormals"] : m_shaders["showNormals"];
				std::vector<Texture*> modelTextures = modelData->m_textures;
				const unsigned int indiceCount = modelData->m_indiceCount;

				Shader::useShader( modelShader );
				for (auto const& entity : entities) {
					// render function executed for every entity
					modelRender( entity, modelName, modelShader, modelTextures, indiceCount);
				}
			}
		}

		if (m_settings.SHOW_VERTICES) {
			for (auto const& _entityVector : _entities) {

				const ModelName modelName = _entityVector.first;
				const ModelData* modelData = m_models[modelName];

				if (modelName == ModelName::SKYBOX) continue;

				std::vector<Entity> entities = _entityVector.second;

				// bind vao
				BindVao( modelData->m_VAO );

				// get data
				const unsigned int modelShader = m_shaders["showVertices"];
				std::vector<Texture*> modelTextures = modelData->m_textures;
				const unsigned int indiceCount = modelData->m_indiceCount;

				Shader::useShader( modelShader );
				for (auto const& entity : entities) {
					// render function executed for every entity
					modelRender( entity, modelName, modelShader, modelTextures, indiceCount );
				}
			}
		}

		// postprocessing
		if (m_models[ModelName::POSTPROCESSING])
		{
			const ModelData* postprocessingModel = m_models[ModelName::POSTPROCESSING];
			const unsigned int postprocessingShader = m_shaders[postprocessingModel->m_shader];
			const std::vector<float> kernel = m_settings.POSTPROCESSING_KERNEL;

			Shader::useShader( postprocessingShader );
			Shader::setInt( postprocessingShader, "screenTexture", 0);
			for (int i = 0; i < 9; i++) 				{
				std::string curKernel = "kernel[" + std::to_string(i) + "]";
				Shader::setFloat( postprocessingShader, curKernel, kernel[i]);
			}

			//// bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

			BindVao(postprocessingModel->m_VAO);
			m_renderVariables.framebuffer_postprocessing.bindTexture();	// use the color attachment texture as the texture of the quad plane
			glDrawElements(GL_TRIANGLES, postprocessingModel->m_indiceCount, GL_UNSIGNED_INT, 0);

			//unbind shader
			Shader::useShader(0);

		}
	}

	// pre & post render functions
	void Renderer::modelPreRender(ModelName _modelname, const  ModelData* _modelData)
	{
		// set shader
		const unsigned int shader = m_shaders[_modelData->m_shader];
		Shader::useShader(shader);

		// set shader variables depending on the model
		switch (_modelname)
		{
		case ModelName::SKYBOX:
			//change depth function to LEQUAL, so the skybox doesn't z-fight with the clearcolor
			glDepthFunc(GL_LEQUAL);
			// change face culling to cull outer instead of inner faces
			glCullFace(GL_FRONT);
			break;
		}

		// set the corresponding textures
		for (auto texture : _modelData->m_textures) {
			texture->use();
		}
	}

	void Renderer::modelRender( Entity _entity, ModelName _modelName, unsigned int _shader, std::vector<Texture*> _textures, unsigned int _indiceCount )
	{

		// set model matrix
		glm::mat4 model = glm::mat4( 1.0f );


						//translate
		model = glm::translate( model, _entity.Position );

				// rotate
		model = glm::rotate( model, glm::radians( _entity.Rotation.x ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
		model = glm::rotate( model, glm::radians( _entity.Rotation.y ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::rotate( model, glm::radians( _entity.Rotation.z ), glm::vec3( 0.0f, 0.0f, 1.0f ) );

						//scale
		model = glm::scale( model, _entity.Scale );



		Shader::setMat4( _shader, "model", model );


		// draw
		glDrawElements(GL_TRIANGLES, _indiceCount, GL_UNSIGNED_INT, 0);
	}

	void Renderer::modelPostRender(ModelName _modelname, const ModelData* _modelData)
	{
		const unsigned int shader = m_shaders[_modelData->m_shader];

		// RESET
		// enable testing
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_CULL_FACE);

		// reset depth test
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);

		// reset face culling
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
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
