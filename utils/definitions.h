#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <unordered_map>
#include <map>

// application settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

struct Entity {
    // entity scaling
    glm::vec3 Scale;
    // entity rotation
    glm::vec3 Rotation;
    // entity position
    glm::vec3 Position;
    
};

struct VertexData {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
};

enum class TextureType {
    DIFFUSE,
    SPECULAR,
    NORMAL
};

enum ModelName {
    NO_MODEL,
    SHIP_MODEL,
    ROCK_MODEL,
    CRATE_MODEL
};

static std::string getModelIdentifier(ModelName _modelName)
{
    switch (_modelName)
    {
        case ModelName::NO_MODEL:
            return "no_model";
        case ModelName::SHIP_MODEL:
            return "standard_ship";
        case ModelName::ROCK_MODEL:
            return "standard_rock";
        case ModelName::CRATE_MODEL:
            return "standard_crate";
    }
}

template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}


enum class VaoReference {
    NO_VAO,
    SHIP_VAO,
    ROCK_VAO,
    CUBE_VAO
};

enum class ShaderReference {
    NO_SHADER,
    STANDARD_SHADER
};





#endif