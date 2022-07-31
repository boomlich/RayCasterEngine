#pragma once

class CtrlPawn
{
public:
	virtual void moveForward(double x) = 0;
	virtual void moveSideways(double x) = 0;
	virtual void rotate(double x) = 0;
};
