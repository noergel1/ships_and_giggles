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
