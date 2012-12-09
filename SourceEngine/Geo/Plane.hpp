#ifndef GEO_PLANE_HPP
#define GEO_PLANE_HPP

#include "Geo/Vector.hpp"

namespace Geo {

class Plane {
public:
	Plane() : mDistance(0) {}

	Plane(const Vector &normal, float distance)
		: mNormal(normal),
		  mDistance(distance)
	{}

	const Vector &normal() { return mNormal; }
	float distance() { return mDistance; }

	bool pointInFront(const Vector &point) { return mNormal * point > mDistance; }

private:
	Vector mNormal;
	float mDistance;
};

}

#endif