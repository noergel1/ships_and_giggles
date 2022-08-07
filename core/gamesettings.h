#pragma once

	// defines the camera modes
enum Camera_Mode {
	FREE_FLOAT,
	ISOMETRIC
};

struct GameSettings
{
	// screen size
	const unsigned int SCR_WIDTH;
	const unsigned int SCR_HEIGHT;

	// camera perspective
	const Camera_Mode CAM_MODE;

	// debug options
	const std::vector<float> POSTPROCESSING_KERNEL;
	const bool ENABLE_POLYGONMODE;

	// other options
};