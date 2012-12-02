#ifndef FILE_VTF_HPP
#define FILE_VTF_HPP

#include "File/IReaderFactory.hpp"

#include <string>

namespace File {

class VTF {
public:
	VTF(IReaderFactory *factory, const std::string &name);

	int width() { return mWidth; }
	int height() { return mHeight; }
	int numMipMaps() { return mNumMipMaps; }

	const char *lowResData() { return mLowResData; }
	const char *data(int mipMapLevel) { return mData[mipMapLevel]; }

private:
	int mWidth;
	int mHeight;
	int mNumMipMaps;

	char *mLowResData;
	char **mData;
};

}
#endif