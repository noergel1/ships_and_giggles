#include "dataprovider.h"


const std::string DataProvider::getShipObjPath()
{
    return "ressources/ship_2cannons.obj";
};

std::vector<float> DataProvider::noPostProcessing() {

    return std::vector<float>();
}

std::vector<float> DataProvider::getSharpenKernel()
{
    return std::vector<float>{
            2, 2, 2,
            2, -15, 2,
            2, 2, 2
    };
}
std::vector<float> DataProvider::getBlurKernel()
{
    return std::vector<float>{
        1.0 / 16, 2.0 / 16, 1.0 / 16,
            2.0 / 16, 4.0 / 16, 2.0 / 16,
            1.0 / 16, 2.0 / 16, 1.0 / 16
    };
}
std::vector<float> DataProvider::getEdgeDetectionKernel()
{
    return std::vector<float>{
        1, 1, 1,
            1, -8, 1,
            1, 1, 1
    };
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

const std::vector<VertexData> DataProvider::generatePlaneVertices( glm::vec3 _v0, glm::vec3 _v1, glm::vec3 _v2, glm::vec3 _v3, unsigned int _div ) {
    //  v3-----v2
    //  |       |
    //  |       |
    //  v0-----v1
    std::vector<VertexData> vertices;
    float divFloat = float( _div );
    glm::vec3 vec03 = (_v3 - _v0) / divFloat;
    glm::vec3 vec12 = (_v2 - _v1) / divFloat;

    for (int row = 0; row < _div + 1; row++) {
        float rowFloat = float( row );
        glm::vec3 start = _v0 + vec03 * rowFloat;
        glm::vec3 end = _v1 + vec12 * rowFloat;
        glm::vec3 lineDiv = (end - start) / divFloat;

        for (int col = 0; col < _div + 1; col++) {
            float colFloat = float( col );
            glm::vec3 position = start + colFloat * lineDiv;
            glm::vec3 normal = glm::cross( glm::normalize(vec03), glm::normalize((end - start)) );
            glm::vec2 texCoords = glm::vec2( colFloat / _div, rowFloat / _div );

            VertexData vertex;
            vertex.Position = position;
            vertex.Normal = normal;
            vertex.TexCoords = texCoords;

            vertices.push_back( vertex );
        }
    }

    return vertices;
}

const std::vector<unsigned int> DataProvider::generatePlaneIndices(unsigned int _div ) {
        //  v3-----v2
        //  |       |
        //  |       |
        //  v0-----v1
 
        // moving up a row equals index+div+1
    std::vector<unsigned int> indices;

    // only div-1 iterations, since there are no vertices to connect to at the end
    for (int row = 0; row < _div; row++) {
        for (int col = 0; col < _div; col++) {

            unsigned int curIndex;
            curIndex = row * (_div + 1) + col;

            unsigned int otherIndex1, otherIndex2;
            // first triangle
            // 1--------2
            // |       /
            // |      /
            // |     /
            // |    /
            // |   /
            // |  /
            // | /
            // |/
            // C

            otherIndex1 = curIndex + _div + 1;
            otherIndex2 = curIndex + _div + 2;
            indices.push_back( curIndex );
            indices.push_back( otherIndex1 );
            indices.push_back( otherIndex2 );

            // second triangle
            //          1
            //         /|
            //        / |
            //       /  |
            //      /   |
            //     /    |
            //    /     |
            //   /      |
            //  /       |
            // C--------2

            otherIndex1 = curIndex + _div + 2;
            otherIndex2 = curIndex + 1;
            indices.push_back( curIndex );
            indices.push_back( otherIndex1 );
            indices.push_back( otherIndex2 );
        }
    }

    //int count = 0;
    //for (int i = 0; i < indices.size(); i = i + 3)         {
    //    std::cout << "Vertex " + std::to_string(count) + ":\n";
    //    std::cout << indices[i] << "\n";
    //    std::cout << indices[i+1] << "\n";
    //    std::cout << indices[i+2] << "\n";

    //    count++;
    //}

    return indices;
}

const std::vector<float> DataProvider::getQuadVertices()
{
    return std::vector<float>{
        // positions   // texCoords
        -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f
    };
}

const std::vector<unsigned int> DataProvider::getQuadIndices()
{
    return std::vector<unsigned int>({ 0,1,2,3,4,5 });
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
