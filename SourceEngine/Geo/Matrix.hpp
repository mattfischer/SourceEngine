#ifndef GEO_MATRIX_HPP
#define GEO_MATRIX_HPP

namespace Geo {

class Matrix {
public:
	Matrix(const float elements[4][4])
	{
		for(int i=0; i<4; i++) {
			for(int j=0; j<4; j++) {
				mElements[i][j] = elements[i][j];
			}
		}
	}

	Matrix(const Matrix &other)
	{
		for(int i=0; i<4; i++) {
			for(int j=0; j<4; j++) {
				mElements[i][j] = other.mElements[i][j];
			}
		}
	}

	Matrix &operator=(const Matrix &other)
	{
		for(int i=0; i<4; i++) {
			for(int j=0; j<4; j++) {
				mElements[i][j] = other.mElements[i][j];
			}
		}

		return *this;
	}

	float operator()(int i, int j) const { return mElements[i][j]; }

	Matrix operator*(const Matrix &other) const
	{
		float elements[4][4];

		for(int i=0; i<4; i++) {
			for(int j=0; j<4; j++) {
				elements[i][j] = 0;
				for(int k=0; k<4; k++) {
					elements[i][j] += mElements[i][k] * other.mElements[k][j];
				}
			}
		}

		return Matrix(elements);
	}

public:
	float mElements[4][4];
};

}
#endif