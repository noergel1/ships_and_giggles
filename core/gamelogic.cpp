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
            glm::vec3( 0.1f ),
            //rotation
            glm::vec3( 0.0f, 0.0f, 0.0f )
        );

        m_camera = new Camera_Isometric( m_player );
    }
}

void GameLogic::tick( float _deltaTime ) {
    moveCannonballs( _deltaTime );

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

    testShip1 = addShip(
        // position
        glm::vec3( 4.0f, 0.0f, 0.0f ),
        //scale
        glm::vec3( 0.5f ),
        //rotation
        glm::vec3( 0.0f, 0.0f, 0.0f )
    );

    testShip2 = addShip(
        // position
        glm::vec3( -4.0f, 0.0f, 0.0f ),
        //scale
        glm::vec3( 0.5f ),
        //rotation
        glm::vec3( 0.0f, 0.0f, 0.0f )
    );

    //        //skybox
    //addEntity(
    //    // position
    //    glm::vec3( 0.0f, 1.0f, 0.0f ),
    //    //scale
    //    glm::vec3( 1.0f ),
    //    //rotation
    //    glm::vec3( 90.0f, 0.0f, 0.0f ),
    //    //modelname
    //    ModelName::TEST_OBJECT
    //);

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

const std::map<ModelName, ModelCollider>& GameLogic::getColliders() const {
    return m_modelColliders;
}

void GameLogic::processKeyboard( PlayerAction _action, float _deltaTime ) {
    
    if (m_settings.CAM_MODE == Camera_Mode::FREE_FLOAT) {
        switch (_action) {
            case PlayerAction::FORWARD:
            {
                m_camera->ProcessKeyboard( PlayerAction::FORWARD, _deltaTime );
                break;
            }
            case PlayerAction::BACKWARD:
            {
                m_camera->ProcessKeyboard( PlayerAction::BACKWARD, _deltaTime );
                break;
            }
            case PlayerAction::LEFT:
            {
                m_camera->ProcessKeyboard( PlayerAction::LEFT, _deltaTime );
                break;
            }
            case PlayerAction::RIGHT:
            {
                m_camera->ProcessKeyboard( PlayerAction::RIGHT, _deltaTime );
                break;
            }
        }
    }
    else {
        switch (_action) {
            case PlayerAction::FORWARD:
            {
                movePlayer( playerMoveSpeed, _deltaTime );
                m_camera->updateCameraVectors();
                break;
            }
            case PlayerAction::BACKWARD:
            {
                movePlayer( -playerMoveSpeed, _deltaTime );
                m_camera->updateCameraVectors();
                break;
            }
            case PlayerAction::LEFT:
            {
                turnPlayer( playerTurnSpeed, _deltaTime );
                m_camera->updateCameraVectors();
                break;
            }
            case PlayerAction::RIGHT:
            {
                turnPlayer( -playerTurnSpeed, _deltaTime );
                m_camera->updateCameraVectors();
                break;
            }
            case PlayerAction::SHOOT:
            {
                if (!playerCooldowns[PlayerAction::SHOOT].isOnCooldown())                     {
                    playerShoot();
                    playerCooldowns[PlayerAction::SHOOT].startCooldown();
                }
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

    m_camera->ProcessKeyboard( PlayerAction::FORWARD, _deltaTime );
    m_camera->updateCameraVectors();
}

void GameLogic::turnPlayer( float _speed, float _deltaTime ) {
    m_player->Rotation.y += _speed * _deltaTime;
}

void GameLogic::playerShoot() {
                glm::vec3 playerPosition = m_player->Position;
                float playerRotation = m_player->Rotation.y;
                glm::mat3 rotationMat = glm::rotate( glm::mat4( 1.0f ), glm::radians( playerRotation ), glm::vec3( 0.0f, 1.0f, 0.0f ) );

                glm::vec3 vecToLeft = rotationMat *  glm::vec3(  1.0f, 0.0f, 0.0f );
                glm::vec3 vecToRight = rotationMat * glm::vec3( -1.0f, 0.0f, 0.0f );

                glm::vec3 posUpperRightBall = rotatePointAroundCenter( playerPosition+offsets_standardModel.horizontalOffset+offsets_standardModel.verticalOffset, playerPosition, rotationMat );
                glm::vec3 posUpperLeftBall = rotatePointAroundCenter( playerPosition-offsets_standardModel.horizontalOffset+offsets_standardModel.verticalOffset, playerPosition, rotationMat );
                glm::vec3 posLowerRightBall = rotatePointAroundCenter( playerPosition+offsets_standardModel.horizontalOffset-offsets_standardModel.verticalOffset, playerPosition, rotationMat );
                glm::vec3 posLowerLeftBall = rotatePointAroundCenter( playerPosition-offsets_standardModel.horizontalOffset-offsets_standardModel.verticalOffset, playerPosition, rotationMat );
                float heightOffset = offsets_standardModel.heightOffset;

                player_Cannonballs.push_back( Cannonball{
                    addCannonball( posUpperLeftBall, glm::vec3(cannonballScale) ),
                    vecToLeft
                } );

                player_Cannonballs.push_back( Cannonball{
                    addCannonball( posLowerLeftBall, glm::vec3(cannonballScale) ),
                    vecToLeft
                } );

                player_Cannonballs.push_back( Cannonball{
                    addCannonball( posUpperRightBall, glm::vec3(cannonballScale) ),
                    vecToRight
                } );

                player_Cannonballs.push_back( Cannonball{
                    addCannonball( posLowerRightBall, glm::vec3(cannonballScale) ),
                    vecToRight
                } );
}

void GameLogic::moveCannonballs( float _deltaTime ) {
    for (int i = 0; i < player_Cannonballs.size(); i++)         {
        Entity* curEntity = player_Cannonballs[i].entity;
        glm::vec3 curForwardVec = player_Cannonballs[i].direction;

        curEntity->Position += curForwardVec * cannonballSpeed * _deltaTime;
    }
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

Entity* GameLogic::addCannonball( glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation ) {
    Entity* entityPtr = addEntity( _position, _scale, _rotation, ModelName::CANNONBALL );
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

// collider
std::vector<VertexData> GameLogic::getShipColliderVertices() {
        std::vector<VertexData> vertices = DataProvider::generateCapsuleVertices( 10, shipCapsuleLength, shipCapsuleRadius );
        return vertices;
};

std::vector<unsigned int> GameLogic::getShipColliderIndices() {
    return DataProvider::generateCapsuleIndices( 10 );
};

Collision GameLogic::checkCollision( ModelName _model1, Entity _entity1, ModelName _model2, Entity _entity2 ) {
    Collision collision;


    return collision;
};

void GameLogic::processCollisions() {

};


// debuggin
void GameLogic::fillImGui() {
    ImGui::Text( "" );
    ImGui::Text( "Collision Testing" );
    ImGui::Text( "Ship 1" );
    ImGui::SliderFloat3( "Position1", &testShip1->Position.x, -5.0f, 5.0f);
    ImGui::SliderFloat3( "Rotation1", &testShip1->Rotation.x, -180.0f, 180.0f);
    ImGui::SliderFloat3( "Scale1", &testShip1->Scale.x, 0.1f, 5.0f);
    ImGui::Text( "Ship 2" );
    ImGui::SliderFloat3( "Position2", &testShip2->Position.x, -5.0f, 5.0f);
    ImGui::SliderFloat3( "Rotation2", &testShip2->Rotation.x, -180.0f, 180.0f);
    ImGui::SliderFloat3( "Scale2", &testShip2->Scale.x, 0.1f, 5.0f);
}