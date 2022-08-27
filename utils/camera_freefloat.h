#pragma once

#include "camera.h"


// Default camera values
const float YAW_FREEFLOAT = -90.0f;
const float PITCH_FREEFLOAT = 0.0f;
const float SPEED_FREEFLOAT = 2.5f;
const float ACCELERATION_FACTOR = 3.0f;
const float SENSITIVITY_FREEFLOAT = 0.1f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera_FreeFloat : public Camera
{
public:
    // camera options
    float MovementSpeed;
    float MouseSensitivity;

    // constructor with vectors
    Camera_FreeFloat(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW_FREEFLOAT, float pitch = PITCH_FREEFLOAT);

    // constructor with scalar values
    Camera_FreeFloat(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};