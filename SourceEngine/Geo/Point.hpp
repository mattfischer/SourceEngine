#ifndef GEO_POINT_HPP
#define GEO_POINT_HPP

#include "Geo/Vector.hpp"

namespace Geo {

class Point {
public:
	Point()
	{
		mElements[0] = 0;
		mElements[1] = 0;
		mElements[2] = 0;
	}

	Point(float x, float y, float z)
	{
		mElements[0] = x;
		mElements[1] = y;
		mElements[2] = z;
	}

	Point(const Point &other)
	{
		for(int i=0; i<3; i++) {
			mElements[i] = other.mElements[i];
		}
	}

	Point &operator=(const Point &other)
	{
		for(int i=0; i<3; i++) {
			mElements[i] = other.mElements[i];
		}

		return *this;
	}

	operator Vector() const
	{
		return Vector(x(), y(), z());
	}

	float x() const { return mElements[0]; }
	float y() const { return mElements[1]; }
	float z() const { return mElements[2]; }

	Point operator+(const Vector &vector) const
	{
		return Point(x() + vector.x(), y() + vector.y(), z() + vector.z());
	}

	Vector operator-(const Point &other) const
	{
		return Vector(x() - other.x(), y() - other.y(), z() - other.z());
	}

private:
	float mElements[3];
};

static Point operator+(const Vector &vector, const Point &point)
{
	return point + vector;
}

}
#endif