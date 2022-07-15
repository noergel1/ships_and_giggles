#include "renderer.h"


	Renderer::Renderer(Camera* _camera, GameSettings _settings)
		:m_camera(_camera)
		,m_settings(_settings)
	{

		// postprocessing effects
		
		// options:
		// float* postProcessKernel = DataProvider::getSharpenKernel();
		// float* postProcessKernel = DataProvider::getBlurKernel();
		// float* postProcessKernel = DataProvider::getEdgeDetectionKernel();
		float* postProcessKernel = DataProvider::getBlurKernel();

		SetupShaders();
	}

	bool Renderer::shutdownRenderer()
	{

		// free all VAOS
		std::vector<unsigned int> deletedVaos;
		for (auto const& model : m_models)
		{
			const ModelData* modelData = model.second;
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

	bool Renderer::Draw(std::map<ModelName, std::vector<Entity>> _entities)
	{
		ShaderReference curActivatedShader_reference = ShaderReference::NO_SHADER;
		unsigned int curActivatedVAO_reference = 0;

		// go through all key/value pairs in the _entities map
		for (auto const& _entityVector : _entities) {

			// reset depth test
			glDepthFunc(GL_LESS);
			glDepthMask(GL_TRUE);
			//glDepthFunc(getEnumValue(DepthTestFunctions::gl_always));

			const ModelName modelName = _entityVector.first;
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
			preRender(modelName, modelData, &model, &projection, &view, modelShader);

			for (auto const& entity : entities) {
				// render function executed for every entity
				render(entity, modelName, modelData, &model, &projection, &view, modelShader);
			}

			postRender(modelName, modelData, &model, &projection, &view, modelShader);
		}


		return true;
	}





	// creates the shaders and puts the key/value pairs into the shader map
	bool Renderer::SetupShaders()
	{
		const unsigned int standardShader = Shader::createShader("shader/vs.glsl", "shader/fs.glsl");
		m_shaders.insert(std::pair<ShaderReference, const unsigned int>(ShaderReference::STANDARD_SHADER, standardShader));

		const unsigned int waterShader = Shader::createShader("shader/waterVS.glsl", "shader/waterFS.glsl");
		m_shaders.insert(std::pair<ShaderReference, const unsigned int>(ShaderReference::WATER_SHADER, waterShader));

		const unsigned int skyboxShader = Shader::createShader("shader/skyboxVS.glsl", "shader/skyboxFS.glsl");
		m_shaders.insert(std::pair<ShaderReference, const unsigned int>(ShaderReference::SKYBOX_SHADER, skyboxShader));

		return true;
	}


	// pre & post render functions
	void Renderer::preRender(ModelName _modelname, const  ModelData* _modelData, glm::mat4* _model, glm::mat4* _projection, glm::mat4* _view, const unsigned int& _shader)
	{
		// activate shader
		Shader::useShader(_shader);

		// set shader variables depending on the model
		switch (_modelname)
		{
		case ModelName::SKYBOX:
			//change depth function to LEQUAL, so the skybox doesn't z-fight with the clearcolor
			glDepthFunc(GL_LEQUAL);
			// cut the 4d matrix part out to ignore translation -> skybox stays around camera
			Shader::setMat4(_shader, "view", glm::mat4(glm::mat3(*_view)));
			// change face culling to cull outer instead of inner faces
			glCullFace(GL_FRONT);

			Shader::setMat4(_shader, "projection", *_projection);
			break;
		default:
			Shader::setMat4(_shader, "view", *_view);
			Shader::setMat4(_shader, "projection", *_projection);
			break;
		}

		// set shader variables depending on the shader
		switch (_modelData->m_shader)
		{
			case ShaderReference::STANDARD_SHADER:

				// camera position for light calculation
				Shader::setVec3(_shader, "viewPos", m_camera->Position);

				// set material sampler slots
				Shader::setInt(_shader, "material1.diffuse", 0);
				Shader::setInt(_shader, "material1.specular", 1);
				Shader::setFloat(_shader, "material1.shininess", _modelData->m_shininess);


				// set lights
				Shader::setVec3(_shader, "dirLight.direction", 0.1f, -1.0f, 0.1f);
				Shader::setVec3(_shader, "dirLight.ambient", glm::vec3(0.2f));
				Shader::setVec3(_shader, "dirLight.diffuse", glm::vec3(0.2f));
				Shader::setVec3(_shader, "dirLight.specular", glm::vec3(1.0f));

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

	void Renderer::render(Entity _entity, ModelName _modelname, const ModelData* _modelData, glm::mat4* _model, glm::mat4* _projection, glm::mat4* _view, const unsigned int& _shader)
	{
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


			Shader::setMat4(_shader, "model", *_model);
			break;
		}



		// draw
		glDrawElements(GL_TRIANGLES, _modelData->m_indiceCount, GL_UNSIGNED_INT, 0);
	}

	void Renderer::postRender(ModelName _modelname, const ModelData* _modelData, glm::mat4* _model, glm::mat4* _projection, glm::mat4* _view, const unsigned int& _shader)
	{
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
			// reset depth test
			glDepthFunc(GL_LESS);
			glDepthMask(GL_TRUE);

			// reset face culling
			glFrontFace(GL_CCW);
			glCullFace(GL_BACK);

			return;
		}


	}

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