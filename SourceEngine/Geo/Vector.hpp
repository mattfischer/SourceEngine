#ifndef GEO_VECTOR_HPP
#define GEO_VECTOR_HPP

namespace Geo {

class Vector {
public:
	Vector()
		: mX(0), mY(0), mZ(0)
	{}

	Vector(float x, float y, float z)
		: mX(x), mY(y), mZ(z)
	{}

	float x() { return mX; }
	float y() { return mY; }
	float z() { return mZ; }

private:
	float mX;
	float mY;
	float mZ;
};

}
#endif