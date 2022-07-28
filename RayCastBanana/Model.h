#pragma once
#include <vector>

#include "camera.h"
#include "m_grid.h"
#include "MapParser.h"

class Model
{

private:
	MapParser m_map_parser;
	Grid m_grid;
	Camera m_camera;
public:
	Model(const int screenWidth, const int screenHeight);
	void update(double deltaTime);
	Grid& getGrid();
};
