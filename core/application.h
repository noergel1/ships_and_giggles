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



struct RenderVariables {
	Framebuffer framebuffer_postprocessing;
	Framebuffer framebuffer_waterReflection;
	Framebuffer framebuffer_waterRefraction;
};

class Application {

public:
	Application(GameSettings _settings);


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
	const unsigned int REFLECTION_WIDTH = 320;
	const unsigned int REFLECTION_HEIGHT = 180;
	const unsigned int REFRACTION_WIDTH = 1280;
	const unsigned int REFRACTION_HEIGHT = 720;

	// render functions
	// -------------
	RenderVariables* m_renderVariables;

	// window
	// -------------
	GLFWwindow* m_window;

	// preparation
	// -------------
	bool setupWindow(unsigned int _width, unsigned int _height);
	bool setupGamestate();
	bool setupModels();
	bool generateUniformBuffers();
	bool SetRenderVariables();
	bool SetUniforms();


	// execution
	// -------------
	bool updateGamestate();
	bool renderFramebuffers();
	bool renderFrame();

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

	// utility
	bool clearBufferBits();
	bool resetTesting();
};

