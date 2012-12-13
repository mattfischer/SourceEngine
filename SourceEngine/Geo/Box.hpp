#ifndef GEO_BOX_HPP
#define GEO_BOX_HPP

#include "Geo/Vector.hpp"

namespace Geo {

class Box {
public:
	Box() {}
	Box(const Vector &minPoint, const Vector &maxPoint)
		: mMinPoint(minPoint), mMaxPoint(maxPoint)
	{}

	const Vector &minPoint() const { return mMinPoint; }
	const Vector &maxPoint() const { return mMaxPoint; }

private:
	Vector mMinPoint;
	Vector mMaxPoint;
};

}
#endif