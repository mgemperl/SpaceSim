/*
#include "Point2D.h"

#include <cmath>
#include <stdlib.h>

#include "Data.h"

namespace Space
{

	template <class T>
	Point2D<T>::Point2D(T inX, T inY)
	{
		X = inX;
		Y = inY;
	}

	template <class T>
	Point2D<T>::Point2D(const Point2D<T>& point)
	{
		X = point.X;
		Y = point.Y;
	}

	template <class T>
	Point2D<T>::Point2D(const Point2D<T>& topLeft, const Point2D<T>& botRight)
	{
		X = topLeft.X + RANDOM * (botRight.X - topLeft.X);
		Y = botRight.Y + RANDOM * (topLeft.Y - botRight.Y);
	}

	template <class T>
	Point2D<T>::Point2D(void)
	{
		X = 0;
		Y = 0;
	}

	template <class T>
	T Point2D<T>::Distance(const Point2D<T>& other) const
	{
		return std::sqrt((other.X - X) * (other.X - X) +
			(other.Y - Y) * (other.Y - Y));
	}

	template <class T>
	T Point2D<T>::Distance() const
	{
		return std::sqrt(X * X + Y * Y);
	}

	template <class T>
	T Point2D<T>::SqrDistance(const Point2D<T>& other) const
	{
		return abs((other.X - X) * (other.X - X) +
			(other.Y - Y) * (other.Y - Y));
	}

	template <class T>
	T Point2D<T>::SqrDistance() const
	{
		return X * X + Y * Y;
	}

	template <class T>
	bool Point2D<T>::InRect(const Point2D<T>& topLeft, const Point2D<T>& botRight) const
	{
		return ((X >= topLeft.X) &&
			(X <= botRight.X) &&
			(Y >= botRight.Y) &&
			(Y <= topLeft.Y)
			);
	}

	template <class T>
	Point2D<T>& Point2D<T>::operator=(const Point2D<T>& rhs)
	{
		X = rhs.X;
		Y = rhs.Y;

		return *this;
	}

	template <class T>
	bool Point2D<T>::Equals(const Point2D<T>& other) const
	{
		return ((this->X == other.X) &&
			(this->Y == other.Y));
	}

	template <class T>
	bool Point2D<T>::operator==(const Point2D<T>& other) const
	{
		return Equals(other);
	}

	template <class T>
	bool Point2D<T>::operator!=(const Point2D<T>& other) const
	{
		return !Equals(other);
	}

	template <class T>
	Point2D<T>& Point2D<T>::operator+=(const Point2D<T>& rhs)
	{
		X += rhs.X;
		Y += rhs.Y;
		return *this;
	}

	template <class T>
	Point2D<T>& Point2D<T>::operator-=(const Point2D<T>& rhs)
	{
		X -= rhs.X;
		Y -= rhs.Y;
		return *this;
	}

	template <class T>
	const Point2D<T> Point2D<T>::operator+(const Point2D<T>& rhs) const
	{
		return Point2D<T>(X + rhs.X, Y + rhs.Y);
	}

	template <class T>
	const Point2D<T> Point2D<T>::operator-(const Point2D<T>& rhs) const
	{
		return Point2D<T>(X - rhs.X, Y - rhs.Y);
	}

	template <class T>
	Point2D<T> Point2D<T>::operator-() const
	{
		return Point2D<T>(-X, -Y);
	}

	template <class T>
	Point2D<T> Point2D<T>::operator*(T rhs) const
	{
		return Point2D<T>(X * rhs, Y * rhs);
	}


}
*/