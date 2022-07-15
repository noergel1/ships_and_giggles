#include "dataprovider.h"


const std::string DataProvider::getShipObjPath()
{
    return "ressources/ship_2cannons.obj";
};

float* DataProvider::getSharpenKernel()
{
    float* kernel = new float[
        2,  2, 2,
        2,-15, 2,
        2,  2, 2
    ];

    return kernel;
}
float* DataProvider::getBlurKernel()
{
    float* kernel = new float [
            1.0f / 16, 2.0f / 16, 1.0f / 16,
            2.0f / 16, 4.0f / 16, 2.0f / 16,
            1.0f / 16, 2.0f / 16, 1.0f / 16
    ];

    return kernel;
}
float* DataProvider::getEdgeDetectionKernel()
{
    float* kernel = new float[
            1,  1, 1,
            1, -8, 1,
            1,  1, 1
    ];

    return kernel;
};


const std::vector<VertexData> DataProvider::planeVertices = {
    // positions                        // normals                      // texture Coords
    {glm::vec3(-0.5f, 0.0f, -0.5f),    glm::vec3(0.0f, 1.0f, 0.0f),   glm::vec2(0.0f, 0.0f)},
    {glm::vec3(0.5f, 0.0f,  0.5f),    glm::vec3(0.0f, 1.0f, 0.0f),   glm::vec2(1.0f,  1.0f)},
    {glm::vec3(0.5f, 0.0f, -0.5f),    glm::vec3(0.0f, 1.0f, 0.0f),   glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f, 0.0f,  0.5f),    glm::vec3(0.0f, 1.0f, 0.0f),   glm::vec2(1.0f,  1.0f)},
    {glm::vec3(-0.5f, 0.0f, -0.5f),    glm::vec3(0.0f, 1.0f, 0.0f),   glm::vec2(0.0f, 0.0f)},
    {glm::vec3(-0.5f, 0.0f,  0.5f),    glm::vec3(0.0f, 1.0f, 0.0f),   glm::vec2(0.0f,  1.0f)},
};
const std::vector<unsigned int> DataProvider::planeIndices = {
    0,1,2,
    3,4,5
};

const std::vector<VertexData> DataProvider::getPlaneVertices()
{
    return planeVertices;
}

const std::vector<unsigned int> DataProvider::getPlaneIndices()
{
    return planeIndices;
}

const unsigned int DataProvider::getPlaneIndiceSize()
{
    return planeIndices.size();
}

const std::vector<VertexData> DataProvider::getQuadVertices()
{
    return std::vector<VertexData>{
        // positions                        // normals                      // texture Coords
        { glm::vec3(-1.0f, 1.0f, 0.0f),     glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-1.0f, -1.0f, 0.0f),    glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(1.0f, -1.0f, 0.0f),     glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(-1.0f, 1.0f, 0.0f),     glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(1.0f, -1.0f, 0.0f),     glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(1.0f,  1.0f, 0.0f),     glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) }, };
}





const std::vector<VertexData> DataProvider::cubeVertices = {
    // positions                        // normals                      // texture Coords
    // 
    // Back face
    {glm::vec3(-0.5f, -0.5f, -0.5f),    glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 0.0f)},// bottom-left
    {glm::vec3(0.5f,  0.5f, -0.5f),     glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 1.0f)},// top-right
    {glm::vec3(0.5f, -0.5f, -0.5f),     glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 0.0f)},// bottom-right
    {glm::vec3(0.5f,  0.5f, -0.5f),     glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 1.0f)},// top-right
    {glm::vec3(-0.5f, -0.5f, -0.5f),    glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 0.0f)},// bottom-left
    {glm::vec3(-0.5f,  0.5f, -0.5f),    glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 1.0f)},// top-left
     // Front face
    {glm::vec3(-0.5f, -0.5f,  0.5f),    glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 0.0f)},// bottom-left
    {glm::vec3(0.5f, -0.5f,  0.5f),     glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 0.0f)},// bottom-right
    {glm::vec3(0.5f,  0.5f,  0.5f),     glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 1.0f)},// top-right
    {glm::vec3(0.5f,  0.5f,  0.5f),     glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 1.0f)},// top-right
    {glm::vec3(-0.5f,  0.5f,  0.5f),    glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 1.0f)}, // top-left
    {glm::vec3(-0.5f, -0.5f,  0.5f),    glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 0.0f)},// bottom-left
    // Left face
    {glm::vec3(-0.5f,  0.5f,  0.5f),    glm::vec3(-1.0f, 0.0f, 0.0),    glm::vec2(1.0f, 0.0f)},
    {glm::vec3(-0.5f,  0.5f, -0.5f),    glm::vec3(-1.0f, 0.0f, 0.0),    glm::vec2(1.0f, 1.0f)},
    {glm::vec3(-0.5f, -0.5f, -0.5f),    glm::vec3(-1.0f, 0.0f, 0.0),    glm::vec2(0.0f, 1.0f)},
    {glm::vec3(-0.5f, -0.5f, -0.5f),    glm::vec3(-1.0f, 0.0f, 0.0),    glm::vec2(0.0f, 1.0f)},
    {glm::vec3(-0.5f, -0.5f,  0.5f),    glm::vec3(-1.0f, 0.0f, 0.0),    glm::vec2(0.0f, 0.0f)},
    {glm::vec3(-0.5f,  0.5f,  0.5f),    glm::vec3(-1.0f, 0.0f, 0.0),    glm::vec2(1.0f, 0.0f)},
    // Right face
    {glm::vec3(0.5f,  0.5f,  0.5f),     glm::vec3(1.0f, 0.0f, 0.0),     glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f, -0.5f, -0.5f),     glm::vec3(1.0f, 0.0f, 0.0),     glm::vec2(0.0f, 1.0f)},
    {glm::vec3(0.5f,  0.5f, -0.5f),     glm::vec3(1.0f, 0.0f, 0.0),     glm::vec2(1.0f, 1.0f)},
    {glm::vec3(0.5f, -0.5f, -0.5f),     glm::vec3(1.0f, 0.0f, 0.0),     glm::vec2(0.0f, 1.0f)},
    {glm::vec3(0.5f,  0.5f,  0.5f),     glm::vec3(1.0f, 0.0f, 0.0),     glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f, -0.5f,  0.5f),     glm::vec3(1.0f, 0.0f, 0.0),     glm::vec2(0.0f, 0.0f)},
    // Bottom face
    {glm::vec3(-0.5f, -0.5f, -0.5f),    glm::vec3(0.0f, -1.0f, 0.0),    glm::vec2(0.0f, 1.0f)},
    {glm::vec3(0.5f, -0.5f, -0.5f),     glm::vec3(0.0f, -1.0f, 0.0),    glm::vec2(1.0f, 1.0f)},
    {glm::vec3(0.5f, -0.5f,  0.5f),     glm::vec3(0.0f, -1.0f, 0.0),    glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f, -0.5f,  0.5f),     glm::vec3(0.0f, -1.0f, 0.0),    glm::vec2(1.0f, 0.0f)},
    {glm::vec3(-0.5f, -0.5f,  0.5f),    glm::vec3(0.0f, -1.0f, 0.0),    glm::vec2(0.0f, 0.0f)},
    {glm::vec3(-0.5f, -0.5f, -0.5f),    glm::vec3(0.0f, -1.0f, 0.0),    glm::vec2(0.0f, 1.0f)},
    // Top face
    {glm::vec3(-0.5f,  0.5f, -0.5f),    glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 1.0f)},
    {glm::vec3(0.5f,  0.5f,  0.5f),     glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f,  0.5f, -0.5f),     glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 1.0f)},
    {glm::vec3(0.5f,  0.5f,  0.5f),     glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 0.0f)},
    {glm::vec3(-0.5f,  0.5f, -0.5f),    glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 1.0f)}, 
    {glm::vec3(-0.5f,  0.5f,  0.5f),    glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 0.0f)},
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

const std::vector<VertexData> DataProvider::getCubeVertices() { return cubeVertices; };
const std::vector<unsigned int> DataProvider::getCubeIndices() { return cubeIndices; }

const unsigned int DataProvider::getCubeIndiceSize()
{
    return cubeIndices.size();
}
