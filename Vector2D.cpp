#include <cmath>
#include <stdlib.h>
#include "Vector2D.h"
#include "Data.h"

namespace Space
{

Vector2D::Vector2D(const Point2D<double>& inTerminal)
{
	terminal = Point2D<double>(inTerminal);
	Point2D<double> init = Point2D<double>(0, 0);
	angle = ComputeAngle(init, inTerminal);
	magnitude = 0;
	foundMagnitude = false;
}

Vector2D::Vector2D(const Point2D<double>& inInitial,
				   const Point2D<double>& inTerminal)
{
	terminal = Point2D<double>(inTerminal);
	angle = ComputeAngle(inInitial, inTerminal);
	magnitude = 0;
	foundMagnitude = false;
}

Vector2D::Vector2D(const Point2D<double>& inInitial,
	const Point2D<double>& inTarget,
	double inMagnitude)
{
	angle = ComputeAngle(inInitial, inTarget);
	magnitude = inMagnitude;
	terminal = GetTerminalFromPolar(angle, magnitude);
	foundMagnitude = true;
}

Vector2D::Vector2D(double inAngleRads, double inMagnitude)
{
	angle = inAngleRads;
	magnitude = inMagnitude;
	terminal = GetTerminalFromPolar(angle, magnitude);
	foundMagnitude = true;
}

Vector2D::Vector2D(const Vector2D& other)
{
	terminal = Point2D<double>(other.terminal);
	angle = other.angle;

	if (other.foundMagnitude)
	{
		magnitude = other.magnitude;
		foundMagnitude = true;
	}
	
}

Vector2D::Vector2D(double maxMag)
{
	terminal = GetRandTerminalFromPolar(maxMag);
	foundMagnitude = true;
}

Vector2D::Vector2D()
{
	terminal = Point2D<double>(0, 0);
	angle = 0;
	magnitude = 0;
	foundMagnitude = true;
}

Vector2D::~Vector2D()
{

}

double Vector2D::GetMagnitude()
{
	if (!foundMagnitude)
	{
		magnitude = computeMagnitude(terminal);
		foundMagnitude = true;
	}
	
	return magnitude;
}

double Vector2D::GetSqrMagnitude() const
{
	double sqrMag;

	if (foundMagnitude)
	{
		sqrMag = (magnitude * magnitude);
	}
	else
	{
		sqrMag = terminal.SqrDistance();
	}

	return sqrMag;
}

void Vector2D::scaleBy(double scalar)
{
	terminal = terminal * scalar;

	if (foundMagnitude)
	{
		magnitude *= scalar;
	}
	else
	{
		magnitude = 0;
		foundMagnitude = false;
	}
}

void Vector2D::scaleTo(double mag)
{
	magnitude = mag;
	terminal = GetTerminalFromPolar(angle, magnitude);
	foundMagnitude = true;
}

double Vector2D::SimplifyAngle(double theta)
{
	if (theta > TWO_PI)
	{
		theta = std::fmod(theta, TWO_PI);
	}
	else if (theta < 0)
	{
		theta = TWO_PI - std::fmod(-theta, TWO_PI);
	}

	return theta;
}

Point2D<double> Vector2D::GetRandTerminalFromPolar(double maxMag)
{
	double theta = RANDOM * TWO_PI;
	double mag = RANDOM * maxMag;
	
	return Point2D<double>(
		mag * std::cos(theta),
		mag * std::sin(theta)
		);
}

Point2D<double> Vector2D::GetRandTerminalFromPolar(double minMag, double maxMag)
{
	Point2D<double> point;

	if (minMag <= maxMag)
	{
		double theta = RANDOM * TWO_PI;
		double mag = minMag + RANDOM * (maxMag - minMag);

		point = Point2D<double>(
			mag * std::cos(theta),
			mag * std::sin(theta)
			);
	}
	
	return point;
}

Vector2D& Vector2D::operator=(const Vector2D& rhs)
{
	if(*this != rhs)
	{
		this->terminal = rhs.terminal;
		this->angle = rhs.angle;
		this->magnitude = rhs.magnitude;
	}

	return *this;
}

Vector2D& Vector2D::operator+=(const Vector2D& rhs)
{
	Point2D<double> init = Point2D<double>(0, 0);

	terminal += rhs.terminal;
	angle = ComputeAngle(init, terminal);
	magnitude = 0;
	foundMagnitude = false;

	return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& rhs)
{
	Point2D<double> init = Point2D<double>(0, 0);

	terminal -= rhs.terminal;
	angle = ComputeAngle(init, terminal);
	magnitude = 0;
	foundMagnitude = false;

	return *this;
}

}


