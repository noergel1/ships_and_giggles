#pragma once

#include <GLFW/glfw3.h>

class Cooldown {
	
public:
	Cooldown();
	Cooldown( float _duration );

	bool isOnCooldown();

	void startCooldown();
	void resetCooldown();

	float getRemainingCooldown();

private:
	float duration;

	// time stamp when cooldown started
	float startTime = 0.0f;
};