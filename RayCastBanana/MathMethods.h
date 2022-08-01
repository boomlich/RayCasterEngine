#pragma once

#include <cmath>
#include <SFML/System/Vector2.hpp>

inline double vectorLength(double x, double y)
{
	return sqrt(x * x + y * y);
}

inline sf::Vector2<double> vectorNormal(double x, double y)
{
	return sf::Vector2<double>(-y, x);
}

inline sf::Vector2<double> vectorUnit(double x, double y)
{
	double length = vectorLength(x, y);
	return sf::Vector2<double>(x / length, y / length);
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