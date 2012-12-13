#include "Geo/Frustum.hpp"

#include <math.h>

namespace Geo {

Frustum::Frustum(float hfov, float aspectRatio)
{
	float hFovRad = hfov * 3.14f / 180.0f;

	mPlanes[0] = Plane(Vector(cosf(hFovRad), sinf(hFovRad), 0), 0);
	mPlanes[1] = Plane(Vector(-cosf(hFovRad), sinf(hFovRad), 0), 0);

	float vFovRad = atanf(aspectRatio * tanf(hFovRad));

	mPlanes[2] = Plane(Vector(0, sinf(vFovRad), cosf(vFovRad)), 0);
	mPlanes[3] = Plane(Vector(0, sinf(vFovRad), -cosf(vFovRad)), 0);
}

Frustum::Frustum(const Plane planes[4])
{
	for(int i=0; i<4; i++) {
		mPlanes[i] = planes[i];
	}
}

Frustum Frustum::rotateX(float angle)
{
	float anglef = angle * 3.14f / 180.0f;
	float cos = cosf(anglef);
	float sin = sinf(anglef);
	Plane planes[4];

	for(int i=0; i<4; i++) {
		const Vector &normal = mPlanes[i].normal();
		const Vector newNormal(normal.x(), normal.y() * cos + normal.z() * sin, normal.z() * cos - normal.y() * sin);
		planes[i] = Plane(newNormal, mPlanes[i].distance());
	}

	return Frustum(planes);
}

Frustum Frustum::rotateZ(float angle)
{
	float anglef = angle * 3.14f / 180.0f;
	float cos = cosf(anglef);
	float sin = sinf(anglef);
	Plane planes[4];

	for(int i=0; i<4; i++) {
		const Vector &normal = mPlanes[i].normal();
		const Vector newNormal(normal.x() * cos - normal.y() * sin, normal.x() * sin + normal.y() * cos, normal.z());
		planes[i] = Plane(newNormal, mPlanes[i].distance());
	}

	return Frustum(planes);
}

Frustum Frustum::translate(const Vector &disp)
{
	Plane planes[4];
	for(int i=0; i<4; i++) {
		float distance = mPlanes[i].distance() + disp * mPlanes[i].normal();
		planes[i] = Plane(mPlanes[i].normal(), distance);
	}

	return Frustum(planes);
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

			Vector point(x, y, z);
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