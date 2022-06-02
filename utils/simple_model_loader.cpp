#include "simple_model_loader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <boost/spirit/home/x3.hpp>


bool ModelLoader::loadModel(std::string _modelPath, std::vector<VertexData>* _vertices, std::vector<unsigned int>* _indices, std::vector<Texture>* _textures, glm::mat4 _preTransform)
{
    namespace x3 = boost::spirit::x3;


    int lineNumber = 0;
    std::string folderName = _modelPath.substr(0, _modelPath.find_last_of('/'));
    std::string mtlName = _modelPath.substr(0, _modelPath.length() - 4) + ".mtl";

    std::ifstream objFile(_modelPath);
    if (objFile.is_open()) {
        std::string line;

        std::vector<glm::vec3> positionData;
        std::vector<glm::vec2> textureData;
        std::vector<glm::vec3> normalData;

        int indexCounter = 0;

        while (std::getline(objFile, line)) {
            // using printf() in all tests for consistency
            //printf("%s\n", line.c_str());
            lineNumber++;


            // regex for testing which type of data it is
            std::regex positionRegex("v (.*)");
            std::regex texCoordRegex("vt (.*)");
            std::regex normalRegex("vn (.*)");
            std::regex indiceRegex("f (.*)");


            std::string curLineElements[3];
            std::string token;
            char delimiter = ' ';

            // populate position vector
            if (std::regex_match(line, positionRegex))
            {
                std::stringstream ssin(line);
                int i = 0;
                while (getline(ssin, token, delimiter))
                {
                    if (token == "v") continue;

                    curLineElements[i] = token;
                    i++;
                }

                glm::vec3 position = { std::stod(curLineElements[0]), std::stod(curLineElements[1]), std::stod(curLineElements[2]) };
                positionData.push_back(position);

                continue;
            }

            //populate texture coordinate vector
            if (std::regex_match(line, texCoordRegex))
            {
                std::stringstream ssin(line);
                int i = 0;
                while (getline(ssin, token, delimiter))
                {
                    if (token == "vt") continue;

                    curLineElements[i] = token;
                    i++;
                }

                glm::vec2 texCoord = { std::stod(curLineElements[0]), std::stod(curLineElements[1]) };
                textureData.push_back(texCoord);

                continue;
            }

            //populate normals vector
            if (std::regex_match(line, normalRegex))
            {
                std::stringstream ssin(line);
                int i = 0;
                while (getline(ssin, token, delimiter))
                {
                    if (token == "vn") continue;

                    curLineElements[i] = token;
                    i++;
                }

                glm::vec3 normal = { std::stod(curLineElements[0]), std::stod(curLineElements[1]), std::stod(curLineElements[2]) };
                normalData.push_back(normal);

                continue;
            }

            // create vertices and indices
            // since blender didnt correctly triangulate my ship model, i had to look for quad faces and interpreted them as triangles instead
            if (std::regex_match(line, indiceRegex))
            {
                std::stringstream ssin(line);

                int vertexPerLine = 0;
                while (getline(ssin, token, delimiter))
                {
                    if (token == "f" || vertexPerLine > 2)
                    {
                        // if (vertexPerLine > 2) std::cout << "faulty vertex" << std::endl;
                        vertexPerLine = 0;
                        continue;
                    }

                    std::stringstream ssinIndices(token);
                    std::string curIndex;

                    int vertexIndices[3];

                    int i = 0;
                    while (getline(ssinIndices, curIndex, '/'))
                    {
                        vertexIndices[i] = std::stoi(curIndex);
                        i++;
                    }

                    VertexData newVertex = {
                        positionData[vertexIndices[0] - 1],
                        normalData[vertexIndices[2] - 1],
                        textureData[vertexIndices[1] - 1]
                    };

                    _vertices->push_back(newVertex);
                    _indices->push_back(indexCounter);

                    vertexPerLine++;
                    indexCounter++;
                }

                if (_vertices->size() % 3 != 0) std::cout << "vertices wrong at line : " << lineNumber << std::endl;
                continue;
            }



        }
        objFile.close();

    }
    else
    {
        std::cout << "Modelfile couldn't be opened : " << _modelPath << std::endl;
        return false;
    }


    //std::string filename = "ressources/ship_2cannons";
    std::ifstream mtlFile(mtlName);
    if (mtlFile.is_open()) {
        std::string line;

        std::regex diffuseMapRegex("map_Kd (.*)");

        std::string token;
        char delimiter = ' ';

        while (std::getline(mtlFile, line)) {
            if (std::regex_match(line, diffuseMapRegex))
            {
                std::stringstream ssin(line);
                int i = 0;
                while (getline(ssin, token, delimiter))
                {
                    if (token == "map_Kd") continue;

                    std::string textureLocation = folderName + "/" + token;
                    _textures->push_back(Texture(textureLocation.c_str(), false, TextureType::DIFFUSE));
                }
            }
        }
    }
    else
    {
        std::cout << "Materialfile couldn't be opened : " << mtlName << std::endl;
        return false;
    }

    return false;
}

bool ModelLoader::loadModel_x3(std::string _modelPath, std::vector<VertexData>* _vertices, std::vector<unsigned int>* _indices, std::vector<Texture>* _textures, glm::mat4 _preTransform)
{
    namespace x3 = boost::spirit::x3;


    int lineNumber = 0;
    std::string folderName = _modelPath.substr(0, _modelPath.find_last_of('/'));
    std::string mtlName = _modelPath.substr(0, _modelPath.length() - 4) + ".mtl";

    std::ifstream objFile(_modelPath);
    if (objFile.is_open()) {
        std::string line;

        std::vector<glm::vec3> positionData;
        std::vector<glm::vec2> textureData;
        std::vector<glm::vec3> normalData;

        auto iter = objFile._Seekbeg;
        auto iter_end = objFile._Seekend;

        int indexCounter = 0;

        while (std::getline(objFile, line)) {
            // using printf() in all tests for consistency
            //printf("%s\n", line.c_str());
            lineNumber++;


            // regex for testing which type of data it is
            std::regex positionRegex("v (.*)");
            std::regex texCoordRegex("vt (.*)");
            std::regex normalRegex("vn (.*)");
            std::regex indiceRegex("f (.*)");


            std::string curLineElements[3];
            std::string token;
            char delimiter = ' ';

            // populate position vector
            if (std::regex_match(line, positionRegex))
            {
                std::stringstream ssin(line);
                int i = 0;
                while (getline(ssin, token, delimiter))
                {
                    if (token == "v") continue;

                    curLineElements[i] = token;
                    i++;
                }

                glm::vec3 position = { std::stod(curLineElements[0]), std::stod(curLineElements[1]), std::stod(curLineElements[2]) };
                positionData.push_back(position);

                continue;
            }

            //populate texture coordinate vector
            if (std::regex_match(line, texCoordRegex))
            {
                std::stringstream ssin(line);
                int i = 0;
                while (getline(ssin, token, delimiter))
                {
                    if (token == "vt") continue;

                    curLineElements[i] = token;
                    i++;
                }

                glm::vec2 texCoord = { std::stod(curLineElements[0]), std::stod(curLineElements[1]) };
                textureData.push_back(texCoord);

                continue;
            }

            //populate normals vector
            if (std::regex_match(line, normalRegex))
            {
                std::stringstream ssin(line);
                int i = 0;
                while (getline(ssin, token, delimiter))
                {
                    if (token == "vn") continue;

                    curLineElements[i] = token;
                    i++;
                }

                glm::vec3 normal = { std::stod(curLineElements[0]), std::stod(curLineElements[1]), std::stod(curLineElements[2]) };
                normalData.push_back(normal);

                continue;
            }

            // create vertices and indices
            // since blender didnt correctly triangulate my ship model, i had to look for quad faces and interpreted them as triangles instead
            if (std::regex_match(line, indiceRegex))
            {
                std::stringstream ssin(line);

                int vertexPerLine = 0;
                while (getline(ssin, token, delimiter))
                {
                    if (token == "f" || vertexPerLine > 2)
                    {
                        // if (vertexPerLine > 2) std::cout << "faulty vertex" << std::endl;
                        vertexPerLine = 0;
                        continue;
                    }

                    std::stringstream ssinIndices(token);
                    std::string curIndex;

                    int vertexIndices[3];

                    int i = 0;
                    while (getline(ssinIndices, curIndex, '/'))
                    {
                        vertexIndices[i] = std::stoi(curIndex);
                        i++;
                    }

                    VertexData newVertex = {
                        positionData[vertexIndices[0] - 1],
                        normalData[vertexIndices[2] - 1],
                        textureData[vertexIndices[1] - 1]
                    };

                    _vertices->push_back(newVertex);
                    _indices->push_back(indexCounter);

                    vertexPerLine++;
                    indexCounter++;
                }

                if (_vertices->size() % 3 != 0) std::cout << "vertices wrong at line : " << lineNumber << std::endl;
                continue;
            }



        }
        objFile.close();

    }
    else
    {
        std::cout << "Modelfile couldn't be opened : " << _modelPath << std::endl;
        return false;
    }


    //std::string filename = "ressources/ship_2cannons";
    std::ifstream mtlFile(mtlName);
    if (mtlFile.is_open()) {
        std::string line;

        std::regex diffuseMapRegex("map_Kd (.*)");

        std::string token;
        char delimiter = ' ';

        while (std::getline(mtlFile, line)) {
            if (std::regex_match(line, diffuseMapRegex))
            {
                std::stringstream ssin(line);
                int i = 0;
                while (getline(ssin, token, delimiter))
                {
                    if (token == "map_Kd") continue;

                    std::string textureLocation = folderName + "/" + token;
                    _textures->push_back(Texture(textureLocation.c_str(), false, TextureType::DIFFUSE));
                }
            }
        }
    }
    else
    {
        std::cout << "Materialfile couldn't be opened : " << mtlName << std::endl;
        return false;
    }

    return false;
}

//bool ModelLoader::loadModel(std::string _modelPath, std::vector<VertexData>* _vertices, std::vector<unsigned int>* _indices, std::vector<Texture>* _textures, glm::mat4 _preTransform)
//{
//    tinyobj::attrib_t attributes;
//    std::vector<tinyobj::shape_t> shapes;
//    std::vector<tinyobj::material_t> materials;
//    std::string warning, error;
//
//    if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, _modelPath.c_str()))
//    {
//        std::cout << warning << error << "\n";
//    }
//
//    for (const auto& shape : shapes)
//    {
//        for (const auto& index : shape.mesh.material_ids)
//        {
//            _textures->push_back(Texture(materials[index]));
//        }
//
//        for (const auto& index : shape.mesh.indices) {
//
//            glm::vec4 pos = {
//                attributes.vertices[3 * index.vertex_index],
//                attributes.vertices[3 * index.vertex_index + 1],
//                attributes.vertices[3 * index.vertex_index + 2],
//                1.0f
//            };
//
//            pos = _preTransform * pos;
//
//            glm::vec3 normal = {
//                attributes.normals[3 * index.normal_index],
//                attributes.normals[3 * index.normal_index + 1],
//                attributes.normals[3 * index.normal_index + 2],
//            };
//
//            normal = glm::normalize( glm::mat3(_preTransform) * normal );
//
//            glm::vec2 texCoord = {
//                attributes.texcoords[3 * index.texcoord_index],
//                attributes.texcoords[3 * index.texcoord_index + 1],
//            };
//
//            VertexData vertex{  glm::vec3(pos.x, pos.y, pos.z), 
//                                glm::vec3(normal.x, normal.y, normal.z), 
//                                glm::vec2(texCoord.x, texCoord.y)           };
//        }
//    }
//
//    return true;
//}

bool ModelLoader::createModelSimplification(std::string _modelPath, std::vector<VertexData>* _vertices, std::vector<unsigned int>* _indices)
{
    return false;
}

bool ModelLoader::checkForModelSimplification(std::string _modelPath, std::vector<VertexData>* _vertices, std::vector<unsigned int>* _indices)
{
    return false;
}
