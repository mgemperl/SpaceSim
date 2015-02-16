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

Point2D<double> Vector2D::GetTerminal() const
{
	return Point2D<double>(terminal);
}

Point2D<double> Vector2D::GetTerminal(const Point2D<double>& initial) const
{
	return (terminal + initial);
}

double Vector2D::GetAngle() const
{
	return angle;
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

void Vector2D::rotate(double dTheta)
{
	setAngle(angle + dTheta);
}

void Vector2D::rotateTo(double theta)
{
	setAngle(theta);
}

void Vector2D::scaleBy(double scalar)
{
	terminal = terminal * scalar;
	magnitude = 0;
	foundMagnitude = false;
}

void Vector2D::scaleTo(double mag)
{
	magnitude = mag;
	terminal = GetTerminalFromPolar(angle, magnitude);
	foundMagnitude = true;
}

double Vector2D::ComputeAngle(const Point2D<double>& init,
					const Point2D<double>& term)
{
	return SimplifyAngle(atan2((term.GetY() - init.GetY()),
			                   (term.GetX() - init.GetX())));
}

double Vector2D::computeMagnitude(const Point2D<double>& init,
						const Point2D<double>& term)
{
	return init.Distance(term);
}

double Vector2D::ComputeAngle(const Point2D<double>& term)
{
	return SimplifyAngle(std::atan2(term.GetY(), term.GetX()));
}

double Vector2D::computeMagnitude(const Point2D<double>& term)
{
	return term.Distance();
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

double Vector2D::AngleDiff(double theta1, double theta2)
{
	double diff = SimplifyAngle(theta1 - theta2);

	if (diff > PI)
	{
		diff = (TWO_PI - diff);
	}

	return diff;
}

Point2D<double> Vector2D::GetTerminalFromPolar(double theta, double mag)
{
	return Point2D<double>(
		mag * std::cos(theta),
		mag * std::sin(theta)
		);
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

double Vector2D::cross(const Vector2D& other) const
{
	return terminal.GetX() * other.terminal.GetY() -
		   terminal.GetY() * other.terminal.GetX();
}

void Vector2D::normalize()
{
	scaleTo(1);
}

Vector2D Vector2D::normal() const
{
	return Vector2D(angle, 1);
}

void Vector2D::setAngle(double theta)
{
	angle = SimplifyAngle(theta);
	terminal = GetTerminalFromPolar(angle, GetMagnitude());
}

bool Vector2D::operator==(const Vector2D& other) const
{
	return equals(other);
}

bool Vector2D::operator!=(const Vector2D& other) const
{
	return !equals(other);
}

bool Vector2D::equals(const Vector2D& other) const
{
	return (terminal == other.GetTerminal());
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

	//magnitude = computeMagnitude(init, terminal);

	return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& rhs)
{
	Point2D<double> init = Point2D<double>(0, 0);

	terminal -= rhs.terminal;
	angle = ComputeAngle(init, terminal);
	magnitude = 0;
	foundMagnitude = false;

	//magnitude = computeMagnitude(init, terminal);

	return *this;
}

const Vector2D Vector2D::operator+(const Vector2D& rhs) const
{
	return (Vector2D(*this) += rhs);
}

const Vector2D Vector2D::operator-(const Vector2D& rhs) const
{
	return (Vector2D(*this) -= rhs);
}

Vector2D Vector2D::operator-() const
{
	return Vector2D(-terminal);
}

}


