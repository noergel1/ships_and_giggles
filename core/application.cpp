#include "application.h"



Application::Application(GameSettings _settings)
    : m_settings(_settings)
{

    setupWindow(m_settings.SCR_WIDTH, m_settings.SCR_HEIGHT);

                // using a struct for framebuffer, so they don't have to be created each frame
    m_renderVariables = new RenderVariables( {
        /*framebuffer_postprocessing:  */	Framebuffer( _settings.SCR_WIDTH, _settings.SCR_HEIGHT ),

        /*framebuffer_waterReflection: */	Framebuffer( REFLECTION_WIDTH, REFLECTION_HEIGHT ),
        /*framebuffer_waterRefraction: */	Framebuffer( REFRACTION_WIDTH, REFRACTION_HEIGHT ),
                                             } );

    m_renderer = new Renderer(m_settings);
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
    std::vector<Texture*> standardCrateTextures = { new Texture_2D("ressources/crate/crate.png", false),
                                                    new Texture_2D("ressources/crate/crate_specular.png", false) };

    cubeVao = createVao(cubeVertices, cubeIndices);


    // ship model
    std::vector<VertexData> shipVertices;
    std::vector<unsigned int> shipIndices;
    std::vector<Texture*> shipTextures;

    ModelLoader::loadModel("ressources/ship/ship_2cannons.obj", &shipVertices, &shipIndices, &shipTextures, glm::mat4());

    shipVao = createVao(shipVertices, shipIndices);

    // plane model
    unsigned int divisions = 500;
    glm::vec3 v0 = glm::vec3( -0.5f, 0.0f, -0.5f );
    glm::vec3 v1 = glm::vec3( 0.5f, 0.0f, -0.5f );
    glm::vec3 v2 = glm::vec3( 0.5f, 0.0f, 0.5f );
    glm::vec3 v3 = glm::vec3( -0.5f, 0.0f, 0.5f );
    std::vector<VertexData> planeVertices = DataProvider::generatePlaneVertices( v0, v1, v2, v3, divisions );
    std::vector<unsigned int> planeIndices = DataProvider::generatePlaneIndices( divisions );
    unsigned int planeVao = createVao(planeVertices, planeIndices);

    std::vector<Texture*> waterTextures = std::vector<Texture*>( { &m_renderVariables->framebuffer_waterReflection.m_texture, &m_renderVariables->framebuffer_waterRefraction.m_texture, new Texture_2D( "ressources/water/water_dudv.jpg", false ) } );



    // sphere model
    unsigned int sectorCount = 20;
    unsigned int stackCount = 20;
    float sphereRadius = 1.0f;
    std::vector<VertexData> sphereVertices = DataProvider::generateSphereVertices( sectorCount, stackCount, sphereRadius );
    std::vector<unsigned int> sphereIndices = DataProvider::generateSphereIndices( sectorCount, stackCount );
    std::vector<Texture*> cannonballTextures = std::vector<Texture*>( { new Texture_2D( "ressources/cannonball/cannonball_diffuse.jpg", false ), new Texture_2D( "ressources/cannonball/cannonball_specular.png", false ) } );

    unsigned int sphereVao = createVao( sphereVertices, sphereIndices );


    // capsule model
    unsigned int capsuleDivisions = 10;
    float capsuleHeight = 1.0f;
    float capsuleRadius = 1.0f;
    std::vector<VertexData> capsuleVertices = m_gameLogic->getShipColliderVertices();
    std::vector<unsigned int> capsuleIndices = m_gameLogic->getShipColliderIndices();

    unsigned int capsuleVao = createVao( capsuleVertices, capsuleIndices );


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
    std::vector<Texture*> quadTextures = { &m_renderVariables->framebuffer_postprocessing.m_texture };


    ////////////////////////////////////////////////////////////////
    // CREATE COLLIDER VAOS INSIDE RENDERER                      //
    //////////////////////////////////////////////////////////////

    m_renderer->AddColliderVao( ColliderType::SPHERE, sphereVao, sphereIndices.size() );
    m_renderer->AddColliderVao( ColliderType::CUBE, cubeVao, cubeIndices.size() );
    m_renderer->AddColliderVao( ColliderType::CAPSULE, capsuleVao, capsuleIndices.size() );


    ////////////////////////////////////////////////////////////////
    // CREATE MODELDATAS INSIDE RENDERER                         //
    //////////////////////////////////////////////////////////////

    m_renderer->AddNewModel(
        ModelName::CRATE,
        cubeVao,
        cubeIndices.size(),
        "standard",
        standardCrateTextures,
        16.0f
    );

    m_renderer->AddNewModel(
        ModelName::SHIP_STANDARD,
        shipVao,
        shipIndices.size(),
        "standard",
        shipTextures,
        16.0f
    );

    m_renderer->AddNewModel(
        ModelName::WATER,
        planeVao,
        planeIndices.size(),
        "water",
        waterTextures,
        32.0f
    );

    m_renderer->AddNewModel(
        ModelName::SKYBOX,
        cubeVao,
        cubeIndices.size(),
        "skybox",
        skyboxTextures,
        16.0f
    );

    m_renderer->AddNewModel(
        ModelName::CANNONBALL,
        sphereVao,
        sphereIndices.size(),
        "standard",
        cannonballTextures,
        16.0f
    );

    m_renderer->AddNewModel(
        ModelName::TEST_OBJECT,
        capsuleVao,
        capsuleIndices.size(),
        "standard",
        standardCrateTextures,
        16.0f
    );

    // renderer does no postprocessing, if model isn't added
    if (!m_settings.POSTPROCESSING_KERNEL.empty())
    {
        m_renderer->AddNewModel(
            ModelName::POSTPROCESSING,
            quadVao,
            quadIndices.size(),
            "postprocessing",
            quadTextures,
            0.0f
        );
    }

    return false;
}


bool Application::generateUniformBuffers() {
    // set uniform buffer object slots

    // uniform buffer for view and projection matrix
    glGenBuffers( 1, &viewProjectionBuffer );

    glBindBuffer( GL_UNIFORM_BUFFER, viewProjectionBuffer );
    glBufferData( GL_UNIFORM_BUFFER, 2 * sizeof( glm::mat4 ), NULL, GL_STATIC_DRAW );
    glBindBufferRange( GL_UNIFORM_BUFFER, 0, viewProjectionBuffer, 0, 2 * sizeof( glm::mat4 ) );

    glBindBuffer( GL_UNIFORM_BUFFER, 0 );

    // uniform buffer for time
    glGenBuffers( 1, &timeBuffer );

    glBindBuffer( GL_UNIFORM_BUFFER, timeBuffer );
    glBufferData( GL_UNIFORM_BUFFER, sizeof(float), NULL, GL_STATIC_DRAW );

    glBindBufferRange( GL_UNIFORM_BUFFER, 1, timeBuffer, 0, sizeof( float ) );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );

    // uniform buffer for directional light
    glGenBuffers( 1, &dirLightBuffer );

    glBindBuffer( GL_UNIFORM_BUFFER, dirLightBuffer );
    glBufferData( GL_UNIFORM_BUFFER, 4*16, NULL, GL_STATIC_DRAW);
    glBindBufferRange( GL_UNIFORM_BUFFER, 2, dirLightBuffer, 0, 4 * sizeof( glm::vec4 ) );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );

    // uniform buffer for clipping plane
    glEnable( GL_CLIP_DISTANCE0 );
    glGenBuffers( 1, &clippingPlaneBuffer );

    glBindBuffer( GL_UNIFORM_BUFFER, clippingPlaneBuffer );
    glBufferData( GL_UNIFORM_BUFFER, sizeof( glm::vec4 ), NULL, GL_STATIC_DRAW);
    glBindBufferRange( GL_UNIFORM_BUFFER, 3, clippingPlaneBuffer, 0, sizeof( glm::vec4 ) );
    setClippingPlane( glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );

    // uniform buffer for camera
    glEnable( GL_CLIP_DISTANCE0 );
    glGenBuffers( 1, &cameraBuffer );

    glBindBuffer( GL_UNIFORM_BUFFER, cameraBuffer );
    glBufferData( GL_UNIFORM_BUFFER, sizeof( glm::vec4 ), NULL, GL_STATIC_DRAW );
    glBindBufferRange( GL_UNIFORM_BUFFER, 4, cameraBuffer, 0, sizeof( glm::vec3 ) );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );

    return false;
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

    m_gameLogic = new GameLogic( m_settings );
    m_camera = m_gameLogic->getCamera();
    m_gameLogic->setupGame();
    generateUniformBuffers();
    setupModels();



    SetUniforms();
    SetRenderVariables();

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

        // update gamestate
        // ----------------
        m_gameLogic->tick( deltaTime );
        updateUniforms();


        // reset everything
        // ---------------
        clearBufferBits();
        resetTesting();


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
        renderFrame();


        fillImGui();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    stopApplication();

    return true;
}

bool Application::updateUniforms() {
    updateCameraMatrices();

    glBindBuffer( GL_UNIFORM_BUFFER, timeBuffer );
    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof( float ), &lastFrame );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );

        // uniform buffer for directional light
    glBindBuffer( GL_UNIFORM_BUFFER, dirLightBuffer );

    //set the directional light here, because it doesn't need to be updated every frame
    glm::vec3 lightDirection = glm::vec3( 0.1f, -1.0f, 0.1f );
    glm::vec3 ambientColor = glm::vec3( 0.2f );
    glm::vec3 diffuseColor = glm::vec3( 0.2f );
    glm::vec3 specularColor = glm::vec3( 1.0f );

    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof( glm::vec3 ), glm::value_ptr( lightDirection ) );
    glBufferSubData( GL_UNIFORM_BUFFER, sizeof( glm::vec4 ), sizeof( glm::vec3 ), glm::value_ptr( ambientColor ) );
    glBufferSubData( GL_UNIFORM_BUFFER, 2 * sizeof( glm::vec4 ), sizeof( glm::vec3 ), glm::value_ptr( diffuseColor ) );
    glBufferSubData( GL_UNIFORM_BUFFER, 3 * sizeof( glm::vec4 ), sizeof( glm::vec3 ), glm::value_ptr( specularColor ) );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );

    // set the camera position
    glBindBuffer( GL_UNIFORM_BUFFER, cameraBuffer );
    glm::vec3 cameraPos = m_camera->getPosition();
    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof( glm::vec3 ), &cameraPos );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );

    return true;
}

bool Application::updateCameraMatrices() {
        // update matrices
    glm::mat4 view = m_camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective( glm::radians( m_camera->getZoom() ), (float)m_settings.SCR_WIDTH / (float)m_settings.SCR_HEIGHT, 0.1f, 100.0f );

    glBindBuffer( GL_UNIFORM_BUFFER, viewProjectionBuffer );
    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof( glm::mat4 ), glm::value_ptr( projection ) );
    glBufferSubData( GL_UNIFORM_BUFFER, sizeof( glm::mat4 ), sizeof( glm::mat4 ), glm::value_ptr( view ) );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );
    return true;
}

bool Application::renderFramebuffers() {
    const std::map<ModelName, std::vector<Entity*>> entities = m_gameLogic->getEntities();

    // render reflection
    // --------------
    m_renderVariables->framebuffer_waterReflection.bind();
    clearBufferBits();
    glm::vec3 cameraPos = m_camera->getPosition();
    float distance = 2 * (cameraPos.y - waterHeight);
    m_camera->setPosition( glm::vec3(cameraPos.x, m_camera->getPosition().y - distance, cameraPos.z) );
    m_camera->invertPitch();
    updateCameraMatrices();
    setClippingPlane( glm::vec4( 0.0f, 1.0f, 0.0f, waterHeight ) );
    //setClippingPlane( glm::vec4( 0.0f ) );
    // render scene
    m_renderer->renderScene( entities, std::vector<ModelName>{ModelName::SKYBOX, ModelName::WATER} );


    // render skybox
    //change depth function to LEQUAL, so the skybox doesn't z-fight with the clearcolor
    glDepthFunc( GL_LEQUAL );
    // change face culling to cull outer instead of inner faces
    glCullFace( GL_FRONT );
    m_renderer->renderEntities( ModelName::SKYBOX, entities.at(ModelName::SKYBOX) );
    resetTesting();
    m_renderVariables->framebuffer_waterReflection.unbind();
    m_camera->setPosition( glm::vec3( cameraPos.x, m_camera->getPosition().y + distance, cameraPos.z ) );
    m_camera->invertPitch();
    updateCameraMatrices();


    // render refraction
    // --------------
    m_renderVariables->framebuffer_waterRefraction.bind();
    clearBufferBits();
    setClippingPlane( glm::vec4( 0.0f, -1.0f, 0.0f, waterHeight ) );
    //setClippingPlane( glm::vec4( 0.0f ) );
    // render scene
    m_renderer->renderScene( entities, std::vector<ModelName>{ModelName::SKYBOX, ModelName::WATER} );


    // render skybox
    //change depth function to LEQUAL, so the skybox doesn't z-fight with the clearcolor
    glDepthFunc( GL_LEQUAL );
    // change face culling to cull outer instead of inner faces
    glCullFace( GL_FRONT );
    m_renderer->renderEntities( ModelName::SKYBOX, entities.at(ModelName::SKYBOX) );
    resetTesting();
    m_renderVariables->framebuffer_waterRefraction.unbind();

    setClippingPlane( glm::vec4( 0.0f ) );

    return false;
}

bool Application::renderFrame() {
    const std::map<ModelName, std::vector<Entity*>> entities = m_gameLogic->getEntities();

    //pre render scene
    renderFramebuffers();

    // render on texture framebuffer if postprocessing is enabled
    if (!m_settings.POSTPROCESSING_KERNEL.empty()) {
        m_renderVariables->framebuffer_postprocessing.bind();

        // make sure to clear the framebuffer's content
        clearBufferBits();
    }

    // render scene
    // --------------
    m_renderer->renderScene( entities, std::vector<ModelName>{ModelName::SKYBOX, ModelName::WATER} );


    // render skybox
    // --------------
    //change depth function to LEQUAL, so the skybox doesn't z-fight with the clearcolor
    glDepthFunc( GL_LEQUAL );
    // change face culling to cull outer instead of inner faces
    glCullFace( GL_FRONT );
    m_renderer->renderEntities( ModelName::SKYBOX, entities.at(ModelName::SKYBOX) );
    resetTesting();

    m_renderer->renderEntities( ModelName::WATER, entities.at(ModelName::WATER));

    // render debugging options
    // --------------
    if (m_settings.SHOW_NORMALS) {
        m_renderer->renderScene( entities, "showNormals", std::vector<ModelName>{ModelName::SKYBOX, ModelName::WATER} );
        m_renderer->renderEntities( ModelName::WATER, entities.at(ModelName::WATER), "waterNormals");
    }

    if (m_settings.SHOW_VERTICES) {
        m_renderer->renderScene( entities, "showVertices", std::vector<ModelName>{ModelName::SKYBOX, ModelName::WATER} );
    }

    if (m_settings.SHOW_COLLIDERS) {
        m_renderer->renderColliders( entities, m_gameLogic->getColliders(), std::vector<ModelName>{ModelName::SKYBOX, ModelName::WATER} );
    };

    // postprocessing
    // --------------
    if (!m_settings.POSTPROCESSING_KERNEL.empty()) {
        const std::vector<float> kernel = m_settings.POSTPROCESSING_KERNEL;
        const ModelData* postProcessingModel = m_renderer->getModelData( ModelName::POSTPROCESSING );
        unsigned int postprocessingShader = m_renderer->getShaderID( "postprocessing" );
        Shader::useShader( postprocessingShader );
        Shader::setInt( postprocessingShader, "screenTexture", 0 );
        for (int i = 0; i < 9; i++) {
            std::string curKernel = "kernel[" + std::to_string( i ) + "]";
            Shader::setFloat( postprocessingShader, curKernel, kernel[i] );
        }

        //// bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        glDisable( GL_DEPTH_TEST ); // disable depth test so screen-space quad isn't discarded due to depth test.

        m_renderer->BindVao( postProcessingModel->m_VAO );

        m_renderVariables->framebuffer_postprocessing.m_texture.use(0); // use the color attachment texture as the texture of the quad plane
        glDrawElements( GL_TRIANGLES, postProcessingModel->m_indiceCount, GL_UNSIGNED_INT, 0 );

        Shader::useShader( 0 );
    }

    return false;
}

bool Application::SetUniforms() {
        //standard shader
        // --------------
    unsigned int standardShader = m_renderer->getShaderID("standard");
    Shader::useShader( standardShader );

    // set material sampler slots
    Shader::setInt( standardShader, "material1.diffuse", 0 );
    Shader::setInt( standardShader, "material1.specular", 1 );
    Shader::setFloat( standardShader, "material1.shininess", 16.0f );

    Shader::useShader( 0 );

    //water shader
    // --------------
    unsigned int waterShader = m_renderer->getShaderID("water");
    Shader::useShader( waterShader );
    Shader::setVec3( waterShader, "viewPos", m_camera->getPosition() );
    Shader::setInt( waterShader, "reflectionTexture", 0 );
    Shader::setInt( waterShader, "refractionTexture", 1 );
    Shader::setInt( waterShader, "dudvTexture", 2 );
    Shader::useShader( 0 );

    //post processing
    // --------------
    if (!m_settings.POSTPROCESSING_KERNEL.empty())         {
        unsigned int postprocessingShader = m_renderer->getShaderID( "postprocessing" );
        const std::vector<float> kernel = m_settings.POSTPROCESSING_KERNEL;
        Shader::useShader( postprocessingShader );
        Shader::setInt( postprocessingShader, "screenTexture", 0 );
        for (int i = 0; i < 9; i++) {
            std::string curKernel = "kernel[" + std::to_string( i ) + "]";
            Shader::setFloat( postprocessingShader, curKernel, kernel[i] );
        }
        Shader::useShader( 0 );
    }


    return false;
}

bool Application::SetRenderVariables() {
    //postprocessing
    m_renderVariables->framebuffer_postprocessing.bind();
    resetTesting();
    clearBufferBits();
    m_renderVariables->framebuffer_postprocessing.createDepthStencilRenderbufferAttachement();
    m_renderVariables->framebuffer_postprocessing.unbind();

    // water reflection
    m_renderVariables->framebuffer_waterReflection.bind();
    m_renderVariables->framebuffer_waterReflection.createDepthRenderbufferAttachement();
    resetTesting();
    clearBufferBits();
    m_renderVariables->framebuffer_waterReflection.unbind();

    // water refraction
    m_renderVariables->framebuffer_waterRefraction.bind();
    m_renderVariables->framebuffer_waterRefraction.createDepthTextureAttachement();
    resetTesting();
    clearBufferBits();
    m_renderVariables->framebuffer_waterRefraction.unbind();

    return false;
}

void Application::fillImGui() {
        // imgui
#ifndef NDEBUG
        if (cursorEnabled == true)
        {
        ImVec2 windowSize = ImVec2( 50.0f, 50.0f );
        //ImGui::SetNextWindowSize( windowSize );

        ImGui::Begin("Debugging");
        ImGui::Text("Settings");
        ImGui::Checkbox( "Show Colliders", &m_settings.SHOW_COLLIDERS );
        ImGui::Checkbox( "Poligon Mode", &m_settings.ENABLE_POLYGONMODE );
        ImGui::Checkbox( "Show Normals", &m_settings.SHOW_NORMALS );
        ImGui::Checkbox( "Show Vertices", &m_settings.SHOW_VERTICES );

        m_gameLogic->fillImGui();
        ImGui::End();

            ImGui::Render();
            ImGui:ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
#endif // !NDEBUG

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

    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        m_gameLogic->processKeyboard(PlayerAction::LEFT, deltaTime);

    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        m_gameLogic->processKeyboard(PlayerAction::RIGHT, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        m_gameLogic->processKeyboard(PlayerAction::FORWARD, deltaTime);

    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        m_gameLogic->processKeyboard(PlayerAction::BACKWARD, deltaTime);

    if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_gameLogic->processKeyboard(PlayerAction::SHOOT, deltaTime);


    if (glfwGetKey( _window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS)
        m_camera->Accelerated = true;

    if (glfwGetKey( _window, GLFW_KEY_LEFT_SHIFT ) == GLFW_RELEASE)
        m_camera->Accelerated = false;

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

bool Application::clearBufferBits() {
    glClearColor( 1.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    return false;
}

bool Application::resetTesting() {
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

    return false;
}

bool Application::setClippingPlane( glm::vec4 _clippingPlane ) {
    glBindBuffer( GL_UNIFORM_BUFFER, clippingPlaneBuffer );
    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof( glm::vec4 ), glm::value_ptr( _clippingPlane ) );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );
    return false;
}




void Application
::enablePolygonMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

