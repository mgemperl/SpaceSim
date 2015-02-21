#pragma once

#include "Point2D.h"

namespace Space
{

/**
 * A two dimensional vector to represent a direction and magnitude in space
 * centred at the origin.
 * @author Markus Gemperle
 * @version 2015_02_08
 */
class Vector2D
{

public:

	/** Copy Constructor **/
	Vector2D(const Vector2D& vector);

	/**
	 * Constructor taking the terminal point of a 2D vector.
	 * @param  inTerminal  reference to the terminal point of the vector
	 */
	Vector2D(const Point2D<double>& inTerminal);

	/**
	 * Constructor taking the initial and terminal points of a 2D vector.
	 * @param  inInitial   reference to the initial point of the vector
	 * @param  inTerminal  reference to the terminal point of the vector
	 */
	Vector2D(const Point2D<double>& inInitial,
			 const Point2D<double>& inTerminal);

	/**
	 * Constructor that creates a vector where the argued initial and terminal points 
	 * determine the angle and the argued magnitude determines the magnitude.
	 * @param inInitial  reference to the initial point for the vector
	 * @param inTarget   reference to the terminal point for the vector
	 * @param inMagnitude the magnitude of the vector
	 */
	Vector2D(const Point2D<double>& inInitial,
		const Point2D<double>& inTarget,
		double inMagnitude);

	/**
	 * Constructor taking the polar coordinates of a 2D vector.
	 * @param  inAngleRads  the angle of the vector in radians
	 * @param  inMagnitude  the length of the vector
	 */
	Vector2D(double inAngleRads, double inMagnitude);

	/**
	 * Constructor that creates a vector with a random angle and
	 * a magnitude less than the argued value.
	 * @param maxMagnitude  the maximum value for the vector's magnitude
	 */
	Vector2D(double maxMagnitude);

	/**
	 * Default constructor that makes the zero vector.
	 */
	Vector2D();

	/**
	 * Destructor for Vector2D
	 */
	~Vector2D();

	/**
	 * Accessor method for the terminal point of the vector.
	 * @returns the terminal point of the vector
	 */
	inline Point2D<double> GetTerminal() const
	{
		return Point2D<double>(terminal);
	}

	/**
	* This method returns the terminal point of the vector offset by
	* the position of the argued initial point.
	* @param initial  Point representing the offset
	* @return         the terminal point
	*/
	inline Point2D<double> GetTerminal(const Point2D<double>& initial) const
	{
		return (terminal + initial);
	}

	/**
	 * Accessor method for the angle of the vector in radians
	 * @returns angle of the vector in radians
	 */
	inline double GetAngle() const
	{
		return angle;
	}

	/**
	 * Accessor method for the magnitude of this vector. This operation requires a square root
	 * on its first invocation and thus can be expensive, so do not use it excessively with 
	 * different vectors.
	 * @returns magnitude of this vector
	 */
	double GetMagnitude();

	/**
	 * Accessor method for the squared magnitude of this vector. Less expensive
	 * than GetMagnitude()
	 * @returns squared magnitude of this vector
	 */
	double GetSqrMagnitude() const;

	/**
	 * Checks whether this vector overlaps the argued rectangle.
	 * @param rect    the rectangle
	 * @returns       true if this vector overlaps the rectangle
	 *                false if this vector does not overlap the rectangle
	 */
	//bool Overlaps(const RECT& rect);

	/**
	 * Method that rotates the vector by the argued number of radians.
	 * @param dTheta  the number of radians by which to rotate the vector
	 */
	inline void rotate(double dTheta)
	{
		setAngle(angle + dTheta);
	}

	/**
	 * Method that rotates the vector to the argued angle in radians.
	 *
	 * @param dTheta  the number of radians by which to rotate the angle
	 */
	inline void rotateTo(double theta)
	{
		setAngle(theta);
	}

	/**
	 * Method that scales the vector by the argued value.
	 * @param scalar  the value by which the vector will be scaled
	 */
	void scaleBy(double scalar);

	/**
	 * Method that sets the magnitude to the argued value.
	 * @param  mag  the value to which the magnitude will be set
	 */
	void scaleTo(double mag);

	/**
	 * Method that normalizes the vector.
	 */
	inline void normalize()
	{
		scaleTo(1);
	}

	/**
	 * Method that returns the normal of this vector.
	 * @returns the normal of this vector
	 */
	inline Vector2D normal() const
	{
		return Vector2D(angle, 1);
	}

	/**
	*  Returns a vector perpendicular to this one.
	*/
	inline Vector2D Perpendicular() const
	{
		return Vector2D(angle + HALF_PI, magnitude);
	}

	/**
	 * Method that returns the cartesian point represented by the argued
	 * polar coordinates.
	 * @param theta  the angular coordinate of the polar point
	 * @param mag    the radial coordinate of the polar point
	 * @returns      the cartesian point represented by the argued polar coordinates
	 */
	inline static Point2D<double> GetTerminalFromPolar(double theta, double mag)
	{
		return Point2D<double>(
			mag * std::cos(theta),
			mag * std::sin(theta)
			);
	}

	/**
	* Method that returns a random point within the argued distance
	* of the origin.
	* @param maxMag the maximum possible distance of the point from the origin
	* @returns      a random Point2D within the argued area
	*/
	static Point2D<double> GetRandTerminalFromPolar(double maxMag);

	/**
	* Method that returns a random point within the argued area.
	* @param maxMag the maximum possible distance of the point from the origin
	* @param minMag the minimum possible distance of the point from the origin
	* @returns      a random Point2D within the argued area
	*/
	static Point2D<double> GetRandTerminalFromPolar(double minMag, double maxMag);

	/**
	 * Method that computes the angle from the first point to the
	 * second point.
	 * @param init  the first point
	 * @param term  the second point
	 * @returns     the angle from the first point to the second point
	 */
	inline static double ComputeAngle(const Point2D<double>& init,
		const Point2D<double>& term)
	{
		return init.Angle(term);
	}

	/**
	 * Method that computes the distance between the argued points.
	 * @param init  the first point
	 * @param term  the second point
	 * @returns     the distance between the argued points
	 */
	inline static double computeMagnitude(const Point2D<double>& init,
		const Point2D<double>& term)
	{
		return init.Distance(term);
	}

	/**
	 * Method that computes the angle from the origin to the argued point.
	 * @param   term  point whose angle from the origin will be computed
	 * @returns the angle in radians from the origin to the argued point
	 */
	inline static double ComputeAngle(const Point2D<double>& term)
	{
		return term.Angle();
	}

	/**
	 * Method that computes the distance between the argued point and the origin.
	 * @param term  point whose distance from the origin will be computed
	 * @returns     the distance between the origin and the point
	 */
	inline static double computeMagnitude(const Point2D<double>& term)
	{
		return term.Distance();
	}

	/**
	 * Reduces the argued angle to an equivalent angle between 0 and 2π radians
	 * @param theta  angle in radians to be reduced
	 * @return       equivalent angle between 0 and 2π radians
	 */
	static double SimplifyAngle(double theta);
	
	/**
	 * Returns the difference between the argued angles.
	 * @param theta1  first angle in radians
	 * @param theta2  second angle in radians
	 * @return        difference between the argued angle in radians
	 */
	inline static double AngleDiff(double theta1, double theta2)
	{
		double diff = SimplifyAngle(theta1 - theta2);
		return diff > PI ? (TWO_PI - diff) : diff;
	}

	/**
	 * Computes the dot product of this and the argued vector
	 */
	inline double Dot(const Vector2D& other) const
	{
		return GetTerminal().GetX() * other.GetTerminal().GetX() +
			GetTerminal().GetY() * other.GetTerminal().GetY();
	}

	/**
	 * Computes the length of the projection of the argued vector onto 
	 * this vector
	 */
	inline double ProjLength(const Vector2D& other)
	{
		return Dot(other) / GetMagnitude();
	}

	/**
	 * Method that computes the cross product of this and the argued vector.
	 * @param other  the other vector
	 * @returns      the cross product of this and the other vector
	 */
	inline double cross(const Vector2D& other) const
	{
		return terminal.GetX() * other.terminal.GetY() -
			terminal.GetY() * other.terminal.GetX();
	}


	/**
	 * Override of == operator that checks for equivalence
	 * between this and the argued vector.
	 */
	inline bool operator==(const Vector2D& other) const
	{
		return equals(other);
	}

	/**
	 * Override of != operator that checks for inequivalence
	 * between this and the argued vector.
	 */
	inline bool operator!=(const Vector2D& other) const
	{
		return !equals(other);
	}

	/**
	 * Method that checks for equivalence between this
	 * and the argued vector.
	 *
	 * @param  other  reference to the other point
	 */
	inline bool equals(const Vector2D& other) const
	{
		return (terminal == other.GetTerminal());
	}

	/**
	 * Implementation of the assignment operator.
	 */
	Vector2D& operator=(const Vector2D& rhs);

	/**
	 * Implementation of the += operator.
	 */
	Vector2D& operator+=(const Vector2D& rhs);

	/**
	 * Implementation of the -= operator.
	 */
	Vector2D& operator-=(const Vector2D& rhs);

	/**
	 * Implementation of the + operator.
	 */
	inline const Vector2D operator+(const Vector2D& rhs) const
	{
		return (Vector2D(*this) += rhs);
	}

	/**
	 * Implementation of the binary - operator.
	 */
	inline const Vector2D operator-(const Vector2D& rhs) const
	{
		return (Vector2D(*this) -= rhs);
	}

	/**
	 * Implementation of the unary - operator.
	 */
	inline Vector2D operator-() const
	{
		return Vector2D(-terminal);
	}

	bool operator<(const Vector2D& other)
	{
		return angle < other.angle;
	}

private:

	/** Terminal point of this vector */
	Point2D<double> terminal;

	/** Angle of this vector in radians between 0 and 2PI */
	double angle;

	/** Magnitude of this vector */
	double magnitude;

	/** Boolean for whether this vector's magnitude is known */
	bool foundMagnitude;

	/**
	 * This method sets the angle of the vector to the equivalent of the argued
	 * angle between 0 and 2PI.
	 * @param theta  the new angle of the vector
	 */
	inline void setAngle(double theta)
	{
		angle = SimplifyAngle(theta);
		terminal = GetTerminalFromPolar(angle, GetMagnitude());
	}

};


}

