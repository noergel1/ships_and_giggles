#pragma once
# define M_PI           3.14159265358979323846  /* pi */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "definitions.h"
#include <fstream>
#include <sstream>
#include <map>
#include <numeric>

float degreeToRadians(float _angle);

float radiansToDegree(float _angle);

unsigned int createVao(const std::vector<VertexData>& _vertices, const std::vector<unsigned int>& _indices);
unsigned int createVao(const std::vector<VertexData>& _vertices);