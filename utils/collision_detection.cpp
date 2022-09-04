#include "collision_detection.h"

const glm::vec3 CollisionDetection::ClosestPointOnLineSegment(glm::vec3 A, glm::vec3 B, glm::vec3 Point)
{
  glm::vec3 AB = B - A;
  float t = glm::dot(Point - A, AB) / dot(AB, AB);
  return A + glm::clamp(t, 0.0f, 1.0f) * AB; // saturate(t) can be written as: min((max(t, 0), 1)
}

const Collision CollisionDetection::SphereToSphere( glm::vec3 _center1, float _radius1, glm::vec3 _center2, float _radius2 ) {
	Collision collision;

	collision.penetrationNormal = _center1 - _center2;
	float len = length(collision.penetrationNormal);
	collision.penetrationNormal /= len;  // normalize
	collision.penetrationDepth = _radius1 + _radius2 - len;
	collision.collided = collision.penetrationDepth > 0;

	return collision;
}

const Collision CollisionDetection::SphereToTriangle( glm::vec3 _center, float _radius, glm::vec3 _p1, glm::vec3 _p2, glm::vec3 _p3 ) {
	Collision collision;

	glm::vec3 normal = glm::normalize(glm::cross((_p2 - _p1), (_p3 - _p1))); // plane normal
	float dist = glm::dot(_center - _p1, normal); // signed distance between sphere and plane

	if(dist < -_radius || dist > _radius)
	  return collision; // no intersection

	glm::vec3 point0 = _center - normal * dist; // projected sphere center on triangle plane
 
	// Now determine whether point0 is inside all triangle edges: 
	glm::vec3 c0 = cross(point0 - _p1, _p2 - _p1); 
	glm::vec3 c1 = cross(point0 - _p2, _p3 - _p2); 
	glm::vec3 c2 = cross(point0 - _p3, _p1 - _p3);
	bool inside = glm::dot(c0, normal) <= 0 && glm::dot(c1, normal) <= 0 && glm::dot(c2, normal) <= 0;

	// otherwhise calculate the closest point over all edges and see if it's inside sphere
	bool intersects;
	glm::vec3 point1, point2, point3;
	if (!inside) {
			float radiussq = _radius * _radius; // sphere radius squared
			// Edge 1:
			point1 = ClosestPointOnLineSegment(_p1, _p2, _center);
			glm::vec3 v1 = _center - point1;
			float distsq1 = glm::dot(v1, v1);
			intersects = distsq1 < radiussq;
 
			// Edge 2:
			point2 = ClosestPointOnLineSegment(_p2, _p3, _center);
			glm::vec3 v2 = _center - point2;
			float distsq2 = glm::dot(v2, v2);
			intersects |= distsq2 < radiussq;
 
			// Edge 3:
			point3 = ClosestPointOnLineSegment(_p3, _p1, _center);
			glm::vec3 v3 = _center - point3;
			float distsq3 = glm::dot(v3, v3);
			intersects |= distsq3 < radiussq;
	}

 


	if(inside || intersects)
	{
		glm::vec3 best_point = point0;
		glm::vec3 intersection_vec;
 
		if(inside)
		{
		intersection_vec = _center - point0;
		}
		else  
		{
		glm::vec3 d = _center - point1;
		float best_distsq = dot(d, d);
		best_point = point1;
		intersection_vec = d;
 
		d = _center - point2;
		float distsq = dot(d, d);
		if(distsq < best_distsq)
		{
			distsq = best_distsq;
			best_point = point2;
			intersection_vec = d;
		}
 
		d = _center - point3;
		distsq = dot(d, d);
		if(distsq < best_distsq)
		{
			distsq = best_distsq;
			best_point = point3; 
			intersection_vec = d;
		}
	}
 
	float len = glm::length(intersection_vec);  // vector3 length calculation: 
	//sqrt( glm::dot( v, v ) );
	collision.collided = inside || intersects;
	collision.penetrationNormal = intersection_vec / len;  // normalize
	collision.penetrationDepth = _radius - len; // radius = sphere radius
	return collision; // intersection success
	}
}

const Collision CollisionDetection::CapsuleToCapsule( glm::vec3 top1, glm::vec3 bottom1, float radius1, glm::vec3 top2, glm::vec3 bottom2, float radius2 ) {
	// capsule A:
	glm::vec3 a_Normal = glm::normalize(top1 - bottom1); 
	glm::vec3 a_LineEndOffset = a_Normal * radius1; 
 
	// capsule B:
	glm::vec3 b_Normal = glm::normalize(top2 - bottom2); 
	glm::vec3 b_LineEndOffset = b_Normal * radius2; 
 
	// vectors between line endpoints:
	glm::vec3 v0 = bottom2 - bottom1; 
	glm::vec3 v1 = top2 - bottom1; 
	glm::vec3 v2 = bottom2 - top1; 
	glm::vec3 v3 = top2 - top1;
 
	// squared distances:
	float d0 = dot(v0, v0); 
	float d1 = dot(v1, v1); 
	float d2 = dot(v2, v2); 
	float d3 = dot(v3, v3);
 
	// select best potential endpoint on capsule A:
	glm::vec3 bestA;
	if (d2 < d0 || d2 < d1 || d3 < d0 || d3 < d1)
	{
	  bestA = top1;
	}
	else
	{
	  bestA = bottom1;
	}
 
	// select point on capsule B line segment nearest to best potential endpoint on A capsule:
	glm::vec3 bestB = ClosestPointOnLineSegment(bottom2, top2, bestA);
 
	// now do the same for capsule A segment:
	bestA = ClosestPointOnLineSegment(bottom1, top1, bestB);

	Collision collision = SphereToSphere( bestA, radius1, bestB, radius2 );

	return collision;
}

const Collision CollisionDetection::CapsuleToSphere( glm::vec3 capTop, glm::vec3 capBottom, float capRadius, glm::vec3 sphereCenter, float sphereRadius ) {

	// find closest sphere on capsule
	glm::vec3 closestSphere = ClosestPointOnLineSegment( capBottom, capTop, sphereCenter );

	Collision collision = SphereToSphere( closestSphere, capRadius, sphereCenter, sphereRadius );

	return collision;
}
