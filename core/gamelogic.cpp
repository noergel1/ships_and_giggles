#include "gamelogic.h"



GameLogic::GameLogic( GameSettings _settings )
    :m_settings(_settings)
{
    if (m_settings.CAM_MODE == Camera_Mode::FREE_FLOAT) {
        m_camera = new Camera_FreeFloat( glm::vec3( 0.0f, 3.0f, 3.0f ) );
    }
    else {
        m_player = addShip(
                // position
            glm::vec3( 0.0f, 0.0f, 0.0f ),
            //scale
            glm::vec3( 0.5f ),
            //rotation
            glm::vec3( 0.0f, 0.0f, 0.0f )
        );

        m_camera = new Camera_Isometric( m_player );
    }
}

void GameLogic::setPlayer( Entity* _player ) {
}

Entity* GameLogic::getPlayer() {
    return m_player;
}

void GameLogic::setupGame() {
        // generate objects
    addCrate(
        // position
        glm::vec3( 3.0f, 0.0f, 0.0f ),
        //scale
        glm::vec3( 1.0f ),
        //rotation
        glm::vec3( 0.0f, 0.0f, 0.0f )
    );

    addWater(
        // position
        glm::vec3( 0.0f, waterHeight, 0.0f ),
        //scale
        glm::vec3( 50.0f ),
        //rotation
        glm::vec3( 0.0f, 45.0f, 0.0f )
    );

    addShip(
                    // position
        glm::vec3( 4.0f, 0.0f, 0.0f ),
        //scale
        glm::vec3( 0.5f ),
        //rotation
        glm::vec3( 0.0f, 0.0f, 0.0f )
    );

    addShip(
                // position
        glm::vec3( -4.0f, 0.0f, 0.0f ),
        //scale
        glm::vec3( 0.5f ),
        //rotation
        glm::vec3( 0.0f, 0.0f, 0.0f )
    );

        //skybox
    addEntity(
        // position
        glm::vec3( 0.0f, 0.0f, 0.0f ),
        //scale
        glm::vec3( 1.0f ),
        //rotation
        glm::vec3( 0.0f ),
        //modelname
        ModelName::SKYBOX
    );
}

Camera* GameLogic::getCamera() {
    return m_camera;
}

const std::map<ModelName, std::vector<Entity*>>& GameLogic::getEntities() const{
    return m_entities;
}

void GameLogic::processKeyboard( PlayerAction _action, float _deltaTime ) {
    
    if (m_settings.CAM_MODE == Camera_Mode::FREE_FLOAT) {
        switch (_action) {
            case FORWARD:
            {
                m_camera->ProcessKeyboard( FORWARD, _deltaTime );
                break;
            }
            case BACKWARD:
            {
                m_camera->ProcessKeyboard( BACKWARD, _deltaTime );
                break;
            }
            case LEFT:
            {
                m_camera->ProcessKeyboard( LEFT, _deltaTime );
                break;
            }
            case RIGHT:
            {
                m_camera->ProcessKeyboard( RIGHT, _deltaTime );
                break;
            }
        }
    }
    else {
        switch (_action) {
            case FORWARD:
            {
                movePlayer( playerMoveSpeed, _deltaTime );
                m_camera->updateCameraVectors();
                break;
            }
            case BACKWARD:
            {
                movePlayer( -playerMoveSpeed, _deltaTime );
                m_camera->updateCameraVectors();
                break;
            }
            case LEFT:
            {
                turnPlayer( playerTurnSpeed, _deltaTime );
                m_camera->updateCameraVectors();
                break;
            }
            case RIGHT:
            {
                turnPlayer( -playerTurnSpeed, _deltaTime );
                m_camera->updateCameraVectors();
                break;
            }
        }
    }

}


void GameLogic::movePlayer( float _speed, float _deltaTime ) {
    float playerRotation = m_player->Rotation.y;
    glm::vec3 initialForwardDir = glm::vec3( 0.0f, 0.0f, 1.0f );
    glm::mat3 rotationMat = glm::rotate( glm::mat4(1.0f), glm::radians(playerRotation), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 forwardVec = rotationMat * initialForwardDir;

    m_player->Position += (glm::normalize(forwardVec) * _speed * _deltaTime);

    m_camera->ProcessKeyboard( FORWARD, _deltaTime );
    m_camera->updateCameraVectors();
}

void GameLogic::turnPlayer( float _speed, float _deltaTime ) {
    m_player->Rotation.y += _speed * _deltaTime;
}

Entity* GameLogic::addEntity( glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation, ModelName _modelName ) {

    Entity* newEntity = new Entity {
        // position
        _position,
        //scale
        _scale,
        //rotation
        _rotation,
    };

    m_entities[_modelName].push_back( newEntity );

    return newEntity;
}

Entity* GameLogic::addRock( glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation ) {
    Entity* entityPtr = addEntity( _position, _scale, _rotation, ModelName::ROCK );
    return entityPtr;
}

Entity* GameLogic::addCrate( glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation ) {
    Entity* entityPtr = addEntity( _position, _scale, _rotation, ModelName::CRATE );
    return entityPtr;
}

Entity* GameLogic::addShip( glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation ) {
    Entity* entityPtr = addEntity( _position, _scale, _rotation, ModelName::SHIP_STANDARD );
    return entityPtr;
}

Entity* GameLogic::addWater( glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation ) {
    Entity* entityPtr = addEntity( _position, _scale, _rotation, ModelName::WATER );
    return entityPtr;
}