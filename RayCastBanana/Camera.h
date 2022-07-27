#pragma once
#include "vec.h"

class Camera
{
private:
	int m_screenWidth;
	Vec2 m_position; // Position in 2d space of the camera
	Vec2 m_direction; // Direction of the camera
	Vec2 m_cameraPlane; // 2d version of the camera plane
public:

private:
	void ray_trace();
public:
	Camera(int screenWidth, Vec2 position, Vec2 direction, Vec2 cameraPlane);
	void update();
};
