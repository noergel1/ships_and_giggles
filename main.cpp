#include "core/application.h"


void setupCallbacks(Application _gameInstance);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#define ENABLE true
#define DISABLE false

Application* gameInstance;

int main(void)
{
    // options for postprocessing:
    // DataProvider::noPostProcessing() to disable
    // DataProvider::getBlurKernel()
    // DataProvider::getEdgeDetectionKernel()
    // DataProvider::getSharpenKernel()

    GameSettings gameSetting = {
        /* Screen Width: */             800,//px
        /* Screen Height: */            600,//px                           
        /* Camera Mode: */              Camera_Mode::FREE_FLOAT,
        /* Postprocessing: */           DataProvider::getBlurKernel(),
        /* Polygon Mode: */             DISABLE,
        /* Show Normals: */             DISABLE,
        /* Show Vertices: */            DISABLE,

    };

    gameInstance = new Application(gameSetting);

    setupCallbacks(*gameInstance);
    gameInstance->runApplication();
}



void setupCallbacks(Application _gameInstance)
{
    glfwSetFramebufferSizeCallback(gameInstance->getWindow(), framebuffer_size_callback);
    glfwSetCursorPosCallback(gameInstance->getWindow(), mouse_callback);
    glfwSetScrollCallback(gameInstance->getWindow(), scroll_callback);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
        gameInstance->process_resize(window, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
        gameInstance->process_mouse(window, xpos, ypos);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    gameInstance->process_scroll(window, xoffset, yoffset);
}