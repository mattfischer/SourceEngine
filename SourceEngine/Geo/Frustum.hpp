#ifndef GEO_FRUSTUM_HPP
#define GEO_FRUSTUM_HPP

#include "Geo/Plane.hpp"

namespace Geo {

class Frustum {
public:
	Frustum() {}
	Frustum(float hfov, float aspectRatio);
	Frustum(const Plane planes[4]);

	Frustum rotateX(float angle);
	Frustum rotateZ(float angle);
	Frustum translate(const Vector &disp);

private:
	Plane mPlanes[4];
};

}
#endif