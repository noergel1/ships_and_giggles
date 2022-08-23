#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <unordered_map>
#include <map>


// A list of all models in the game
enum class ModelName {
    //models are rendered top->bottom
    NO_MODEL,
    SHIP_STANDARD,
    CRATE,
    ROCK,
    WATER,
    SKYBOX,
    TEST_OBJECT,
    POSTPROCESSING,
};


struct Entity {
    // entity position
    glm::vec3 Position;
    // entity scaling
    glm::vec3 Scale;
    // entity rotation
    glm::vec3 Rotation;
    
};

struct VertexData {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
};

template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}










#endif