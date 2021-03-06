#include "application.h"



Application::Application(GameSettings _settings)
    : m_settings(_settings)
{

    setupWindow(m_settings.SCR_WIDTH, m_settings.SCR_HEIGHT);
    
    if (m_settings.CAM_MODE == Camera_Mode::FREE_FLOAT)
    {
        m_camera = new Camera_FreeFloat(glm::vec3(0.0f, 0.0f, 3.0f));
    }
    else if (m_settings.CAM_MODE == ISOMETRIC)
    {
        return;
    }

    m_renderer = new Renderer(m_camera, m_settings);
}

bool Application::setupWindow(unsigned int _width, unsigned int _height)
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
    GLFWwindow* window = glfwCreateWindow(_width, _height, "LearnOpenGL", NULL, NULL);
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

    if (m_settings.ENABLE_POLYGONMODE) enablePolygonMode();


    return true;
}

bool Application::setupModels()
{
    ////////////////////////////////////////////////////////
    // CREATE ALL VAOS AND GET TEXTURES                   //
    ////////////////////////////////////////////////////////

    unsigned int cubeVao = 0;
    unsigned int shipVao = 0;

    // cube model
    std::vector<VertexData> cubeVertices = DataProvider::getCubeVertices();
    std::vector<unsigned int> cubeIndices = DataProvider::getCubeIndices();
    std::vector<Texture*> standardCrateTextures = { new Texture_2D("ressources/crate/crate.png", false, TextureType::DIFFUSE, 0),
                                                    new Texture_2D("ressources/crate/crate_specular.png", false, TextureType::SPECULAR, 1) };

    cubeVao = createVao(cubeVertices, cubeIndices);


    // ship model
    std::vector<VertexData> shipVertices;
    std::vector<unsigned int> shipIndices;
    std::vector<Texture*> shipTextures;

    ModelLoader::loadModel("ressources/ship/ship_2cannons.obj", &shipVertices, &shipIndices, &shipTextures, glm::mat4());

    shipVao = createVao(shipVertices, shipIndices);

    // plane model
    std::vector<VertexData> planeVertices;
    std::vector<unsigned int> planeIndices;
    std::vector<Texture*> planeTextures;

    planeVertices = DataProvider::getPlaneVertices();
    planeIndices = DataProvider::getPlaneIndices();

    unsigned int planeVao = createVao(planeVertices, planeIndices);

    // skybox textures
    std::vector<const char*> skyboxPaths =
    {
        "ressources/skybox/day_water/right.jpg",
        "ressources/skybox/day_water/left.jpg",
        "ressources/skybox/day_water/top.jpg",
        "ressources/skybox/day_water/bottom.jpg",
        "ressources/skybox/day_water/front.jpg",
        "ressources/skybox/day_water/back.jpg"
    };
    std::vector<Texture*> skyboxTextures = { new Texture_CubeMap(skyboxPaths, false) };


    // postprocessing quad
    std::vector<float> quadVertices = DataProvider::getQuadVertices();
    std::vector<unsigned int> quadIndices = DataProvider::getQuadIndices();
    // quad VAO
    // TODO: createVAO should have parameters to know what is inside the vertexdata
    // TODO: data format for vertex data should be more flexible to have different compositions
    unsigned int quadVao;
    glGenVertexArrays(1, &quadVao);
    glBindVertexArray(quadVao);

    unsigned int quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * quadVertices.size(), &quadVertices.at(0), GL_STATIC_DRAW);

    // position with x and y coords
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // texcoords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    unsigned int quadEbo;
    glGenBuffers(1, &quadEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * quadIndices.size(), &quadIndices.at(0), GL_STATIC_DRAW);

    glBindVertexArray(0);

    glBindVertexArray(0);
    unsigned int QuadVao, QuadVbo;
    glGenVertexArrays(1, &QuadVao);
    glGenBuffers(1, &QuadVbo);
    glBindVertexArray(QuadVao);
    glBindBuffer(GL_ARRAY_BUFFER, QuadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    // texture 
    std::vector<Texture*> quadTextures = { new Texture_2D(m_settings.SCR_WIDTH, m_settings.SCR_HEIGHT, TextureType::DIFFUSE, 0) };



    ////////////////////////////////////////////////////////////////
    // CREATE MODELDATAS INSIDE RENDERER                         //
    //////////////////////////////////////////////////////////////

    m_renderer->AddNewModel(
        ModelName::CRATE,
        cubeVao,
        cubeIndices.size(),
        ShaderReference::STANDARD_SHADER,
        standardCrateTextures,
        16.0f
    );

    m_renderer->AddNewModel(
        ModelName::SHIP_STANDARD,
        shipVao,
        shipIndices.size(),
        ShaderReference::STANDARD_SHADER,
        shipTextures,
        16.0f
    );

    m_renderer->AddNewModel(
        ModelName::WATER,
        planeVao,
        planeIndices.size(),
        ShaderReference::STANDARD_SHADER,
        planeTextures,
        16.0f
    );

    m_renderer->AddNewModel(
        ModelName::SKYBOX,
        cubeVao,
        cubeIndices.size(),
        ShaderReference::SKYBOX_SHADER,
        skyboxTextures,
        16.0f
    );

    // renderer does no postprocessing, if model isn't added
    if (m_settings.POSTPROCESSING_KERNEL != NULL)
    {
        m_renderer->AddNewModel(
            ModelName::POSTPROCESSING,
            quadVao,
            quadIndices.size(),
            ShaderReference::POSTPROCESSING_SHADER,
            quadTextures,
            0.0f
        );
    }

    return false;
}

bool Application::setupGamestate()
{
    setupModels();

    // generate objects
    addCrate(
        // position
        glm::vec3(1.0f, 0.0f, 0.0f), 
        //scale
        glm::vec3(1.0f), 
        //rotation
        glm::vec3(0.0f, 0.0f, 0.0f)
    );

    addCrate(
        // position
        glm::vec3(1.0f, 0.0f, 0.0f),
        //scale
        glm::vec3(3.0f, 1.0f, 1.0f),
        //rotation
        glm::vec3(1.0f, 1.0f, 1.0f)
    );

    addShip(
        // position
        glm::vec3(-1.0f, 0.0f, 0.0f),
        //scale
        glm::vec3(0.5f),
        //rotation
        glm::vec3(0.0f, 0.0f, 0.0f)
    );

    addCrate(
        // position
        glm::vec3(0.0f, 0.0f, 0.0f),
        //scale
        glm::vec3(1.0f),
        //rotation
        glm::vec3(45.0f, 1.0f, 1.0f)
    );

    addWater(
        // position
        glm::vec3(-1.0f, 0.0f, 0.0f),
        //scale
        glm::vec3(1.0f),
        //rotation
        glm::vec3(0.0f, 0.0f, 0.0f)
    );

    //skybox
    addEntity(
        //scale
        glm::vec3(1.0f, 1.0f, 1.0f),
        //rotation
        glm::vec3(0.0f),
        // position
        glm::vec3(0.0f, 0.0f, 0.0f),
        //modelname
        ModelName::SKYBOX
    );


    return true;
}

bool Application::runApplication()
{
#ifndef NDEBUG

    // initialize imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

#endif // !NDEBUG

    setupGamestate();

    // enable testing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    //glCullFace(GL_FRONT);


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


        // imgui
#ifndef NDEBUG
        if (cursorEnabled == true)
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }
#endif // !NDEBUG

        // render the scene
        m_renderer->Draw(m_entities);


        // imgui
#ifndef NDEBUG
        if (cursorEnabled == true)
        {
        ImGui::Begin("Hello imgui");
        ImGui::Text("this is me");
        ImGui::End();

            ImGui::Render();
            ImGui:ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
#endif // !NDEBUG


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    stopApplication();

    return true;
}

bool Application::stopApplication()
{
    // delete vaos, vbos & shaders
    m_renderer->shutdownRenderer();

#ifndef NDEBUG
    // shutdown imgui if debugmode is enabled
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif // !NDEBUG


    return false;
}





// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Application::processInput(GLFWwindow* _window)
{

    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);

    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera->ProcessKeyboard(FORWARD, deltaTime);

    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera->ProcessKeyboard(BACKWARD, deltaTime);

    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera->ProcessKeyboard(LEFT, deltaTime);

    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera->ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(_window, GLFW_KEY_TAB) == GLFW_PRESS && (lastFrame - lastCursorToggle) >= cursorToggleDelay)
    {
        lastCursorToggle = lastFrame;

        if (cursorEnabled == false)
        {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursorEnabled = true;
        }
        else
        {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursorEnabled = false;
        }
    }


}

void Application::process_resize(GLFWwindow* _window, int _width, int _height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, _width, _height);
}

void Application::process_mouse(GLFWwindow* _window, double _xpos, double _ypos)
{
    if (cursorEnabled == false)
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
}

void Application::process_scroll(GLFWwindow* _window, double _xoffset, double _yoffset)
{
    m_camera->ProcessMouseScroll(_yoffset);
}



bool Application::addEntity(glm::vec3 _scale, glm::vec3 _rotation, glm::vec3 _position, ModelName _modelName)
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
    addEntity(_scale, _rotation, _position, ModelName::ROCK);
    return true;
}

bool Application::addCrate(glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation)
{
    addEntity(_scale, _rotation, _position, ModelName::CRATE);
    return true;
}

bool Application::addShip(glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation)
{
    addEntity(_scale, _rotation, _position, ModelName::SHIP_STANDARD);
    return true;
}

bool Application::addWater(glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation)
{
    addEntity(_scale, _rotation, _position, ModelName::WATER);
    return true;
}



void Application
::enablePolygonMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

