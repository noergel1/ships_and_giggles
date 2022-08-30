#include "camera_isometric.h"


// constructor with vectors
Camera_Isometric::Camera_Isometric( Entity* _player )
    : Camera()
    , MovementSpeed(SPEED_ISOMETRIC)
    , MouseSensitivity(SENSITIVITY_ISOMETRIC)
    , m_player(_player)
    , circlePos( glm::three_over_two_pi<float>() )
{
    Zoom = 45.0f;
    WorldUp = glm::vec3( 0.0f, 1.0f, 0.0f );
    updateCameraVectors();
}



// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera_Isometric::ProcessKeyboard(PlayerAction _direction, float _deltaTime)
{
    Position = getPosOnCircle( circlePos, player_heightDiff );
    return;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera_Isometric::ProcessMouseMovement(float _xoffset, float _yoffset, GLboolean _constrainPitch)
{
    _xoffset *= MouseSensitivity;
    _yoffset *= MouseSensitivity;

    circlePos += _xoffset;
    Position = getPosOnCircle( circlePos, player_heightDiff );

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera_Isometric::ProcessMouseScroll(float _yoffset)
{
    Zoom -= (float)_yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera_Isometric::updateCameraVectors()
{
    // calculate the new Front vector
    Front = glm::normalize(m_player->Position - Position);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}

glm::vec3 Camera_Isometric::getPosOnCircle( float _angle, float _height ) {
    glm::vec3 camPos = (m_player->Position) + glm::vec3( cos( _angle ) * player_xzPlaneDiff, _height, sin( _angle ) * player_xzPlaneDiff );
    return camPos;
}


void Camera_Isometric::invertPitch() {
    return;
}