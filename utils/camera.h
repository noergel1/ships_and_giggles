#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "definitions.h"
#include "../core/gamesettings.h"

#include <vector>





// Default camera values
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    Camera();

    glm::vec3 getPosition();
    void setPosition(glm::vec3 _position);

    glm::vec3 getFront();
    void setFront(glm::vec3 _front);

    glm::vec3 getUp();
    void setUp(glm::vec3 _up);

    float getZoom();
    void setZoom( float _zoom );

    bool Accelerated = false;

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    virtual void ProcessKeyboard(PlayerAction direction, float deltaTime) = 0;

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    virtual void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) = 0;

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    virtual void ProcessMouseScroll(float yoffset) = 0;

    // inverts pitch for rendering water reflection
    virtual void invertPitch() = 0;

    // calculates the front vector from the Camera's (updated) Euler Angles
    virtual void updateCameraVectors() = 0;
protected:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Zoom;
};