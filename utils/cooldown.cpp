#include "cooldown.h"

Cooldown::Cooldown()
	:duration( 0.0f )
{}

Cooldown::Cooldown( float _duration ) 
	:duration(_duration)
{}

bool Cooldown::isOnCooldown() {
	return ((glfwGetTime() - startTime) < duration);
}

void Cooldown::startCooldown() {
	startTime = glfwGetTime();
}

float Cooldown::getRemainingCooldown() {
	return duration - (glfwGetTime() - startTime);
}