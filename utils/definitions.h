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

// A list of all models in the game
enum class ModelName {
    NO_MODEL,
    SHIP_MODEL,
    ROCK_MODEL,
    CRATE_MODEL
};

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

template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}




enum class ShaderReference {
    NO_SHADER,
    STANDARD_SHADER
};





#endif