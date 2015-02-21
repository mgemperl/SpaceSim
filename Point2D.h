#pragma once

#ifndef PI
#define PI 3.14159265
static const double TWO_PI = PI * 2.0;
static const double HALF_PI = PI / 2.0;
static const double THIRD_PI = PI / 3.0;
static const double QUARTER_PI = PI / 4.0;
static const double EIGHTH_PI = PI / 8.0;
static const double SIXTEENTH_PI = PI / 16.0;
#endif

#ifndef THETA_E
#define THETA_E 0.0001
#endif

#ifndef RANDOM
#define RANDOM (double(rand()) / double(RAND_MAX))
#define RANDSIGN ((rand() % 2) ? -1.0 : 1.0)
#define RANDBOOL ((rand() % 2) ? false : true)
#endif

#include <cmath>
#include <stdlib.h>

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
	Point2D(T inX, T inY)
	{
		X = inX;
		Y = inY;
	}

	/**
	 * Copy constructor for a 2D point in space.
	 * @param  point  reference to the point to be copied
	 */
	Point2D(const Point2D<T>& point)
	{
		X = point.X;
		Y = point.Y;
	}

	/**
	 * Contsructor for a random point within the argued rectangle.
	 * @param topLeft  upper-left-most point possible
	 * @param botRight lower-right-most point possible
	 */
	Point2D(const Point2D<T>& topLeft, const Point2D<T>& botRight)
	{
		X = topLeft.X + RANDOM * (botRight.X - topLeft.X);
		Y = botRight.Y + RANDOM * (topLeft.Y - botRight.Y);
	}

	 /**
	 * Default contructor that initializes the point at the origin.
	 */
	Point2D(void)
	{
		X = 0;
		Y = 0;
	}

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
	* Method that computes the angle from this point to the argued point.
	*/
	inline double Angle(const Point2D<T>& other) const
	{
		double angle = fmod(atan2(other.Y - Y, other.X - X), TWO_PI);
		return (angle < 0 ? TWO_PI + angle : angle);
	}

	inline double Angle() const
	{
		double angle = fmod(atan2(Y, X), TWO_PI);
		return (angle < 0 ? TWO_PI + angle : angle);
	}

	/**
	 * Computes and returns the distance between this point
	 * and the argued point.
	 * @param other  reference to the other point
	 * @returns distance between this point and the argued point
	 */
	inline T Distance(const Point2D<T>& other) const
	{
		return std::sqrt((other.X - X) * (other.X - X) +
			(other.Y - Y) * (other.Y - Y));
	}

	/**
	* Computes and returns the distance between this point
	* and the origin.
	* @returns distance between this point and the origin
	*/
	inline T Distance() const
	{
		return std::sqrt(X * X + Y * Y);
	}


	/**
	 * Computes and returns the square distance between this point
	 * and the argued point, a cheaper operation than finding the
	 * distance.
	 * @param other  the other point
	 * @returns square distance between this point and the argued point
	 */
	inline T SqrDistance(const Point2D<T>& other) const
	{
		return abs((other.X - X) * (other.X - X) +
			(other.Y - Y) * (other.Y - Y));
	}

	/**
	* Computes and returns the square distance between this point
	* and the origin, a cheaper operation than finding the
	* distance.
	* @returns square distance between this point and the origin
	*/
	inline T SqrDistance() const
	{
		return X * X + Y * Y;
	}

	/**
	 * Checks whether this point is contained within the argued rectangle,
	 * including the edges.
	 * @param topLeft  the top left corner of the rectangle
	 * @param botRight the bottom right corner of the rectangle
	 * @returns        true if this point is in the rectangle
	 *                 false if this point is not in the rectangle
	 */
	inline bool InRect(const Point2D<T>& topLeft, const Point2D<T>& botRight) const
	{
		return ((X >= topLeft.X) &&
			(X <= botRight.X) &&
			(Y >= botRight.Y) &&
			(Y <= topLeft.Y)
			);
	}

	/**
	 * Implementation of = operator.
	 */
	inline Point2D<T>& operator=(const Point2D<T>& rhs)
	{
		X = rhs.X;
		Y = rhs.Y;

		return *this;
	}

	/**
	 * Implementation of == operator.
	 */
	inline bool operator==(const Point2D<T>& other) const
	{
		return Equals(other);
	}

	/**
	 * Implementation of != operator.
	 */
	inline bool operator!=(const Point2D<T>& other) const
	{
		return !Equals(other);
	}

	/**
	 * Implementation of += operator.
	 */
	inline Point2D<T>& operator+=(const Point2D<T>& rhs)
	{
		X += rhs.X;
		Y += rhs.Y;
		return *this;
	}

	/** 
	 *  Implementation of -= operator.
	 */
	inline Point2D<T>& operator-=(const Point2D<T>& rhs)
	{
		X -= rhs.X;
		Y -= rhs.Y;
		return *this;
	}

	/**
	 * Implementation of + operator.
	 */
	inline const Point2D<T> operator+(const Point2D<T>& rhs) const
	{
		return Point2D<T>(X + rhs.X, Y + rhs.Y);
	}

	/**
	* Implementation of binary - operator.
	*/
	inline const Point2D<T> operator-(const Point2D<T>& rhs) const
	{
		return Point2D<T>(X - rhs.X, Y - rhs.Y);
	}

	/**
	* Implementation of unary - operator.
	*/
	inline Point2D<T> operator-() const
	{
		return Point2D<T>(-X, -Y);
	}


	/**
	*  Scale the point by the scalar rhs
	*/
	inline Point2D<T> operator*(T rhs) const
	{
		return Point2D<T>(X * rhs, Y * rhs);
	}

	/**
	 * Method that checks for equivalence between this
	 * and the argued object.
	 * @param  other  reference to the other point
	 */
	inline bool Equals(const Point2D<T>& other) const
	{
		return ((this->X <= other.X + THETA_E) &&
			(this->X >= other.X - THETA_E) &&
			(this->Y <= other.Y + THETA_E) &&
			(this->Y >= other.Y - THETA_E)
		);
	}

private:

	/** The x coordinate of the point */
	T X;

	/** The y coordinate of the point */
	T Y;
};

}
