#ifndef FILE_VTF_HPP
#define FILE_VTF_HPP

#include "File/Space.hpp"

#include <string>

namespace File {

class VTF {
public:
	enum Format {
		FormatDXT1,
		FormatDXT5,
		FormatUnknown
	};

	VTF(File *file);
	~VTF();

	int width(int n);
	int height(int n);
	int width() { return width(mNumMipMaps - 1); }
	int height() { return height(mNumMipMaps - 1); }
	int numMipMaps() { return mNumMipMaps; }
	Format format() { return mFormat; }

	const unsigned char *lowResData() { return mLowResData; }
	const unsigned char *data(int mipMapLevel) { return mData[mipMapLevel]; }
	int dataSize(int mipMapLevel);

	static VTF *open(Space *space, const std::string &filename);

private:
	int mWidth;
	int mHeight;
	int mNumMipMaps;
	Format mFormat;

	unsigned char *mLowResData;
	unsigned char **mData;
};

}
#endif