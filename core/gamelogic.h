#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "../utils/definitions.h"
#include "../utils/camera_freefloat.h"
#include "../utils/camera_isometric.h"
#include "gamesettings.h"

#include <map>
#include <functional>

const float waterHeight = 0.0f;

struct CannonOffsets {
	float verticalOffset;
	float horizontalOffset;
};

class GameLogic {
public:
	GameLogic( GameSettings _settings );

	void setPlayer( Entity* _player );
	void setupGame();

	Camera* getCamera();
	Entity* getPlayer();
	const std::map<ModelName, std::vector<Entity*>>& getEntities() const;

	void processKeyboard( PlayerAction _action, float _deltaTime );


private:
	Camera* m_camera;
	GameSettings m_settings;

	std::map<ModelName, std::vector<Entity*>> m_entities;	// vector containing all entities in the game

	//player variables
private:
	Entity* m_player;										// pointer to the player entity
	const float playerMoveSpeed = 1.5f;
	const float playerTurnSpeed = 15.0f;
	CannonOffsets offsets_standardModel = {
		0.5f,
		0.5f
	};

	void movePlayer( float _speed, float _deltaTime );
	void turnPlayer( float _speed, float _deltaTime );

private:
	// gamestate management
	// ---------------------


	// entity management
	// -------------
	Entity* addEntity( glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation, ModelName _modelName );

	Entity* addRock( glm::vec3 _position, glm::vec3 _scale = glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3 _rotation = glm::vec3( 0.0f, 0.0f, 0.0f ) );
	Entity* addCrate( glm::vec3 _position, glm::vec3 _scale = glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3 _rotation = glm::vec3( 0.0f, 0.0f, 0.0f ) );
	Entity* addShip( glm::vec3 _position, glm::vec3 _scale = glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3 _rotation = glm::vec3( 0.0f, 0.0f, 0.0f ) );
	Entity* addWater( glm::vec3 _position, glm::vec3 _scale = glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3 _rotation = glm::vec3( 0.0f, 0.0f, 0.0f ) );
};