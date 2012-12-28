#ifndef GEO_FRUSTUM_HPP
#define GEO_FRUSTUM_HPP

#include "Geo/Plane.hpp"
#include "Geo/BoxAligned.hpp"
#include "Geo/BoxOriented.hpp"
#include "Geo/Transformation.hpp"

namespace Geo {

class Frustum {
public:
	Frustum() {}
	Frustum(float hfov, float aspectRatio);
	Frustum(const Plane planes[4])
	{
		for(int i=0; i<4; i++) {
			mPlanes[i] = planes[i];
		}
	}

	const Plane &plane(int n) const { return mPlanes[n]; }

	bool boxOutside(const BoxAligned &box) const;
	bool boxOutside(const BoxOriented &oriented) const;

private:
	Plane mPlanes[4];
};

static Frustum operator*(const Frustum &frustum, const Transformation &transformation)
{
	Plane planes[4];
	for(int i=0; i<4; i++) {
		planes[i] = frustum.plane(i) * transformation;
	}

	return Frustum(planes);
}

}
#endif