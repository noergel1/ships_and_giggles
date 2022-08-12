#include "renderer.h"


	Renderer::Renderer(Camera* _camera, GameSettings _settings)
		:m_camera(_camera)
		,m_settings(_settings)

		// using a struct for framebuffer, so they don't have to be created each frame
		,m_renderVariables({
			/*framebuffer_postprocessing: */	Framebuffer(_settings.SCR_WIDTH, _settings.SCR_HEIGHT),
		})
	{
		SetupShaders();
	}

	// creates the shaders and puts the key/value pairs into the shader map
	bool Renderer::SetupShaders()
	{
		//old

		const unsigned int standardShader = Shader::createShader("shader/vs.glsl", "shader/fs.glsl");
		m_shaders.insert(std::pair<ShaderReference, const unsigned int>(ShaderReference::STANDARD_SHADER, standardShader));

		const unsigned int waterShader = Shader::createShader("shader/waterVS.glsl", "shader/waterFS.glsl");
		m_shaders.insert(std::pair<ShaderReference, const unsigned int>(ShaderReference::WATER_SHADER, waterShader));

		const unsigned int skyboxShader = Shader::createShader("shader/skyboxVS.glsl", "shader/skyboxFS.glsl");
		m_shaders.insert(std::pair<ShaderReference, const unsigned int>(ShaderReference::SKYBOX_SHADER, skyboxShader));

		const unsigned int postprocessingShader = Shader::createShader("shader/postprocessingVS.glsl", "shader/postprocessingFS.glsl");
		m_shaders.insert(std::pair<ShaderReference, const unsigned int>(ShaderReference::POSTPROCESSING_SHADER, postprocessingShader));

		const unsigned int testingShader = Shader::createShader( "shader/testVS.glsl", "shader/testFS.glsl" );
		m_shaders.insert( std::pair<ShaderReference, const unsigned int>( ShaderReference::TESTING_SHADER, testingShader ) );


		struct shaderFiles {
			std::string vs;
			std::string fs;
			std::string gs = "";
		};

		// wahrscheinlich am besten eine map 

		boost::regex shaderRegex;
		//new
		try {
			shaderRegex = boost::regex("shader\\\\\\w*(VS|FS|vs|fs).glsl");
		}
		catch (const std::string err) {
			std::cout << err << '\n';
		};

		std::cout << (boost::regex_match( "shader\\blblafS.glsl", shaderRegex) ? "matched" : "not matched") << '\n';
		for (auto const& dir_entry : boost::filesystem::directory_iterator( "shader" )) {
			std::string shaderPath = dir_entry.path().string();
			if (!boost::regex_match( shaderPath, shaderRegex )) {
				std::cout << shaderPath << " didnt match regex" << '\n';
				continue;
			}

			switch ()


		}
		

		return true;
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
	}

	void Renderer::renderScene(std::map<ModelName, std::vector<Entity>> _entities)
	{
		// go through all key/value pairs in the _entities map
		for (auto const& _entityVector : _entities) {

			const ModelName modelName = _entityVector.first;
			//if (modelName == ModelName::SKYBOX) continue;
			const ModelData* modelData = m_models[modelName];
			std::vector<Entity> entities = _entityVector.second;

			// bind vao
			BindVao(modelData->m_VAO);

			// get model shader id
			const unsigned int modelShader = m_shaders[modelData->m_shader];


			// create model, view and projection matrices
			glm::mat4 view = m_camera->GetViewMatrix();
			glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), (float)m_settings.SCR_WIDTH / (float)m_settings.SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 model = glm::mat4(1.0f);


			//render
			modelPreRender(modelName, modelData, &model, &projection, &view);

			for (auto const& entity : entities) {
				// render function executed for every entity
				modelRender(entity, modelName, modelData, &model, &projection, &view);
			}

			modelPostRender(modelName, modelData, &model, &projection, &view);
		}
	}

	void Renderer::postRenderScene(std::map<ModelName, std::vector<Entity>> _entities)
	{
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

	// pre & post render functions
	void Renderer::modelPreRender(ModelName _modelname, const  ModelData* _modelData, glm::mat4* _model, glm::mat4* _projection, glm::mat4* _view)
	{
		const unsigned int shader = m_shaders[_modelData->m_shader];

		// activate shader
		Shader::useShader(shader);

		// set shader variables depending on the model
		switch (_modelname)
		{
		case ModelName::SKYBOX:
			//change depth function to LEQUAL, so the skybox doesn't z-fight with the clearcolor
			glDepthFunc(GL_LEQUAL);
			// cut the 4d matrix part out to ignore translation -> skybox stays around camera
			Shader::setMat4(shader, "view", glm::mat4(glm::mat3(*_view)));
			// change face culling to cull outer instead of inner faces
			glCullFace(GL_FRONT);

			Shader::setMat4(shader, "projection", *_projection);
			break;
		default:
			Shader::setMat4(shader, "view", *_view);
			Shader::setMat4(shader, "projection", *_projection);
			break;
		}

		// set shader variables depending on the shader
		switch (_modelData->m_shader)
		{
			case ShaderReference::STANDARD_SHADER:

				// camera position for light calculation
				Shader::setVec3(shader, "viewPos", m_camera->Position);

				// set material sampler slots
				Shader::setInt(shader, "material1.diffuse", 0);
				Shader::setInt(shader, "material1.specular", 1);
				Shader::setFloat(shader, "material1.shininess", _modelData->m_shininess);


				// set lights
				Shader::setVec3(shader, "dirLight.direction", 0.1f, -1.0f, 0.1f);
				Shader::setVec3(shader, "dirLight.ambient", glm::vec3(0.2f));
				Shader::setVec3(shader, "dirLight.diffuse", glm::vec3(0.2f));
				Shader::setVec3(shader, "dirLight.specular", glm::vec3(1.0f));

				break;
			case ShaderReference::NO_SHADER:
				std::cout << "NO_SHADER for model " << std::endl;
				break;
		}

		// set the corresponding textures
		for (auto texture : _modelData->m_textures) {
			texture->use();
		}
	}

	void Renderer::modelRender(Entity _entity, ModelName _modelname, const ModelData* _modelData, glm::mat4* _model, glm::mat4* _projection, glm::mat4* _view)
	{
		const unsigned int shader = m_shaders[_modelData->m_shader];

		switch (_modelname)
		{
		case ModelName::SKYBOX:
			break;
		default:
			// set model matrix

			//scale
			*_model = glm::scale(glm::mat4(1.0f), _entity.Scale);

			// rotation
			// for whatever reason, it always rotates double the amount, so i divide by two
			*_model = glm::rotate(*_model, glm::radians(_entity.Rotation.x / 2), glm::vec3(1.0f, 0.0f, 0.0f));
			*_model = glm::rotate(*_model, glm::radians(_entity.Rotation.y / 2), glm::vec3(0.0f, 1.0f, 0.0f));
			*_model = glm::rotate(*_model, glm::radians(_entity.Rotation.z / 2), glm::vec3(0.0f, 0.0f, 1.0f));



			//translation
			*_model = glm::translate(*_model, _entity.Position);


			Shader::setMat4(shader, "model", *_model);
			break;
		}



		// draw
		glDrawElements(GL_TRIANGLES, _modelData->m_indiceCount, GL_UNSIGNED_INT, 0);
	}

	void Renderer::modelPostRender(ModelName _modelname, const ModelData* _modelData, glm::mat4* _model, glm::mat4* _projection, glm::mat4* _view)
	{
		const unsigned int shader = m_shaders[_modelData->m_shader];

		switch (_modelData->m_shader)
		{
		case ShaderReference::STANDARD_SHADER: {
			break;
		};
		default: {
			break;
		}
		}

		switch (_modelname)
		{
		default:
			break;
		}

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

	bool Renderer::AddNewModel(ModelName _modelName, const unsigned int _vao, const unsigned int _indiceCount, const ShaderReference _shader, const std::vector<Texture*> _textures, const float _shininess)
	{
		const ModelData* newModel = new ModelData(_vao, _indiceCount, _shader, _textures, _shininess);
		m_models.insert(std::pair<ModelName, const ModelData*>(_modelName, newModel));

		return true;
	}