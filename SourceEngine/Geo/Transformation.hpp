#ifndef GEO_TRANSFORMATION_HPP
#define GEO_TRANSFORMATION_HPP

#include "Geo/Matrix.hpp"

namespace Geo {

class Vector;
class Orientation;

class Transformation {
public:
	Transformation(const Matrix &matrix, const Matrix &inverseMatrix)
		: mMatrix(matrix), mInverseMatrix(inverseMatrix)
	{}

	Transformation(const Transformation &other)
		: mMatrix(other.mMatrix), mInverseMatrix(other.mInverseMatrix)
	{}

	Transformation &operator=(const Transformation &other)
	{
		mMatrix = other.mMatrix;
		mInverseMatrix = other.mInverseMatrix;

		return *this;
	}

	const Matrix &matrix() const { return mMatrix; }
	const Matrix &inverseMatrix() const { return mInverseMatrix; }

	Transformation inverse() const { return Transformation(mInverseMatrix, mMatrix); }

	Transformation operator*(const Transformation &other) const
	{
		return Transformation(mMatrix * other.mMatrix, other.mInverseMatrix * mInverseMatrix);
	}

	static Transformation identity();
	static Transformation translate(const Vector &vector);
	static Transformation rotateX(float angle);
	static Transformation rotateY(float angle);
	static Transformation rotateZ(float angle);
	static Transformation rotate(const Geo::Orientation &orientation);
	static Transformation translateRotate(const Geo::Vector &vector, const Geo::Orientation &orientation);

private:
	Matrix mMatrix;
	Matrix mInverseMatrix;
};

}
#endif