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

	float x() const { return mX; }
	float y() const { return mY; }
	float z() const { return mZ; }

	float operator*(const Vector &other) const {
		return mX * other.mX + mY * other.mY + mZ * other.mZ;
	}

	Vector operator*(float a) const {
		return Vector(mX * a, mY * a, mZ * a);
	}

	Vector operator+(const Vector &other) const {
		return Vector(mX + other.mX, mY + other.mY, mZ + other.mZ);
	}

private:
	float mX;
	float mY;
	float mZ;
};

static Vector operator*(float a, const Vector &other) {
	return other * a;
}

}
#endif