#ifndef GEO_TRANSFORMATION_HPP
#define GEO_TRANSFORMATION_HPP

#include "Geo/Matrix.hpp"

namespace Geo {

class Vector;

class Transformation {
public:
	Transformation(const Matrix &matrix, const Matrix &inverse)
		: mMatrix(matrix), mInverse(inverse)
	{}

	Transformation(const Transformation &other)
		: mMatrix(other.mMatrix), mInverse(other.mInverse)
	{}

	Transformation &operator=(const Transformation &other)
	{
		mMatrix = other.mMatrix;
		mInverse = other.mInverse;

		return *this;
	}

	const Matrix &matrix() const { return mMatrix; }
	const Matrix &inverse() const { return mInverse; }

	Transformation operator*(const Transformation &other) const
	{
		return Transformation(mMatrix * other.mMatrix, other.mInverse * mInverse);
	}

	static Transformation identity();
	static Transformation translate(const Vector &vector);
	static Transformation rotateX(float angle);
	static Transformation rotateY(float angle);
	static Transformation rotateZ(float angle);

private:
	Matrix mMatrix;
	Matrix mInverse;
};

}
#endif