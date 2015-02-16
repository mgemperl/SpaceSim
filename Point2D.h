#pragma once

#include <cmath>
#include <stdlib.h>
#include "Data.h"

namespace Space
{

template <class T>
/**
* A point in 2 dimensional space.
* @author Markus Gemperle
* @version 2015_02_08
* @pre  type T must be a numeric primitive data type (i.e. int, float, double, etc.)
*/
class Point2D
{
public:

	/**
	 * Constructor of a 2D point in space.
	 * @param  inX  a value representing an X coordinate in 2-space
	 * @param  inY  a value representing a Y coordinate in 2-space
	 */
	Point2D(T inX, T inY);

	/**
	 * Copy constructor for a 2D point in space.
	 * @param  point  reference to the point to be copied
	 */
	Point2D(const Point2D<T>& point);

	/**
	 * Contsructor for a random point within the argued rectangle.
	 * @param topLeft  upper-left-most point possible
	 * @param botRight lower-right-most point possible
	 */
	Point2D(const Point2D<T>& topLeft, const Point2D<T>& botRight);

	 /**
	 * Default contructor that initializes the point at the origin.
	 */
	Point2D(void);

	/**
	 * Accessor method for the X coordinate.
	 * @returns value of the X coordinate
	 */
	inline T GetX() const { return X; }

	/**
	 * Accessor method for the Y coordinate.
	 * @returns value of the Y coordinate
	 */
	inline T GetY() const { return Y; }

	/**
	 * Method for setting the point's coordinates
	 * @param neoX  the new X coordinate
	 * @param neoY  the new Y coordinate
	 */
	inline void Set(T neoX, T neoY) { X = neoX; Y = neoY; }

	/**
	 * Method for setting the point's coordinates
	 * @param point  point with this point's new coordinates
	 */
	inline void Set(const Point2D<T>& point) { X = point.X; Y = point.Y; }

	/**
	 * Computes and returns the distance between this point
	 * and the argued point.
	 * @param other  reference to the other point
	 * @returns distance between this point and the argued point
	 */
	T Distance(const Point2D<T>& other) const;

	/**
	* Computes and returns the distance between this point
	* and the origin.
	* @returns distance between this point and the origin
	*/
	T Distance() const;

	/**
	 * Computes and returns the square distance between this point
	 * and the argued point, a cheaper operation than finding the
	 * distance.
	 * @param other  the other point
	 * @returns square distance between this point and the argued point
	 */
	T SqrDistance(const Point2D<T>& other) const;

	/**
	* Computes and returns the square distance between this point
	* and the origin, a cheaper operation than finding the
	* distance.
	* @returns square distance between this point and the origin
	*/
	T SqrDistance() const;

	/**
	 * Checks whether this point is contained within the argued rectangle,
	 * including the edges.
	 * @param topLeft  the top left corner of the rectangle
	 * @param botRight the bottom right corner of the rectangle
	 * @returns        true if this point is in the rectangle
	 *                 false if this point is not in the rectangle
	 */
	bool InRect(const Point2D<T>& topLeft, const Point2D<T>& botRight) const;

	/**
	 * Implementation of = operator.
	 */
	Point2D<T>& operator=(const Point2D<T>& rhs);

	/**
	 * Implementation of == operator.
	 */
	bool operator==(const Point2D<T>& other) const;

	/**
	 * Implementation of != operator.
	 */
	bool operator!=(const Point2D<T>& other) const;

	/**
	 * Implementation of += operator.
	 */
	Point2D<T>& operator+=(const Point2D<T>& rhs);

	/** 
	 *  Implementation of -= operator.
	 */
	Point2D<T>& operator-=(const Point2D<T>& rhs);

	/**
	 * Implementation of + operator.
	 */
	const Point2D<T> operator+(const Point2D<T>& rhs) const;

	/**
	* Implementation of binary - operator.
	*/
	const Point2D<T> operator-(const Point2D<T>& rhs) const;

	/**
	* Implementation of unary - operator.
	*/
	Point2D<T> operator-() const;

	/**
	* Implementation of * operator with scalars
	*/
	Point2D<T> operator*(T rhs) const;

	/**
	 * Method that checks for equivalence between this
	 * and the argued object.
	 * @param  other  reference to the other point
	 */
	bool Equals(const Point2D<T>& other) const;

private:

	/** The x coordinate of the point */
	T X;

	/** The y coordinate of the point */
	T Y;
};


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
