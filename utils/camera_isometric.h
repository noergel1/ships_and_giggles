#pragma once

#include "camera.h"
#include "helpers.h"
#include <iostream>

// Default camera values
const float SPEED_ISOMETRIC = 2.5f;
const float SENSITIVITY_ISOMETRIC = 0.0005f;
const float player_heightDiff = 1.5f;
const float player_xzPlaneDiff = 3.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera_Isometric : public Camera
{
public:
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // position on the circle around the player entity
    float circlePos;

    // takes the player entity, since it follows the player
    Camera_Isometric( Entity* _player );

    Entity* m_player;


    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(PlayerAction _direction, float _deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float _xoffset, float _yoffset, GLboolean _constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float _yoffset);

    void invertPitch();

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

    glm::vec3 getPosOnCircle( float _angle, float _height );
};