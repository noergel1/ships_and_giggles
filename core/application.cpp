#include "application.h"


Application::Application(Camera_Mode _cameraMode)
{
    setupWindow();
    
    if (_cameraMode == FREE_FLOAT)
    {
        m_camera = new Camera_FreeFloat(glm::vec3(0.0f, 0.0f, 3.0f));
    }
    else if (_cameraMode == ISOMETRIC)
    {
        return;
    }

    m_renderer = new Renderer(m_camera);
}

bool Application::setupWindow()
{
    // glfw: initialize and configure
 // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // enable debugger
    m_debugger = new glDebugger();

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);


    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    m_debugger->enableDebug();

    m_window = window;

    return true;
}

bool Application::setupGamestate()
{
    // generate objects
    addCrate(
        //scale
        glm::vec3(0.0f, 0.0f, 0.0f), 
        //rotation
        glm::vec3(0.0f, 0.0f, 0.0f), 
        // position
        glm::vec3(0.0f, 0.0f, 0.0f) 
    );

    addCrate(
        //scale
        glm::vec3(1.0f, 1.0f, 1.0f),
        //rotation
        glm::vec3(0.0f, 0.0f, 0.0f),
        // position
        glm::vec3(3.0f, 0.0f, 0.0f)
    );

    addShip(
        //scale
        glm::vec3(1.0f, 1.0f, 1.0f),
        //rotation
        glm::vec3(0.0f, 0.0f, 0.0f),
        // position
        glm::vec3(3.0f, 0.0f, 0.0f)
    );


    return true;
}

bool Application::runApplication()
{
    setupGamestate();

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(m_window))
    {


        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // input
        // -----
        processInput(m_window);

        // render
        // ------
        glClearColor(0.5f, 0.5f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        m_renderer->Draw(m_entities);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    return true;
}

void Application
::enablePolygonMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Application::processInput(GLFWwindow* _window)
{
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(_window, true);

    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera->ProcessKeyboard(FORWARD, deltaTime);

    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera->ProcessKeyboard(BACKWARD, deltaTime);

    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera->ProcessKeyboard(LEFT, deltaTime);

    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera->ProcessKeyboard(RIGHT, deltaTime);
}



void Application::process_resize(GLFWwindow* _window, int _width, int _height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, _width, _height);
}



void Application::process_mouse(GLFWwindow* _window, double _xpos, double _ypos)
{
    if (firstMouse)
    {
        lastX = _xpos;
        lastY = _ypos;
        firstMouse = false;
    }

    float xoffset = _xpos - lastX;
    float yoffset = lastY - _ypos; // reversed since y-coordinates go from bottom to top

    lastX = _xpos;
    lastY = _ypos;

    m_camera->ProcessMouseMovement(xoffset, yoffset);
}




void Application::process_scroll(GLFWwindow* _window, double _xoffset, double _yoffset)
{
    m_camera->ProcessMouseScroll(_yoffset);
}

bool Application::addEntity(glm::vec3 _scale, glm::vec3 _rotation, glm::vec3 _position, std::string _modelName)
{
    Entity newEntity = {
        //scale
        _scale,
        //rotation
        _rotation,
        // position
        _position
    };


    m_entities[_modelName].push_back(newEntity);

    return true;
}

bool Application::addRock(glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation)
{
    addEntity(_scale, _rotation, _position, getModelIdentifier(ModelName::ROCK_MODEL));
    return true;
}

bool Application::addCrate(glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation)
{
    addEntity(_scale, _rotation, _position, getModelIdentifier(ModelName::CRATE_MODEL));
    return true;
}

bool Application::addShip(glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation)
{
    addEntity(_scale, _rotation, _position, getModelIdentifier(ModelName::SHIP_MODEL));
    return true;
}
