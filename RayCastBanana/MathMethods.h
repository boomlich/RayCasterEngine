#pragma once

#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <sstream>

inline double vectorLength(double x, double y)
{
	return sqrt(x * x + y * y);
}

inline sf::Vector2<double> vectorNormal(double x, double y)
{
	return sf::Vector2<double>(-y, x);
}

inline sf::Vector2<double> vectorUnit(double x, double y, double length)
{
	return sf::Vector2<double>(x / length, y / length);
}

inline sf::Vector2<double> vectorUnit(double x, double y)
{
	double length = vectorLength(x, y);
	return vectorUnit(x, y, length);
}

inline sf::Vector2<double> vectorNormalUnit(double x, double y)
{
	sf::Vector2<double> normal = vectorNormal(x, y);
	return vectorUnit(normal.x, normal.y);
}

inline sf::Vector2<double> rotateVector(double x, double y, double angle)
{
	double vecX = x * cos(angle) - y * sin(angle);
	double vecY = x * sin(angle) + y * cos(angle);
	return sf::Vector2<double>(vecX, vecY);
}

inline double angleFromXAxis(double x, double y)
{
	double angle = acos(x / vectorLength(x, y));

	if (y > 0)
	{
		return 2.0 * 3.14159265359 - angle;
	}
	return angle;
}

// Return true if point C is to the left of the line (A, B)
inline bool isLeft(double xA, double yA, double xB, double yB, double xC, double yC) {
	return ((xB - xA) * (yC - yA) - (yB - yA) * (xC - xA)) > 0;
}

inline std::string convertToHexString(int x)
{
	std::stringstream sstream;
	sstream << std::hex << x;
	return sstream.str();
}