#include "core/application.h"


void setupCallbacks(Application _gameInstance);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


Application* gameInstance;

int main(void)
{
    gameInstance = new Application(FREE_FLOAT);
    //gameInstance->enablePolygonMode();

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
    ImGuiIO& io = ImGui::GetIO();

    if (!io.WantCaptureMouse)
    {
        gameInstance->process_mouse(window, xpos, ypos);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    gameInstance->process_scroll(window, xoffset, yoffset);
}