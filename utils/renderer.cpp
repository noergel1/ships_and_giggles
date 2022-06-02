#include "renderer.h"


	Renderer::Renderer(Camera* _camera)
		:m_camera(_camera)
	{
		SetupShaders();
	};

	bool Renderer::Draw(std::map<const ModelData*, std::vector<Entity>> _entities)
	{
		ShaderReference curActivatedShader_reference = ShaderReference::NO_SHADER;
		unsigned int curActivatedVAO_reference = 0;

		// go through all key/value pairs in the _entities map
		for (auto const& _entityVector : _entities) {
			const ModelData* modelData = _entityVector.first;
			std::vector<Entity> entities = _entityVector.second;

			ShaderReference modelShaderReference = modelData->m_shader;
			const unsigned int modelShader = m_shaders[modelData->m_shader];

			unsigned int modelVao = modelData->m_VAO;

			const std::vector<Texture> modelTextures = modelData->m_textures;
			float modelShininess = modelData->m_shininess;

			if (curActivatedShader_reference != modelShaderReference)
			{
				Shader::useShader(modelShader);
				curActivatedShader_reference = modelShaderReference;
			}

			if (curActivatedVAO_reference != modelData->m_VAO)
			{
				BindVao(modelVao);
				curActivatedVAO_reference = modelData->m_VAO;
			}

			// set the needed uniforms for every shader
			switch (curActivatedShader_reference)
			{
				case ShaderReference::STANDARD_SHADER:
					// camera position for light calculation
					Shader::setVec3(modelShader, "viewPos", m_camera->Position);

					// set material sampler slots
					Shader::setInt(modelShader, "material1.diffuse", 0);
					Shader::setInt(modelShader, "material1.specular", 1);
					Shader::setFloat(modelShader, "material1.shininess", modelData->m_shininess);


					// set lights
					Shader::setVec3(modelShader, "dirLight.direction", 0.1f, -1.0f, 0.1f);
					Shader::setVec3(modelShader, "dirLight.ambient", glm::vec3(0.2f));
					Shader::setVec3(modelShader, "dirLight.diffuse", glm::vec3(0.2f));
					Shader::setVec3(modelShader, "dirLight.specular", glm::vec3(1.0f));

					break;
				case ShaderReference::NO_SHADER:
					std::cout << "NO_SHADER for model " << std::endl;
					return false;
			}

			// set view and projection matrices inside the shader
			glm::mat4 view = m_camera->GetViewMatrix();
			glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 model = glm::mat4(1.0f);
			Shader::setMat4(modelShader, "view", view);
			Shader::setMat4(modelShader, "projection", projection);

			for (auto const& entity : entities) {
				// set model matrix
				Shader::setMat4(modelShader, "model", glm::translate(glm::mat4(1.0f), entity.Position));

				// set the corresponding textures
				for (auto texture : modelTextures) {
					switch (texture.m_textureType)
					{
						case TextureType::DIFFUSE:
							texture.use(0);
							break;
						case TextureType::SPECULAR:
							texture.use(1);
							break;
					}
				}

				glDrawElements(GL_TRIANGLES, modelData->m_indiceCount, GL_UNSIGNED_INT, 0);
			}
		}


		return true;
	}

	const ModelData* Renderer::AddNewModel(const unsigned int _vao, const unsigned int _indiceCount, const ShaderReference _shader, const std::vector<Texture> _textures, const float _shininess)
	{
		const ModelData* newModel = new ModelData(_vao, _indiceCount, _shader, _textures, _shininess);
		m_models.push_back(newModel);

		return newModel;
	}



	// creates the shaders and puts the key/value pairs into the shader map
	bool Renderer::SetupShaders()
	{
		const unsigned int standardShader = Shader::createShader("shader/vs.glsl", "shader/fs.glsl");
		m_shaders.insert(std::pair<ShaderReference, const unsigned int>(ShaderReference::STANDARD_SHADER, standardShader));


		return true;
	}
	bool Renderer::SetShaderVariables(ShaderReference _shaderRef)
	{
		return false;
	}
	bool Renderer::BindVao(unsigned int _vao)
	{
		glBindVertexArray(_vao);

		return true;
	}
