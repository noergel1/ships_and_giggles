#include "renderer.h"


	Renderer::Renderer(Camera* _camera)
		:m_camera(_camera)
	{
		


		SetupBaseVaos();
		SetupShaders();
		SetupModelData();
	};

	//bool Renderer::DrawStandardObject(ModelName _modelName, std::vector<Entity> _entities)
	//{
	//	ModelData* modelData = m_models[_modelName];

	//	std::vector<Texture> diffuseMaps = modelData->getDiffuseMaps();
	//	std::vector<Texture> specularMaps = modelData->getDiffuseMaps();
	//	float shininess = modelData->getShininess();

	//	Shader* shader = m_shaders[modelData->m_Shader];
	//	shader->use();


	//	// set view and projection matrices inside the shader
	//	glm::mat4 view = m_camera->GetViewMatrix();
	//	glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), (float)800 / (float)600, 0.1f, 100.0f);
	//	glm::mat4 model = glm::mat4(1.0f);
	//	shader->setMat4("view", view);
	//	shader->setMat4("projection", projection);

	//	// camera position for light calculation
	//	shader->setVec3("viewPos", m_camera->Position);

	//	// set material sampler slots
	//	shader->setInt("material1.diffuse", 0);
	//	shader->setInt("material1.specular", 1);
	//	shader->setFloat("material1.shininess", shininess);


	//	// set lights
	//	shader->setVec3("dirLight.direction", 0.1f, -1.0f, 0.1f);
	//	shader->setVec3("dirLight.ambient", glm::vec3(0.2f));
	//	shader->setVec3("dirLight.diffuse", glm::vec3(0.2f));
	//	shader->setVec3("dirLight.specular", glm::vec3(1.0f));

	//	// bind vao
	//	glBindVertexArray(m_vaos[modelData->m_VAO]);



	//	for (auto const& _entity : _entities) {


	//		// set model matrix
	//		shader->setMat4("model", glm::translate(glm::mat4(1.0f), _entity.Position));

	//		// set the corresponding textures
	//		diffuseMaps[0].use(0);
	//		specularMaps[0].use(1);

	//		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//	}


	//	return true;
	//}

	bool Renderer::Draw(std::map<std::string, std::vector<Entity>> _entities)
	{
		ShaderReference curActivatedShader_reference = ShaderReference::NO_SHADER;
		unsigned int curActivatedVAO_reference = 0;

		// go through all key/value pairs in the _entities map
		for (auto const& _entityVector : _entities) {
			std::string modelName = _entityVector.first;
			std::vector<Entity> entities = _entityVector.second;

			ModelData* modelData = m_models[modelName];

			Shader* modelShader = m_shaders[modelData->m_Shader];
			unsigned int modelVao = modelData->m_VAO;

			std::vector<Texture> modelTextures = modelData->getTextures();
			float modelShininess = modelData->getShininess();

			if (curActivatedShader_reference != modelData->m_Shader)
			{
				modelShader->use();
				curActivatedShader_reference = modelData->m_Shader;
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
					modelShader->setVec3("viewPos", m_camera->Position);

					// set material sampler slots
					modelShader->setInt("material1.diffuse", 0);
					modelShader->setInt("material1.specular", 1);
					modelShader->setFloat("material1.shininess", modelData->getShininess());


					// set lights
					modelShader->setVec3("dirLight.direction", 0.1f, -1.0f, 0.1f);
					modelShader->setVec3("dirLight.ambient", glm::vec3(0.2f));
					modelShader->setVec3("dirLight.diffuse", glm::vec3(0.2f));
					modelShader->setVec3("dirLight.specular", glm::vec3(1.0f));

					break;
				case ShaderReference::NO_SHADER:
					std::cout << "NO_SHADER for model " << modelName << std::endl;
					return false;
			}

			// set view and projection matrices inside the shader
			glm::mat4 view = m_camera->GetViewMatrix();
			glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 model = glm::mat4(1.0f);
			modelShader->setMat4("view", view);
			modelShader->setMat4("projection", projection);

			for (auto const& entity : entities) {
				// set model matrix
				modelShader->setMat4("model", glm::translate(glm::mat4(1.0f), entity.Position));

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

	//bool Renderer::AddNewModel(VaoReference _vao, ShaderReference _shader, std::vector<VertexData> _vertices,
	//	std::vector<unsigned int> _indices, std::vector<Texture> _textures, float _shininess, ModelName _modelName)
	//{
	//	ModelData* newModel = new ModelData(_vao, _shader, _vertices, _indices, _textures, _shininess);

	//	// save key/value pair in m_models
	//	m_models[_modelName] = newModel;

	//	return true;
	//}

	bool Renderer::AddNewModel(std::string _modelName, VaoReference _vao, ShaderReference _shader, std::vector<Texture> _textures, float _shininess)
	{
		unsigned int modelVao = m_baseVaos[_vao];

		ModelData* newModel = new ModelData(modelVao, getBaseModelIndiceSize(_vao), _shader, _textures, _shininess);

		m_models[_modelName] = newModel;


		return false;
	}

	bool Renderer::AddNewModel(std::string _modelName, std::vector<VertexData> _vertices, std::vector<unsigned int> _indices, ShaderReference _shader, std::vector<Texture> _textures, float _shininess)
	{
		unsigned int modelVao = createVao(_vertices, _indices);

		ModelData* newModel = new ModelData(modelVao, _indices.size(), _shader, _textures, _shininess);

		m_models[_modelName] = newModel;

		return false;
	}

	bool Renderer::AddNewModel(std::string _modelName, std::vector<VertexData> _vertices, ShaderReference _shader, std::vector<Texture> _textures, float _shininess)
	{
		unsigned int modelVao = createVao(_vertices);

		ModelData* newModel = new ModelData(modelVao, _vertices.size(), _shader, _textures, _shininess);

		m_models[_modelName] = newModel;

		return false;
	}

	bool Renderer::AddCrateModel()
	{
		std::vector<Texture> modelTextures = { Texture("ressources/crate/crate.png", false, TextureType::DIFFUSE), 
												Texture("ressources/crate/crate_specular.png", false, TextureType::SPECULAR) };

		AddNewModel(
			getModelIdentifier(ModelName::CRATE_MODEL),
			VaoReference::CUBE_VAO,					// vao reference
			ShaderReference::STANDARD_SHADER,		// shader reference
			modelTextures,						// textures
			16.0f							// shininess
		);

		return true;
	}

	bool Renderer::AddShipModel()
	{
		std::vector<VertexData> modelVertices;
		std::vector<unsigned int> modelIndices;
		std::vector<Texture> modelTextures;
		
		ModelLoader::loadModel("ressources/ship/ship_2cannons.obj", &modelVertices, &modelIndices, &modelTextures, glm::mat4());

		AddNewModel(
			getModelIdentifier(ModelName::SHIP_MODEL),
			modelVertices,					// vertices
			modelIndices,					// indices
			ShaderReference::STANDARD_SHADER,	// shader reference
			modelTextures,					// textures
			16.0f						// shininess
		);

		return true;
	}

	unsigned int Renderer::getBaseModelIndiceSize(VaoReference _vaoReference)
	{
		switch (_vaoReference)
		{
			case VaoReference::CUBE_VAO:
				return DataProvider::getCubeIndiceSize();
			default:
				return 0;
		}
	}

	// creates the vaos and puts the key/value pairs into the vao map
	bool Renderer::SetupBaseVaos()
	{
		// make cube vao
		std::vector<VertexData> cubeVertices = DataProvider::getCubeVertices();
		std::vector<unsigned int> cubeIndices = DataProvider::getCubeIndices();

		unsigned int cubeVao = createVao(cubeVertices, cubeIndices);
		m_baseVaos[VaoReference::CUBE_VAO] = cubeVao;




		//for (std::pair<ModelName, ModelData*> model : m_models) {


		//	ModelName modelName = model.first;
		//	VaoReference modelVao = m_models[modelName]->m_VAO;
		//	if (m_vaos[modelVao]) continue;

		//	ModelData* modelData = model.second;

		//	std::vector<VertexData> vertices = modelData->getVertices();
		//	std::vector<unsigned int> indices = modelData->getIndices();

		//	unsigned int VAO;
		//	glGenVertexArrays(1, &VAO);
		//	glBindVertexArray(VAO);

		//	unsigned int VBO;
		//	glGenBuffers(1, &VBO);
		//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertices.size(), &vertices.at(0), GL_STATIC_DRAW);

		//	// position attribute
		//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		//	glEnableVertexAttribArray(0);
		//	// normal attribute
		//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		//	glEnableVertexAttribArray(1);
		//	// texCoord attribute
		//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		//	glEnableVertexAttribArray(2);

		//	unsigned int EBO;
		//	glGenBuffers(1, &EBO);
		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices.at(0), GL_STATIC_DRAW);

		//	glBindVertexArray(0);

		//	m_vaos[modelVao] = VAO;
		//}


		return true;

	}


	unsigned int Renderer::createVao(const std::vector<VertexData>& _vertices, const std::vector<unsigned int>& _indices)
	{
	
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
	
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * _vertices.size(), &_vertices.at(0), GL_STATIC_DRAW);
	
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	
		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), &_indices.at(0), GL_STATIC_DRAW);
	
		glBindVertexArray(0);

		return VAO;
	}
	unsigned int Renderer::createVao(const std::vector<VertexData>& _vertices)
	{
		std::vector<unsigned int> indices(_vertices.size());
		std::iota(std::begin(indices), std::end(indices), 0); // Fill with consecutive ints up to _vertices.size() - 1

		unsigned int VAO = createVao(_vertices, indices);


		return VAO;
	}
	;

	// creates the shaders and puts the key/value pairs into the shader map
	bool Renderer::SetupShaders()
	{
		Shader* standardShader = new Shader("shader/vs.glsl", "shader/fs.glsl");
		m_shaders[ShaderReference::STANDARD_SHADER] = standardShader;


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
	bool Renderer::SetupModelData()
	{
		AddCrateModel();
		AddShipModel();

		return true;
	}
	;
