#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/shader.h"
#include "utils/camera.h"
#include "utils/texture.h"
#include "utils/debug.h"
#include "utils/mesh2.h"

#include <fstream>
#include <sstream>
#include <map>


#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
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
    glDebugger debugger;

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    debugger.enableDebug();


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);





    // build and compile shaders
    // -------------------------
    Shader shader("shader/vs.glsl", "shader/fs.glsl");
    Shader shaderSingleColor("shader/vs.glsl", "shader/fsSingleColor.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    static const float s_HalfEdgeLength = 2.0f;

    float cubeVertices[] = {
        // positions                                                            // texture Coords
            -s_HalfEdgeLength, -s_HalfEdgeLength, -s_HalfEdgeLength,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
             s_HalfEdgeLength, -s_HalfEdgeLength, -s_HalfEdgeLength,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
             s_HalfEdgeLength,  s_HalfEdgeLength, -s_HalfEdgeLength,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
            -s_HalfEdgeLength,  s_HalfEdgeLength, -s_HalfEdgeLength,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,

             s_HalfEdgeLength, -s_HalfEdgeLength, -s_HalfEdgeLength,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
             s_HalfEdgeLength, -s_HalfEdgeLength,  s_HalfEdgeLength,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
             s_HalfEdgeLength,  s_HalfEdgeLength,  s_HalfEdgeLength,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
             s_HalfEdgeLength,  s_HalfEdgeLength, -s_HalfEdgeLength,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

             s_HalfEdgeLength, -s_HalfEdgeLength,  s_HalfEdgeLength,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
            -s_HalfEdgeLength, -s_HalfEdgeLength,  s_HalfEdgeLength,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
            -s_HalfEdgeLength,  s_HalfEdgeLength,  s_HalfEdgeLength,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
             s_HalfEdgeLength,  s_HalfEdgeLength,  s_HalfEdgeLength,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,

            -s_HalfEdgeLength, -s_HalfEdgeLength,  s_HalfEdgeLength,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
            -s_HalfEdgeLength, -s_HalfEdgeLength, -s_HalfEdgeLength,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
            -s_HalfEdgeLength,  s_HalfEdgeLength, -s_HalfEdgeLength,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
            -s_HalfEdgeLength,  s_HalfEdgeLength,  s_HalfEdgeLength,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

            -s_HalfEdgeLength,  s_HalfEdgeLength, -s_HalfEdgeLength,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
             s_HalfEdgeLength,  s_HalfEdgeLength, -s_HalfEdgeLength,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
             s_HalfEdgeLength,  s_HalfEdgeLength,  s_HalfEdgeLength,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
            -s_HalfEdgeLength,  s_HalfEdgeLength,  s_HalfEdgeLength,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,

            -s_HalfEdgeLength, -s_HalfEdgeLength,  s_HalfEdgeLength,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
             s_HalfEdgeLength, -s_HalfEdgeLength,  s_HalfEdgeLength,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
             s_HalfEdgeLength, -s_HalfEdgeLength, -s_HalfEdgeLength,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
            -s_HalfEdgeLength, -s_HalfEdgeLength, -s_HalfEdgeLength,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,

    };

    unsigned int cubeIndices[] = {
              0,  1,  2, 
              0,  2,  3, 

              4,  5,  6, 
              4,  6,  7, 

              8,  9, 10, 
              8, 10, 11, 

             12, 13, 14, 
             12, 14, 15, 

             16, 17, 18, 
             16, 18, 19, 

             20, 21, 22, 
             20, 22, 23, 
    };




    // shader configuration
    // --------------------
    //shader.setVec3("dirLight.direction", 0.1f, -1.0f, 0.1f);
    //shader.setVec3("dirLight.ambient", glm::vec3(0.2f));
    //shader.setVec3("dirLight.diffuse", glm::vec3(0.2f));
    //shader.setVec3("dirLight.specular", glm::vec3(1.0f));

    Texture cubeTexture_diffuse("ressources/box.png", false);
    Texture cubeTexture_specular("ressources/box_specular.png", false);
    sMeshTextures cubeTextures(cubeTexture_diffuse, cubeTexture_specular, 16.0f);


    cMesh cube(cubeVertices, cubeIndices, sizeof(cubeVertices)/sizeof(cubeVertices[0]), sizeof(cubeIndices)/sizeof(cubeIndices[0]), cubeTextures, shader);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {


        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        cube.drawMesh();


        


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}