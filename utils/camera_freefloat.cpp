#include "camera_freefloat.h"

// constructor with vectors
Camera_FreeFloat::Camera_FreeFloat(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Camera()
    , MovementSpeed(SPEED_FREEFLOAT)
    , MouseSensitivity(SENSITIVITY_FREEFLOAT)
{
    Zoom = ZOOM;
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

// constructor with scalar values
Camera_FreeFloat::Camera_FreeFloat(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :  MovementSpeed(SPEED_FREEFLOAT), MouseSensitivity(SENSITIVITY_FREEFLOAT)
{
    Zoom = ZOOM;
    Front = glm::vec3(0.0f, 0.0f, -1.0f);
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera_FreeFloat::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos( glm::radians( Yaw ) ) * cos( glm::radians( Pitch ) );
    front.y = sin( glm::radians( Pitch ) );
    front.z = sin( glm::radians( Yaw ) ) * cos( glm::radians( Pitch ) );
    Front = glm::normalize( front );
    // also re-calculate the Right and Up vector
    Right = glm::normalize( glm::cross( Front, WorldUp ) );  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize( glm::cross( Right, Front ) );
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera_FreeFloat::ProcessKeyboard(PlayerAction direction, float deltaTime)
{
    float velocity = Accelerated==true  ? MovementSpeed * deltaTime * ACCELERATION_FACTOR 
                                        : MovementSpeed * deltaTime;

    if (direction == PlayerAction::FORWARD)
        Position += Front * velocity;
    if (direction == PlayerAction::BACKWARD)
        Position -= Front * velocity;
    if (direction == PlayerAction::LEFT)
        Position -= Right * velocity;
    if (direction == PlayerAction::RIGHT)
        Position += Right * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera_FreeFloat::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
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
void Camera_FreeFloat::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera_FreeFloat::invertPitch() {
    Pitch = -Pitch;
}
