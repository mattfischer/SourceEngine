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

	const Vector &normal() const { return mNormal; }
	float distance() const { return mDistance; }

	bool pointInFront(const Vector &point) const { return mNormal * point > mDistance; }

private:
	Vector mNormal;
	float mDistance;
};

}

#endif