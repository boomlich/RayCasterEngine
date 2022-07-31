#pragma once

#include "Model.h"
#include "Renderer.h"

class Controller
{
private:
	Renderer* m_renderer;
	Model* m_model;
	CtrlPawn* m_pawn;
	sf::Clock clock;
	double m_dt;
	int oldMouseX;

private:
	void gameLoop();
	void keyboardInput();
public:
	Controller(Model* model, Renderer* renderer);
};
