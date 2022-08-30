#include "camera.h"

Camera::Camera()
// camera Attributes
	:Position(glm::vec3(0.0f, 0.0f, 0.0f))
	,Front(glm::vec3(0.0f, 0.0f, -1.0f))
	,Up(glm::vec3(0.0f, 1.0f, 0.0f))
	,Right(glm::vec3(0.0f, 0.0f, 0.0f))
	,WorldUp(glm::vec3(0.0f, 0.0f, 0.0f))
	,Zoom(45.0f)
{
}

glm::vec3 Camera::getPosition() {
    return Position;
}

void Camera::setPosition(glm::vec3 _position) {
    Position = _position;
    updateCameraVectors();
}

glm::vec3 Camera::getFront() {
    return Front;
}

void Camera::setFront( glm::vec3 _front ) {
    Front = _front;
    updateCameraVectors();
}

glm::vec3 Camera::getUp() {
    return Up;
}

void Camera::setUp( glm::vec3 _up ) {
    Up = _up;
    updateCameraVectors();
}

float Camera::getZoom() {
    return Zoom;
}

void Camera::setZoom( float _zoom ) {
    Zoom = _zoom;
    updateCameraVectors();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix() {
    updateCameraVectors();
    return glm::lookAt( Position, Position + Front, Up );
}

