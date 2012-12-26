#ifndef GEO_ORIENTATION_HPP
#define GEO_ORIENTATION_HPP

namespace Geo {

class Orientation {
public:
	Orientation() {
		mElements[0] = 0;
		mElements[1] = 0;
		mElements[2] = 0;
	}

	Orientation(float pitch, float yaw, float roll)
	{
		mElements[0] = pitch;
		mElements[1] = yaw;
		mElements[2] = roll;
	}

	float pitch() const { return mElements[0]; }
	float yaw() const { return mElements[1]; }
	float roll() const { return mElements[2]; }

private:
	float mElements[3];
};

}

#endif