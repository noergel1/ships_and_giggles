#pragma once

#include "../utils/imgui/imgui.h"
#include "../utils/imgui/imgui_impl_glfw.h"
#include "../utils/imgui/imgui_impl_opengl3.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../utils/shader.h"
#include "../utils/camera.h"
#include "../utils/camera_freefloat.h"
#include "../utils/camera.h"
#include "../utils/texture_2d.h"
#include "../utils/texture_cubemap.h"
#include "../utils/debug.h"
#include "../utils/renderer.h"

#include <fstream>
#include <sstream>
#include <map>



class Application {

public:
	Application(GameSettings _settings);

	bool setupWindow(unsigned int _width, unsigned int _height);

	bool runApplication();
	bool stopApplication();



	void processInput(GLFWwindow* _window);
	void process_resize(GLFWwindow* _window, int _width, int _height);
	void process_mouse(GLFWwindow* _window, double _xpos, double _ypos);
	void process_scroll(GLFWwindow* _window, double _xoffset, double _yoffset);

	GLFWwindow* getWindow() { return m_window; }




private: 
	// game settings
	// -------------
	GameSettings m_settings;

	// window
	// -------------
	GLFWwindow* m_window;

	// execution
	// -------------
	bool setupGamestate();
	bool generateUniformBuffers();
	bool updateGamestate();

	// gamestate
	// -------------
	Entity* m_player;										// pointer to the player entity
	std::map<ModelName, std::vector<Entity> > m_entities;	// vector containing all entities in the game
	unsigned int viewProjectionBuffer;
	unsigned int timeBuffer;
	unsigned int dirLightBuffer;



	// debugging
	// -------------
	void enablePolygonMode();

	// timing
	// -------------
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// renderer instance
	// -------------
	Renderer* m_renderer;

	// debugger instance - starts in SetupApplication()
	// -------------
	glDebugger* m_debugger;

	// camera instance
	// -------------
	Camera* m_camera;
	float lastX = (float)m_settings.SCR_WIDTH / 2.0;
	float lastY = (float)m_settings.SCR_HEIGHT / 2.0;
	bool firstMouse = true;

	// controls
	// -------------
	double lastCursorToggle = 0.0f;				// notes time tab was pressed the last time
	const double cursorToggleDelay = 1.0f;		// the delay between toggling cursor
	bool cursorEnabled = false;



	// entity management
	// -------------
	bool addEntity( glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation, ModelName _modelName);

	bool addRock (glm::vec3 _position, glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 _rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	bool addCrate(glm::vec3 _position, glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 _rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	bool addShip (glm::vec3 _position, glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 _rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	bool addWater(glm::vec3 _position, glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 _rotation = glm::vec3(0.0f, 0.0f, 0.0f));



	bool setupModels();
};

