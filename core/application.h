#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../utils/shader.h"
#include "../utils/camera.h"
#include "../utils/camera_freefloat.h"
#include "../utils/camera.h"
#include "../utils/texture.h"
#include "../utils/debug.h"
#include "../utils/renderer.h"

#include <fstream>
#include <sstream>
#include <map>

enum ModelName {
	NO_MODEL,
	SHIP_MODEL,
	ROCK_MODEL,
	CRATE_MODEL
};



class Application {

public:
	Application(Camera_Mode _cameraMode);

	bool setupWindow();
	bool setupGamestate();
	bool runApplication();

	void enablePolygonMode();

	void processInput(GLFWwindow* _window);
	void process_resize(GLFWwindow* _window, int _width, int _height);
	void process_mouse(GLFWwindow* _window, double _xpos, double _ypos);
	void process_scroll(GLFWwindow* _window, double _xoffset, double _yoffset);

	GLFWwindow* getWindow() { return m_window; }




private: 

	// window
	GLFWwindow* m_window;


	// timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// renderer instance
	Renderer* m_renderer;

	// debugger instance - starts in SetupApplication()
	glDebugger* m_debugger;

	// camera instance
	Camera* m_camera;
	float lastX = (float)SCR_WIDTH / 2.0;
	float lastY = (float)SCR_HEIGHT / 2.0;
	bool firstMouse = true;

	// entities hold scale, rotation and position
	Entity* m_player;										// pointer to the player entity
	std::map<const ModelData*, std::vector<Entity> > m_entities;				// vector containing all entities in the game

	// entity management
	bool addEntity(glm::vec3 _scale, glm::vec3 _rotation, glm::vec3 _position, std::string _modelName);

	bool addRock (glm::vec3 _position, glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 _rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	bool addCrate(glm::vec3 _position, glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 _rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	bool addShip (glm::vec3 _position, glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 _rotation = glm::vec3(0.0f, 0.0f, 0.0f));

	std::map<std::string, const ModelData*> m_modelReferences;


	bool setupModels();
	unsigned int createVao(const std::vector<VertexData>& _vertices, const std::vector<unsigned int>& _indices);
	unsigned int createVao(const std::vector<VertexData>& _vertices);

	const std::string getModelIdentifier(ModelName _modelName);



};

