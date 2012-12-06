#ifndef FILE_VTF_HPP
#define FILE_VTF_HPP

#include "File/IReaderFactory.hpp"

#include <string>

namespace File {

class VTF {
public:
	VTF(IReader *reader);

	int width(int n);
	int height(int n);
	int width() { return width(mNumMipMaps - 1); }
	int height() { return height(mNumMipMaps - 1); }
	int numMipMaps() { return mNumMipMaps; }

	const unsigned char *lowResData() { return mLowResData; }
	const unsigned char *data(int mipMapLevel) { return mData[mipMapLevel]; }

	static VTF *open(IReaderFactory *factory, const std::string &name);

private:
	int mWidth;
	int mHeight;
	int mNumMipMaps;

	unsigned char *mLowResData;
	unsigned char **mData;
};

}
#endif