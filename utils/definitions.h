#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <unordered_map>
#include <map>


struct Entity {
    glm::vec3 Position;
    glm::vec3 Scale;
    glm::vec3 Rotation;
};

// defines possible inputs of player
enum class PlayerAction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    SHOOT
};

enum class ColliderType {
    CAPSULE,
    CUBE,
    SPHERE
};

struct ModelCollider {
    ColliderType colliderType;
    Entity diffToModel;			// defines the deviation from the models entity
};

// A list of all models in the game
enum class ModelName {
    //models are rendered top->bottom
    NO_MODEL,
    SHIP_STANDARD,
    CANNON,
    WALL,
    CRATE,
    ROCK,
    CANNONBALL,
    WATER,
    SKYBOX,
    TEST_OBJECT,
    POSTPROCESSING,
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