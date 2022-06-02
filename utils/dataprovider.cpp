#include "dataprovider.h"

const std::vector<VertexData> DataProvider::getCubeVertices() { return cubeVertices; };
const std::vector<unsigned int> DataProvider::getCubeIndices() { return cubeIndices; }

const unsigned int DataProvider::getCubeIndiceSize()
{
    return cubeIndices.size();
}

const std::string DataProvider::getShipObjPath()
{
    return "ressources/ship_2cannons.obj";
}
;

const std::vector<VertexData> DataProvider::cubeVertices = {
    // positions                        // normals                      // texture Coords
    {glm::vec3(-0.5f, -0.5f, -0.5f),    glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 0.0f)},
    {glm::vec3(0.5f, -0.5f, -0.5f),    glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f,  0.5f, -0.5f),    glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 1.0f)},
    {glm::vec3(0.5f,  0.5f, -0.5f),    glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 1.0f)},
    {glm::vec3(-0.5f,  0.5f, -0.5f),    glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 1.0f)},
    {glm::vec3(-0.5f, -0.5f, -0.5f),    glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 0.0f)},

    {glm::vec3(-0.5f, -0.5f,  0.5f),    glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 0.0f)},
    {glm::vec3(0.5f, -0.5f,  0.5f),    glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f,  0.5f,  0.5f),    glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 1.0f)},
    {glm::vec3(0.5f,  0.5f,  0.5f),    glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 1.0f)},
    {glm::vec3(-0.5f,  0.5f,  0.5f),    glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 1.0f)},
    {glm::vec3(-0.5f, -0.5f,  0.5f),    glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 0.0f)},

    {glm::vec3(-0.5f,  0.5f,  0.5f),    glm::vec3(-1.0f, 0.0f, 0.0),    glm::vec2(1.0f, 0.0f)},
    {glm::vec3(-0.5f,  0.5f, -0.5f),    glm::vec3(-1.0f, 0.0f, 0.0),    glm::vec2(1.0f, 1.0f)},
    {glm::vec3(-0.5f, -0.5f, -0.5f),    glm::vec3(-1.0f, 0.0f, 0.0),    glm::vec2(0.0f, 1.0f)},
    {glm::vec3(-0.5f, -0.5f, -0.5f),    glm::vec3(-1.0f, 0.0f, 0.0),    glm::vec2(0.0f, 1.0f)},
    {glm::vec3(-0.5f, -0.5f,  0.5f),    glm::vec3(-1.0f, 0.0f, 0.0),    glm::vec2(0.0f, 0.0f)},
    {glm::vec3(-0.5f,  0.5f,  0.5f),    glm::vec3(-1.0f, 0.0f, 0.0),    glm::vec2(1.0f, 0.0f)},

    {glm::vec3(0.5f,  0.5f,  0.5f),    glm::vec3(1.0f, 0.0f, 0.0),    glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f,  0.5f, -0.5f),    glm::vec3(1.0f, 0.0f, 0.0),    glm::vec2(1.0f, 1.0f)},
    {glm::vec3(0.5f, -0.5f, -0.5f),    glm::vec3(1.0f, 0.0f, 0.0),    glm::vec2(0.0f, 1.0f)},
    {glm::vec3(0.5f, -0.5f, -0.5f),    glm::vec3(1.0f, 0.0f, 0.0),    glm::vec2(0.0f, 1.0f)},
    {glm::vec3(0.5f, -0.5f,  0.5f),    glm::vec3(1.0f, 0.0f, 0.0),    glm::vec2(0.0f, 0.0f)},
    {glm::vec3(0.5f,  0.5f,  0.5f),    glm::vec3(1.0f, 0.0f, 0.0),    glm::vec2(1.0f, 0.0f)},

    {glm::vec3(-0.5f, -0.5f, -0.5f),    glm::vec3(0.0f, -1.0f, 0.0),    glm::vec2(0.0f, 1.0f)},
    {glm::vec3(0.5f, -0.5f, -0.5f),    glm::vec3(0.0f, -1.0f, 0.0),    glm::vec2(1.0f, 1.0f)},
    {glm::vec3(0.5f, -0.5f,  0.5f),    glm::vec3(0.0f, -1.0f, 0.0),    glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f, -0.5f,  0.5f),    glm::vec3(0.0f, -1.0f, 0.0),    glm::vec2(1.0f, 0.0f)},
    {glm::vec3(-0.5f, -0.5f,  0.5f),    glm::vec3(0.0f, -1.0f, 0.0),    glm::vec2(0.0f, 0.0f)},
    {glm::vec3(-0.5f, -0.5f, -0.5f),    glm::vec3(0.0f, -1.0f, 0.0),    glm::vec2(0.0f, 1.0f)},

    {glm::vec3(-0.5f,  0.5f, -0.5f),    glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 1.0f)},
    {glm::vec3(0.5f,  0.5f, -0.5f),    glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 1.0f)},
    {glm::vec3(0.5f,  0.5f,  0.5f),    glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f,  0.5f,  0.5f),    glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 0.0f)},
    {glm::vec3(-0.5f,  0.5f,  0.5f),    glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 0.0f)},
    {glm::vec3(-0.5f,  0.5f, -0.5f),    glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 1.0f)},
};

const std::vector<unsigned int> DataProvider::cubeIndices = {
       0,1,2,
       3,4,5,
       6,7,8,
       9,10,11,
       12,13,14,
       15,16,17,
       18,19,20,
       21,22,23,
       24,25,26,
       27,28,29,
       30,31,32,
       33,34,35
};