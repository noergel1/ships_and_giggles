#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "../utils/imgui/imgui.h"

#include "../utils/definitions.h"
#include "../utils/cooldown.h"
#include "../utils/camera_freefloat.h"
#include "../utils/camera_isometric.h"
#include "gamesettings.h"
#include "../utils/dataprovider.h"
#include "../utils/collision_detection.h"

#include <map>
#include <functional>

const float waterHeight = 0.0f;

struct CannonOffsets {
	glm::vec3 verticalOffset;
	glm::vec3 horizontalOffset;
	float heightOffset;
};

struct Cannonball {
	const Entity* emitter;
	Entity* entity;
	glm::vec3 direction;
};

struct Ship {
	Entity* entity;
	Cooldown shootCD;
};

class GameLogic {
public:
	GameLogic( GameSettings _settings );

	void tick( float _deltaTime );

	void setupGame();
	void restartGame();

	Camera* getCamera();
	Entity* getPlayer();
	const std::map<ModelName, std::vector<Entity*>>& getEntities() const;
	const std::map<ModelName, ModelCollider>& getColliders() const;

	void processKeyboard( PlayerAction _action, float _deltaTime );


private:
	Camera* m_camera;
	GameSettings m_settings;

	std::map<ModelName, std::vector<Entity*>> m_entities;	// vector containing all entities in the game

	//player
private:
	Ship m_player;										// pointer to the player entity
	const float playerMoveSpeed = 1.5f;
	const float playerTurnSpeed = 45.0f;

	void movePlayer( float _speed, float _deltaTime );
	void turnPlayer( float _speed, float _deltaTime );

	//enemies
private:
	void enemiesShoot(float _deltaTime);
	void enemiesTurn(float _deltaTime);

	std::vector<Ship> enemyShips;

	//projectiles
private:
	void shipShoot(Ship* _emitter);

private:
	const float cannonballScale = 0.02f;
	const float cannonballSpeed = 1.5f;
	std::vector<Cannonball> cannonballs;
	CannonOffsets offsets_standardModel = {
		glm::vec3( 0.0f, 0.0f, 0.18f ),
		glm::vec3( 0.045f, 0.0f, 0.0f ),
		0.075f
	};


	void moveCannonballs( float _deltaTime );

private:
	// gamestate management
	// ---------------------
	float lastGameStart = 5.0f;

	float level_xBoundary = 20.0f;
	float level_yBoundary = 20.0f;
	float level_zBoundary = 20.0f;

	// entity management
	// -------------
	Entity* addEntity( glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation, ModelName _modelName );

	Entity* addRock( glm::vec3 _position, glm::vec3 _scale = glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3 _rotation = glm::vec3( 0.0f, 0.0f, 0.0f ) );
	Entity* addCannonball( glm::vec3 _position, glm::vec3 _scale = glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3 _rotation = glm::vec3( 0.0f, 0.0f, 0.0f ) );
	Entity* addCrate( glm::vec3 _position, glm::vec3 _scale = glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3 _rotation = glm::vec3( 0.0f, 0.0f, 0.0f ) );
	Entity* addShip( glm::vec3 _position, glm::vec3 _scale = glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3 _rotation = glm::vec3( 0.0f, 0.0f, 0.0f ) );
	Entity* addWater( glm::vec3 _position, glm::vec3 _scale = glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3 _rotation = glm::vec3( 0.0f, 0.0f, 0.0f ) );


// colliders
public:
	std::vector<VertexData> getShipColliderVertices();
	std::vector<unsigned int> getShipColliderIndices();

private:
	Collision checkCollision( ModelName _model1, Entity _entity1, ModelName _model2, Entity _entity2 );
	void processCollisions();


	// ship collider
	float shipCapsuleRadius = 1.1f;
	float shipCapsuleLength = 6.3f;

	std::map<ModelName, ModelCollider> m_modelColliders = {
						{ModelName::SHIP_STANDARD,	ModelCollider( {ColliderType::CAPSULE,
																	Entity( {
																			// position
																			glm::vec3( 0.0f, 0.25f, -0.0f ),
																			//scale
																			glm::vec3( 1.0f ),
																			//rotation
																			glm::vec3( 90.0f, 0.0f, 0.0f ) } )
																													} )},

						{ModelName::CRATE,			ModelCollider( {ColliderType::CUBE,
																	Entity( {
																				// position
																				glm::vec3( 0.0f, 0.0f, 0.0f ),
																				//scale
																				glm::vec3( 1.0f ),
																				//rotation
																				glm::vec3( 0.0f, 0.0f, 0.0f ) } )
																														} )},

						{ModelName::CANNONBALL,		ModelCollider( {ColliderType::SPHERE,
																	Entity( {
																				// position
																				glm::vec3( 0.0f, 0.0f, 0.0f ),
																				//scale
																				glm::vec3( cannonballScale ),
																				//rotation
																				glm::vec3( 0.0f, 0.0f, 0.0f ) } )
																													} )},




	};

//debugging
public:
	void fillImGui();

private:
	Entity* testShip1;
	Entity* testShip2;
	Entity* testCannonball1;
};