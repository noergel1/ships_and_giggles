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
	std::vector<float> POSTPROCESSING_KERNEL;
	bool ENABLE_POLYGONMODE;
	bool SHOW_NORMALS;
	bool SHOW_VERTICES;
	bool SHOW_COLLIDERS;


	// other options

};