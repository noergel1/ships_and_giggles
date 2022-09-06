#pragma once

#include "definitions.h"

struct Collision {
	bool collided = false;
	float penetrationDepth = 0.0f;
	glm::vec3 penetrationNormal = glm::vec3(0.0f);
};



class CollisionDetection {
public:
	static const glm::vec3 ClosestPointOnLineSegment( glm::vec3 A, glm::vec3 B, glm::vec3 Point );
	static const Collision SphereToSphere( glm::vec3 _center1, float _radius1, glm::vec3 _center2, float _radius2 );
	static const Collision SphereToTriangle( glm::vec3 _center, float _radius, glm::vec3 _p1, glm::vec3 _p2, glm::vec3 _p3);
	static const Collision CapsuleToCapsule( glm::vec3 top1, glm::vec3 bottom1, float radius1, glm::vec3 top2, glm::vec3 bottom2, float radius2 );
	static const Collision CapsuleToSphere( glm::vec3 capTop, glm::vec3 capBottom, float capRadius, glm::vec3 sphereCenter, float sphereRadius );
};