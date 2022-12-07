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

const std::vector<VertexData> DataProvider::generatePlaneVertices( glm::vec3 _v0, glm::vec3 _v1, glm::vec3 _v2, glm::vec3 _v3, unsigned int _div ) {
    // start benchmark
    std::string benchmarkTitle = std::string( "generatePlaneVertices(div=") + boost::lexical_cast<std::string>(_div) + ")";
    Benchmark benchmark = Benchmark( benchmarkTitle );

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

    benchmark.endBenchmark();

    return vertices;
}

const std::vector<unsigned int> DataProvider::generatePlaneIndices(unsigned int _div ) {
    // start benchmark
    std::string benchmarkTitle = std::string( "generatePlaneIndices(div=" ) + boost::lexical_cast<std::string>(_div) + ")";
    Benchmark benchmark = Benchmark( benchmarkTitle );
    
    
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

    benchmark.endBenchmark();

    return indices;
}

const std::vector<VertexData> DataProvider::generateSphereVertices( unsigned int _sectorCount, unsigned int _stackCount, float _radius ) {
    std::vector<VertexData> vertices;

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / _radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * M_PI / _sectorCount;
    float stackStep = M_PI / _stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= _stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = _radius * cosf( stackAngle );             // r * cos(u)
        z = _radius * sinf( stackAngle );              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= _sectorCount; ++j) {
            VertexData newVertex;

            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf( sectorAngle );             // r * cos(u) * cos(v)
            y = xy * sinf( sectorAngle );             // r * cos(u) * sin(v)
            newVertex.Position.x = x;
            newVertex.Position.y = y;
            newVertex.Position.z = z;

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            newVertex.Normal.x = nx;
            newVertex.Normal.y = ny;
            newVertex.Normal.z = nz;

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / _sectorCount;
            t = (float)i / _stackCount;
            newVertex.TexCoords.x = s;
            newVertex.TexCoords.y = t;

            vertices.push_back( newVertex );
        }
    }

    return vertices;
}

const std::vector<unsigned int> DataProvider::generateSphereIndices( unsigned int _sectorCount, unsigned int _stackCount ) {
    std::vector<unsigned int> indices;

    // generate CCW index list of sphere triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1
    std::vector<unsigned int> lineIndices;
    int k1, k2;
    for (int i = 0; i < _stackCount; ++i) {
        k1 = i * (_sectorCount + 1);     // beginning of current stack
        k2 = k1 + _sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < _sectorCount; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0) {
                indices.push_back( k1 );
                indices.push_back( k2 );
                indices.push_back( k1 + 1 );
            }

            // k1+1 => k2 => k2+1
            if (i != (_stackCount - 1)) {
                indices.push_back( k1 + 1 );
                indices.push_back( k2 );
                indices.push_back( k2 + 1 );
            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            lineIndices.push_back( k1 );
            lineIndices.push_back( k2 );
            if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
            {
                lineIndices.push_back( k1 );
                lineIndices.push_back( k1 + 1 );
            }
        }
    }

    return indices;
}

    // procedural capsule
const std::vector<VertexData> DataProvider::generateCapsuleVertices( unsigned int _divisions, float _height, float _radius ) {
    std::vector<VertexData> vertices;

    if (_divisions % 2 == 0) _divisions++;

    float cylinderStep = (2 * M_PI) / _divisions;
    float lengthInv = 1.0f / _radius;

    // generate the two half spheres
    std::vector<VertexData> upperCircleVerts;
    std::vector<VertexData> lowerCircleVerts;

    unsigned int cylinderVertCount = vertices.size();


    float x, y, z, xz;                              // vertex position
    float nx, ny, nz;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * M_PI / _divisions;
    float stackStep = M_PI / _divisions;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= _divisions; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xz = _radius * cosf( stackAngle );             // r * cos(u)
        y = _radius * sinf( stackAngle );              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= _divisions; ++j) {
            VertexData newVertex;

            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xz * cosf( sectorAngle );             // r * cos(u) * cos(v)
            z = xz * sinf( sectorAngle );             // r * cos(u) * sin(v)
            newVertex.Position.x = x;
            newVertex.Position.y = y;
            newVertex.Position.z = z;

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            newVertex.Normal.x = nx;
            newVertex.Normal.y = ny;
            newVertex.Normal.z = nz;

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / _divisions;
            t = (float)i / _divisions;
            newVertex.TexCoords.x = s;
            newVertex.TexCoords.y = t;

            if (i == ceil( _divisions / 2 )) {
                newVertex.Position.y += _height / 2;
                upperCircleVerts.push_back( newVertex );
                newVertex.Position.y -= _height;
                lowerCircleVerts.push_back( newVertex );
                continue;
            }
            if (i < ceil( _divisions / 2 )) {
                newVertex.Position.y += _height/2;
                upperCircleVerts.push_back( newVertex );
                continue;
            }
            if (i > ceil( _divisions / 2 )) {
                newVertex.Position.y -= _height/2;
                lowerCircleVerts.push_back( newVertex );
                continue;
            }
        }
    }

    vertices.insert(vertices.end(), upperCircleVerts.begin(), upperCircleVerts.end() );
    vertices.insert(vertices.end(), lowerCircleVerts.begin(), lowerCircleVerts.end() );

    return vertices;
}

const std::vector<unsigned int> DataProvider::generateCapsuleIndices( unsigned int _divisions ) {
    std::vector<unsigned int> indices;

    if (_divisions % 2 == 0) _divisions++;

    // generate CCW index list of sphere triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1

    std::vector<int> lineIndices;
    int k1, k2;
    for(int i = 0; i < _divisions + 1; ++i)
    {
        k1 = i * (_divisions + 1);     // beginning of current stack
        k2 = k1 + _divisions + 1;      // beginning of next stack

        for(int j = 0; j < _divisions; ++j, ++k1, ++k2)
        {

            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                indices.push_back(k2);
                indices.push_back(k1);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (_divisions))
            {
                indices.push_back(k2);
                indices.push_back(k1 + 1);
                indices.push_back(k2 + 1);
            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if(i != 0)  // horizontal lines except 1st stack, k1 => k+1
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

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
