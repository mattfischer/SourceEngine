#include "Geo/Frustum.hpp"

#include "Geo/Point.hpp"

#include <math.h>

namespace Geo {

Frustum::Frustum(float hfov, float aspectRatio)
{
	float hFovRad = hfov * 3.14f / 180.0f;

	mPlanes[0] = Plane(Vector(sinf(hFovRad), cosf(hFovRad), 0), 0);
	mPlanes[1] = Plane(Vector(sinf(hFovRad), -cosf(hFovRad), 0), 0);

	float vFovRad = atanf(aspectRatio * tanf(hFovRad));

	mPlanes[2] = Plane(Vector(sinf(vFovRad), 0, cosf(vFovRad)), 0);
	mPlanes[3] = Plane(Vector(sinf(vFovRad), 0, -cosf(vFovRad)), 0);
}

bool Frustum::boxOutside(const Box &box) const
{
	bool outside;

	for(int i=0; i<4; i++) {
		outside = true;

		for(int j=0; j<8; j++) {
			float x = (j&1) ? box.minPoint().x() : box.maxPoint().x();
			float y = (j&2) ? box.minPoint().y() : box.maxPoint().y();
			float z = (j&4) ? box.minPoint().z() : box.maxPoint().z();

			Point point(x, y, z);
			if(mPlanes[i].pointInFront(point)) {
				outside = false;
				break;
			}
		}

		if(outside) {
			break;
		}
	}

	return outside;
}

}