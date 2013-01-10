#ifndef GEO_VECTOR_HPP
#define GEO_VECTOR_HPP

#include "Geo/Matrix.hpp"
#include "Geo/Transformation.hpp"

namespace Geo {

class Vector {
public:
	Vector()
	{
		mElements[0] = 0;
		mElements[1] = 0;
		mElements[2] = 0;
	}

	Vector(float x, float y, float z)
	{
		mElements[0] = x;
		mElements[1] = y;
		mElements[2] = z;
	}

	Vector(const float elements[3])
	{
		for(int i=0; i<3; i++) {
			mElements[i] = elements[i];
		}
	}

	Vector(const Vector &other)
	{
		for(int i=0; i<3; i++) {
			mElements[i] = other.mElements[i];
		}
	}

	Vector &operator=(const Vector &other)
	{
		for(int i=0; i<3; i++) {
			mElements[i] = other.mElements[i];
		}

		return *this;
	}

	float operator() (int n) const { return mElements[n]; }
	float x() const { return mElements[0]; }
	float y() const { return mElements[1]; }
	float z() const { return mElements[2]; }

	float magnitude2() const { return mElements[0] * mElements[0] + mElements[1] * mElements[1] + mElements[2] * mElements[2]; }

	Vector operator+(const Vector &other) const
	{
		return Vector(x() + other.x(), y() + other.y(), z() + other.z());
	}

	Vector operator-(const Vector &other) const
	{
		return Vector(x() - other.x(), y() - other.y(), z() - other.z());
	}

	Vector operator*(float a) const
	{
		return Vector(x() * a, y() * a, z() * a);
	}

	float operator*(const Vector &other) const
	{
		return x() * other.x() + y() * other.y() + z() * other.z();
	}

private:
	float mElements[3];
};

static Vector operator*(float a, const Vector &vector)
{
	return vector * a;
}

static Vector operator*(const Matrix &matrix, const Vector &point)
{
	float elements[3];

	for(int i=0; i<3; i++) {
		elements[i] = 0;
		for(int j=0; j<3; j++) {
			elements[i] += matrix(i, j) * point(j);
		}
	}

	return Vector(elements);
}

static Vector operator*(const Transformation &transformation, const Vector &vector)
{
	return transformation.matrix() * vector;
}

}
#endif