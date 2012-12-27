#ifndef GEO_BOX_ORIENTED_HPP
#define GEO_BOX_ORIENTED_HPP

#include "Geo/Point.hpp"
#include "Geo/Vector.hpp"
#include "Geo/Transformation.hpp"

namespace Geo {

class BoxOriented {
public:
	BoxOriented() {}

	BoxOriented(const Point &point, const Vector vectors[3])
	{
		mPoint = point;
		for(int i=0; i<3; i++) {
			mVectors[i] = vectors[i];
		}
	}

	BoxOriented(const Point &min, const Point &max)
	{
		mPoint = min;
		mVectors[0] = Vector(max.x() - min.x(), 0, 0);
		mVectors[1] = Vector(0, max.y() - min.y(), 0);
		mVectors[2] = Vector(0, 0, max.z() - min.z());
	}

	const Point &point() const { return mPoint; }
	const Vector &vector(int vector) const { return mVectors[vector]; }

private:
	Point mPoint;
	Vector mVectors[3];
};

static BoxOriented operator*(const Transformation &transformation, const BoxOriented &box)
{
	Point point = transformation * box.point();
	Vector vectors[3];
	for(int i=0; i<3; i++) {
		vectors[i] = transformation * box.vector(i);
	}

	return BoxOriented(point, vectors);
}

}

#endif