#ifndef GEO_PLANE_HPP
#define GEO_PLANE_HPP

#include "Geo/Vector.hpp"
#include "Geo/Point.hpp"
#include "Geo/Transformation.hpp"

namespace Geo {

class Plane {
public:
	Plane()
	{
		mElements[0] = 0;
		mElements[1] = 0;
		mElements[2] = 0;
		mElements[3] = 0;
	}

	Plane(float x, float y, float z, float distance)
	{
		mElements[0] = x;
		mElements[1] = y;
		mElements[2] = z;
		mElements[3] = -distance;
	}

	Plane(const Vector &normal, float distance)
	{
		mElements[0] = normal.x();
		mElements[1] = normal.y();
		mElements[2] = normal.z();
		mElements[3] = -distance;
	}

	Plane(const float elements[4])
	{
		for(int i=0; i<4; i++) {
			mElements[i] = elements[i];
		}
	}

	Plane(const Plane &other)
	{
		for(int i=0; i<4; i++) {
			mElements[i] = other.mElements[i];
		}
	}

	Plane &operator=(const Plane &other)
	{
		for(int i=0; i<4; i++) {
			mElements[i] = other.mElements[i];
		}

		return *this;
	}

	float operator() (int n) const { return mElements[n]; }

	Vector normal() const { return Vector(mElements[0], mElements[1], mElements[2]); }
	float distance() const { return -mElements[3]; }

	bool pointInFront(const Point &point) const
	{
		return point.x() * mElements[0] + point.y() * mElements[1] + point.z() * mElements[2] + mElements[3] > 0;
	}

private:
	float mElements[4];
};

static Plane operator*(const Plane &plane, const Matrix &matrix)
{
	float elements[4];

	for(int i=0; i<4; i++) {
		elements[i] = 0;
		for(int j=0; j<4; j++) {
			elements[i] += plane(j) * matrix(j, i);
		}
	}

	return Plane(elements);
}

static Plane operator*(const Plane &plane, const Transformation &transformation)
{
	return plane * transformation.inverseMatrix();
}

}
#endif