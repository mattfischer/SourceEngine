#ifndef GEO_FRUSTUM_HPP
#define GEO_FRUSTUM_HPP

#include "Geo/Plane.hpp"
#include "Geo/Box.hpp"

namespace Geo {

class Frustum {
public:
	Frustum() {}
	Frustum(float hfov, float aspectRatio);
	Frustum(const Plane planes[4]);

	Frustum rotateY(float angle);
	Frustum rotateZ(float angle);
	Frustum translate(const Vector &disp);

	bool boxOutside(const Box &box) const;

private:
	Plane mPlanes[4];
};

}
#endif