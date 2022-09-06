#include "gamelogic.h"



GameLogic::GameLogic( GameSettings _settings )
    :m_settings(_settings)
{
    if (m_settings.CAM_MODE == Camera_Mode::FREE_FLOAT) {
        m_camera = new Camera_FreeFloat( glm::vec3( 0.0f, 3.0f, 3.0f ) );
    }
    else {
        lastGameStart = glfwGetTime() + 7.0f;

        Entity* playerPtr = addShip(
                // position
            glm::vec3( 0.0f, 0.0f, 0.0f ),
            //scale
            glm::vec3( 0.1f ),
            //rotation
            glm::vec3( 0.0f, 0.0f, 0.0f )
        );

        m_player = Ship( { playerPtr, Cooldown( 0.5f ) } );

        m_camera = new Camera_Isometric( playerPtr );
    }
}

void GameLogic::tick( float _deltaTime ) {
    if (m_settings.CAM_MODE == Camera_Mode::ISOMETRIC) {
        enemiesTurn( _deltaTime );

        if (glfwGetTime() > lastGameStart) enemiesShoot( _deltaTime );
        moveCannonballs( _deltaTime );
        processCollisions();
    };
}

Entity* GameLogic::getPlayer() {
    return m_player.entity;
}

void GameLogic::setupGame() {
        // generate objects
    //addCrate(
    //    // position
    //    glm::vec3( 3.0f, 0.0f, 0.0f ),
    //    //scale
    //    glm::vec3( 1.0f ),
    //    //rotation
    //    glm::vec3( 0.0f, 0.0f, 0.0f )
    //);

    addWater(
        // position
        glm::vec3( 0.0f, waterHeight, 0.0f ),
        //scale
        glm::vec3( 50.0f ),
        //rotation
        glm::vec3( 0.0f, 45.0f, 0.0f )
    );

    Entity* ship1 = addShip(
        // position
        glm::vec3( 4.0f, 0.0f, 0.0f ),
        //scale
        glm::vec3( 0.1f ),
        //rotation
        glm::vec3( 0.0f, 0.0f, 0.0f )
    );
    enemyShips.push_back(Ship({ship1, Cooldown(2.5f)}));


    Entity* ship2 = addShip(
        // position
        glm::vec3( -4.0f, 0.0f, 0.0f ),
        //scale
        glm::vec3( 0.1f ),
        //rotation
        glm::vec3( 0.0f, 0.0f, 0.0f )
    );
    enemyShips.push_back(Ship({ship2, Cooldown(2.5f)}));

    Entity* ship3 = addShip(
        // position
        glm::vec3( -3.0f, 0.0f, 3.0f ),
        //scale
        glm::vec3( 0.1f ),
        //rotation
        glm::vec3( 0.0f, 0.0f, 0.0f )
    );
    enemyShips.push_back(Ship({ship3, Cooldown(2.5f)}));

            Entity* ship4 = addShip(
        // position
        glm::vec3( 3.0f, 0.0f, 3.0f ),
        //scale
        glm::vec3( 0.1f ),
        //rotation
        glm::vec3( 0.0f, 0.0f, 0.0f )
    );
    enemyShips.push_back( Ship( { ship4, Cooldown( 2.5f ) } ) );

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
    //addEntity(
    //    // position
    //    glm::vec3( 0.0f, 0.0f, 0.0f ),
    //    //scale
    //    glm::vec3( 1.0f ),
    //    //rotation
    //    glm::vec3( 0.0f ),
    //    //modelname
    //    ModelName::WALL
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

void GameLogic::restartGame() {
    m_entities.clear();
    enemyShips.clear();
    cannonballs.clear();

    lastGameStart = glfwGetTime() + 5.0f;

    Entity* playerPtr = addShip(
        // position
        glm::vec3( 1.0f, 0.0f, 0.0f ),
        //scale
        glm::vec3( 0.1f ),
        //rotation
        glm::vec3( 0.0f, 0.0f, 0.0f )
    );

    //playerPtr = addShip(
    //    // position
    //    glm::vec3( 0.0f, 0.0f, 3.0f ),
    //    //scale
    //    glm::vec3( 0.1f ),
    //    //rotation
    //    glm::vec3( 0.0f, 0.0f, 0.0f )
    //);

    m_player = Ship( { playerPtr, Cooldown( 0.5f ) } );

    m_camera = new Camera_Isometric( playerPtr );

    setupGame();
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
                shipShoot(&m_player);
                break;
            }
        }
    }

}


void GameLogic::movePlayer( float _speed, float _deltaTime ) {
    float playerRotation = m_player.entity->Rotation.y;
    glm::vec3 initialForwardDir = glm::vec3( 0.0f, 0.0f, 1.0f );
    glm::mat3 rotationMat = glm::rotate( glm::mat4(1.0f), glm::radians(playerRotation), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 forwardVec = rotationMat * initialForwardDir;

    m_player.entity->Position += (glm::normalize(forwardVec) * _speed * _deltaTime);

    m_camera->ProcessKeyboard( PlayerAction::FORWARD, _deltaTime );
    m_camera->updateCameraVectors();
}

void GameLogic::turnPlayer( float _speed, float _deltaTime ) {
    m_player.entity->Rotation.y += _speed * _deltaTime;
}

void GameLogic::shipShoot(Ship* _emitter) {
    if (!_emitter->shootCD.isOnCooldown()) {

        glm::vec3 emitterPos = _emitter->entity->Position;
        float playerRotation = _emitter->entity->Rotation.y;
        glm::mat3 rotationMat = glm::rotate( glm::mat4( 1.0f ), glm::radians( playerRotation ), glm::vec3( 0.0f, 1.0f, 0.0f ) );

        glm::vec3 vecToLeft = rotationMat * glm::vec3( 1.0f, 0.0f, 0.0f );
        glm::vec3 vecToRight = rotationMat * glm::vec3( -1.0f, 0.0f, 0.0f );

        glm::vec3 posUpperRightBall = rotatePointAroundCenter( emitterPos + offsets_standardModel.horizontalOffset + offsets_standardModel.verticalOffset, emitterPos, rotationMat );
        glm::vec3 posUpperLeftBall = rotatePointAroundCenter( emitterPos - offsets_standardModel.horizontalOffset + offsets_standardModel.verticalOffset, emitterPos, rotationMat );
        glm::vec3 posLowerRightBall = rotatePointAroundCenter( emitterPos + offsets_standardModel.horizontalOffset - offsets_standardModel.verticalOffset, emitterPos, rotationMat );
        glm::vec3 posLowerLeftBall = rotatePointAroundCenter( emitterPos - offsets_standardModel.horizontalOffset - offsets_standardModel.verticalOffset, emitterPos, rotationMat );
        float heightOffset = offsets_standardModel.heightOffset;

        cannonballs.push_back( Cannonball{
            _emitter->entity,
            addCannonball( posUpperLeftBall, glm::vec3( cannonballScale ) ),
            vecToLeft
            } );

        cannonballs.push_back( Cannonball{
            _emitter->entity,
            addCannonball( posLowerLeftBall, glm::vec3( cannonballScale ) ),
            vecToLeft
            } );

        cannonballs.push_back( Cannonball{
            _emitter->entity,
            addCannonball( posUpperRightBall, glm::vec3( cannonballScale ) ),
            vecToRight
            } );

        cannonballs.push_back( Cannonball{
            _emitter->entity,
            addCannonball( posLowerRightBall, glm::vec3( cannonballScale ) ),
            vecToRight
            } );
        _emitter->shootCD.startCooldown();
    };
}

void GameLogic::moveCannonballs( float _deltaTime ) {
    for (int i = 0; i < cannonballs.size(); i++)         {
        Entity* curEntity = cannonballs[i].entity;
        glm::vec3 curForwardVec = cannonballs[i].direction;

        curEntity->Position += curForwardVec * cannonballSpeed * _deltaTime;
    }
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

    // check for ships colliding and move away from each other
    std::vector<Entity*> ships = m_entities[ModelName::SHIP_STANDARD];

    glm::vec3 capsuleTop = glm::vec3( 0.0f, shipCapsuleLength / 2, 0.0f );
    glm::vec3 capsuleBottom = glm::vec3( 0.0f, -shipCapsuleLength / 2, 0.0f );

    ModelCollider shipCollider = m_modelColliders[ModelName::SHIP_STANDARD];
    glm::mat4 colliderTrans = createSRTMat4(shipCollider.diffToModel);
    capsuleTop = colliderTrans * glm::vec4(capsuleTop, 1.0f);
    capsuleBottom = colliderTrans * glm::vec4(capsuleBottom, 1.0f);

        // player
    glm::mat4 playerTrans = createSRTMat4( *m_player.entity );
    glm::vec3 capsuleTopPl = playerTrans * glm::vec4(capsuleTop, 1.0f);
    glm::vec3 capsuleBottomPl = playerTrans * glm::vec4(capsuleBottom, 1.0f);

    for (int i = 0; i < ships.size(); i++) {
        if (ships[i] == m_player.entity) continue;

        // ship1 translation
        glm::mat4 otherShipTrans = createSRTMat4( *ships[i]);
        glm::vec3 otherCapsuleTop = otherShipTrans * glm::vec4(capsuleTop, 1.0f);
        glm::vec3 otherCapsuleBottom = otherShipTrans * glm::vec4(capsuleBottom, 1.0f);

            Collision collision = CollisionDetection::CapsuleToCapsule(capsuleTopPl, capsuleBottomPl, shipCapsuleRadius * m_player.entity->Scale.x, otherCapsuleTop, otherCapsuleBottom, shipCapsuleRadius * ships[i]->Scale.x);
            if (collision.collided) {
                std::cout << "player and ship collided" << "\n";
                std::cout << "penetration normal : " << collision.penetrationNormal.x << "|" << collision.penetrationNormal.y << "|" << collision.penetrationNormal.z << "\n";
                std::cout << "penetration depth : " << collision.penetrationDepth << "\n";

                glm::vec3 deltaVec = glm::normalize( glm::vec3( collision.penetrationNormal.x, 0.0f, collision.penetrationNormal.z ) ) * collision.penetrationDepth;
                m_player.entity->Position += deltaVec; // / 2.0f;
                //ships[i]->Position -= deltaVec / 2.0f;
            }

    }


    // check for cannonball hits
    for (int i = 0; i < cannonballs.size(); i++) {
        Cannonball curProjectile = cannonballs[i];
        std::vector<Entity*> ships = m_entities[ModelName::SHIP_STANDARD];

        for (int j = 0; j < ships.size(); j++) {
            Entity* curShip = ships[j];
            if (curProjectile.emitter == curShip) continue;
            if (curProjectile.emitter != m_player.entity && curShip != m_player.entity) continue;

            // ship1 translation
            glm::mat4 shipTrans = createSRTMat4( *ships[j]);
            glm::vec3 shipCapsuleTop = shipTrans * glm::vec4(capsuleTop, 1.0f);
            glm::vec3 shipCapsuleBottom = shipTrans * glm::vec4(capsuleBottom, 1.0f);

            Collision collision = CollisionDetection::CapsuleToSphere( shipCapsuleTop, shipCapsuleBottom, shipCapsuleRadius * curShip->Scale.x, curProjectile.entity->Position, cannonballScale );

            if (collision.collided) {
                if (ships[j] == m_player.entity) {
                    restartGame();
                    return;
                }

                for (int k = 0; k < enemyShips.size(); k++) {
                    if (ships[j] == enemyShips[k].entity) enemyShips.erase( enemyShips.begin() + k );
                };

                m_entities[ModelName::SHIP_STANDARD].erase( m_entities[ModelName::SHIP_STANDARD].begin() + j );
                ships.erase( ships.begin() + j );

            }
        }
    }
};

// enemies
void GameLogic::enemiesShoot(float _deltaTime) {
        for (int i = 0; i < enemyShips.size(); i++) {
            if (enemyShips[i].entity == m_player.entity) continue;
            shipShoot( &enemyShips[i] );
        }
}

void GameLogic::enemiesTurn(float _deltaTime) {
    glm::vec3 defaultShipRotation = glm::vec3( 0.0f, 0.0f, 1.0f );
    std::vector<Entity*> ships = m_entities[ModelName::SHIP_STANDARD];

    for (int i = 0; i < ships.size(); i++) {
        Entity* curShip = ships[i];
        if (curShip == m_player.entity) continue;


        glm::mat4 rotationMat = glm::rotate( glm::mat4( 1.0f ), glm::radians(curShip->Rotation.y), glm::vec3( 0.0f, 1.0f, 0.0f ) );
        glm::vec3 curShipRotation =  glm::normalize(rotationMat * glm::vec4(defaultShipRotation, 1.0f));

        glm::vec3 playerPos = glm::vec3( m_player.entity->Position.x, 0.0f, m_player.entity->Position.z );
        glm::vec3 otherShipPos = glm::vec3( ships[i]->Position.x, 0.0f, ships[i]->Position.z );
        glm::vec3 vecToPlayer = glm::normalize( playerPos - otherShipPos );
        float turnAngle = getAngleBetweenVectorsDeg( curShipRotation, vecToPlayer );

        //std::cout << "turnAngle: " << turnAngle << "\n";
        //std::cout << "ship["<<i<<"] rotation: " << ships[i]->Rotation.y << "\n";

        ships[i]->Rotation.y += std::fmodf(180.0f-(90.0f + turnAngle), 360.0f) ;
    }
}

// debuggin
void GameLogic::fillImGui() {
    //ImGui::Text( "" );
    //ImGui::Text( "Collision Testing" );
    //ImGui::Text( "Ship 1" );
    //ImGui::SliderFloat3( "Position1", &testShip1->Position.x, -5.0f, 5.0f);
    //ImGui::SliderFloat3( "Rotation1", &testShip1->Rotation.x, -180.0f, 180.0f);
    //ImGui::SliderFloat3( "Scale1", &testShip1->Scale.x, 0.1f, 5.0f);
    //ImGui::Text( "Ship 2" );
    //ImGui::SliderFloat3( "Position2", &testShip2->Position.x, -5.0f, 5.0f);
    //ImGui::SliderFloat3( "Rotation2", &testShip2->Rotation.x, -180.0f, 180.0f);
    //ImGui::SliderFloat3( "Scale2", &testShip2->Scale.x, 0.1f, 5.0f);
}

// entity management
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