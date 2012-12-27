#ifndef GEO_BOX_ALIGNED_HPP
#define GEO_BOX_ALIGNED_HPP

#include "Geo/Vector.hpp"

namespace Geo {

class BoxAligned {
public:
	BoxAligned() {}
	BoxAligned(const Vector &minPoint, const Vector &maxPoint)
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