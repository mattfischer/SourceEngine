#include "Geo/Transformation.hpp"

#include "Geo/Vector.hpp"

#include <math.h>

namespace Geo {

Transformation Transformation::identity()
{
	float m[4][4];
	float i[4][4];

	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

	i[0][0] = 1.0f; i[0][1] = 0.0f; i[0][2] = 0.0f; i[0][3] = 0.0f;
	i[1][0] = 0.0f; i[1][1] = 1.0f; i[1][2] = 0.0f; i[1][3] = 0.0f;
	i[2][0] = 0.0f; i[2][1] = 0.0f; i[2][2] = 1.0f; i[2][3] = 0.0f;
	i[3][0] = 0.0f; i[3][1] = 0.0f; i[3][2] = 0.0f; i[3][3] = 1.0f;

	return Transformation(m, i);
}

Transformation Transformation::translate(const Vector &vector)
{
	float m[4][4];
	float i[4][4];

	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = vector.x();
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = vector.y();
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = vector.z();
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

	i[0][0] = 1.0f; i[0][1] = 0.0f; i[0][2] = 0.0f; i[0][3] = -vector.x();
	i[1][0] = 0.0f; i[1][1] = 1.0f; i[1][2] = 0.0f; i[1][3] = -vector.y();
	i[2][0] = 0.0f; i[2][1] = 0.0f; i[2][2] = 1.0f; i[2][3] = -vector.z();
	i[3][0] = 0.0f; i[3][1] = 0.0f; i[3][2] = 0.0f; i[3][3] = 1.0f;

	return Transformation(m, i);
}

Transformation Transformation::rotateX(float angle)
{
	float m[4][4];
	float i[4][4];

	float c = cosf(angle * 3.14f / 180.0f);
	float s = sinf(angle * 3.14f / 180.0f);

	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = c;    m[1][2] = -s;   m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = s;    m[2][2] = c;    m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

	i[0][0] = 1.0f; i[0][1] = 0.0f; i[0][2] = 0.0f; i[0][3] = 0.0f;
	i[1][0] = 0.0f; i[1][1] = c;    i[1][2] = s;    i[1][3] = 0.0f;
	i[2][0] = 0.0f; i[2][1] = -s;   i[2][2] = c;    i[2][3] = 0.0f;
	i[3][0] = 0.0f; i[3][1] = 0.0f; i[3][2] = 0.0f; i[3][3] = 1.0f;

	return Transformation(m, i);
}

Transformation Transformation::rotateY(float angle)
{
	float m[4][4];
	float i[4][4];

	float c = cosf(angle * 3.14f / 180.0f);
	float s = sinf(angle * 3.14f / 180.0f);

	m[0][0] = c;    m[0][1] = 0.0f; m[0][2] = s;    m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = -s;   m[2][1] = 0.0f; m[2][2] = c;    m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

	i[0][0] = c;    i[0][1] = 0.0f; i[0][2] = -s;   i[0][3] = 0.0f;
	i[1][0] = 0.0f; i[1][1] = 1.0f; i[1][2] = 0.0f; i[1][3] = 0.0f;
	i[2][0] = s;    i[2][1] = 0.0f; i[2][2] = c;    i[2][3] = 0.0f;
	i[3][0] = 0.0f; i[3][1] = 0.0f; i[3][2] = 0.0f; i[3][3] = 1.0f;

	return Transformation(m, i);
}

Transformation Transformation::rotateZ(float angle)
{
	float m[4][4];
	float i[4][4];

	float c = cosf(angle * 3.14f / 180.0f);
	float s = sinf(angle * 3.14f / 180.0f);

	m[0][0] = c;    m[0][1] = -s;   m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = s;    m[1][1] = c;    m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

	i[0][0] = c;    i[0][1] = s;    i[0][2] = 0.0f; i[0][3] = 0.0f;
	i[1][0] = -s;   i[1][1] = c;    i[1][2] = 0.0f; i[1][3] = 0.0f;
	i[2][0] = 0.0f; i[2][1] = 0.0f; i[2][2] = 1.0f; i[2][3] = 0.0f;
	i[3][0] = 0.0f; i[3][1] = 0.0f; i[3][2] = 0.0f; i[3][3] = 1.0f;

	return Transformation(m, i);
}

}