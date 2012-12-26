#ifndef GEO_COORDINATE_HPP
#define GEO_COORDINATE_HPP

namespace Geo {

class Coordinate {
public:
	Coordinate()
	{
		mElements[0] = 0;
		mElements[1] = 0;
	}

	Coordinate(float u, float v)
	{
		mElements[0] = u;
		mElements[1] = v;
	}

	float u() const { return mElements[0]; }
	float v() const { return mElements[1]; }

private:
	float mElements[2];
};

}

#endif