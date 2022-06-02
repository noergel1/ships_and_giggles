#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "definitions.h"

#include <vector>

// defines the camera modes
enum Camera_Mode {
    FREE_FLOAT,
    ISOMETRIC
};

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:

    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Zoom;

    Camera();

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    virtual glm::mat4 GetViewMatrix() = 0;

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime) = 0;

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    virtual void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) = 0;

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    virtual void ProcessMouseScroll(float yoffset) = 0;

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    virtual void updateCameraVectors() = 0;
};