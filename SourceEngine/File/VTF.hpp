#ifndef FILE_VTF_HPP
#define FILE_VTF_HPP

#include "File/IReaderFactory.hpp"

#include <string>

namespace File {

class VTF {
public:
	VTF(IReader *reader);

	int width() { return mWidth; }
	int height() { return mHeight; }
	int numMipMaps() { return mNumMipMaps; }

	const unsigned short *lowResData() { return mLowResData; }
	const unsigned short *data(int mipMapLevel) { return mData[mipMapLevel]; }

	static VTF *open(IReaderFactory *factory, const std::string &name);

private:
	int mWidth;
	int mHeight;
	int mNumMipMaps;

	unsigned short *mLowResData;
	unsigned short **mData;
};

}
#endif