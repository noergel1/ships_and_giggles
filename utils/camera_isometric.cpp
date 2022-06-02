#include "camera_isometric.h"

// constructor with vectors
Camera_Isometric::Camera_Isometric(Entity _player)
    : Camera()
    , MovementSpeed(SPEED_ISOMETRIC)
    , MouseSensitivity(SENSITIVITY_ISOMETRIC)
{
    glm::vec3 position = glm::vec3(0.0f, 3.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 front = glm::vec3(0.0f, -0.5f, -0.5f);
    Position = position;
    WorldUp = up;
    Front = front;
    updateCameraVectors();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera_Isometric::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera_Isometric::ProcessKeyboard(Camera_Movement _direction, float _deltaTime)
{
    float velocity = MovementSpeed * _deltaTime;
    if (_direction == FORWARD)
        Position += glm::vec3(Position.x, 0.0f, Position.z) * velocity;
    if (_direction == BACKWARD)
        Position -= glm::vec3(Position.x, 0.0f, Position.z) * velocity;
    if (_direction == LEFT)
        Position -= Right * velocity;
    if (_direction == RIGHT)
        Position += Right * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera_Isometric::ProcessMouseMovement(float _xoffset, float _yoffset, GLboolean _constrainPitch)
{
    _xoffset *= MouseSensitivity;
    _yoffset *= MouseSensitivity;

    Yaw += _xoffset;
    Pitch += _yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (_constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

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
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}