#ifndef GEO_PLANE_HPP
#define GEO_PLANE_HPP

#include "Geo/Vector.hpp"
#include "Geo/Point.hpp"

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

	Vector normal() const { return Vector(mElements[0], mElements[1], mElements[2]); }
	float distance() const { return -mElements[3]; }

	bool pointInFront(const Point &point) const
	{
		return point.x() * mElements[0] + point.y() * mElements[1] + point.z() * mElements[2] + mElements[3] > 0;
	}

private:
	float mElements[4];
};

}
#endif