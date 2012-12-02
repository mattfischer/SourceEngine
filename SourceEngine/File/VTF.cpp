#include "File/VTF.hpp"

namespace File {

#pragma pack(push, 1)
struct Header
{
	char			signature[4];
	unsigned int	version[2];
	unsigned int	headerSize;
	unsigned short	width;
	unsigned short	height;
	unsigned int	flags;
	unsigned short	frames;
	unsigned short	firstFrame;
	unsigned char	padding0[4];
	float			reflectivity[3];
	unsigned char	padding1[4];
	float			bumpmapScale;
	unsigned int	highResImageFormat;
	unsigned char	mipmapCount;
	unsigned int	lowResImageFormat;
	unsigned char	lowResImageWidth;
	unsigned char	lowResImageHeight;
	unsigned short	depth;
};
#pragma pack(pop)

VTF::VTF(IReaderFactory *factory, const std::string &name)
{
	std::string filename = "materials/" + name + ".vtf";
	IReader *reader = factory->open(filename);
	Header header;

	reader->read((char*)&header, sizeof(header));
	reader->seek(header.headerSize);

	mWidth = header.width;
	mHeight = header.height;
	mNumMipMaps = header.mipmapCount;

	int size = header.lowResImageWidth * header.lowResImageHeight / 2;
	mLowResData = new char[size];
	reader->read(mLowResData, size);

	mData = new char*[mNumMipMaps];
	int width = mWidth;
	int height = mHeight;
	for(int i=0; i<mNumMipMaps - 1; i++) {
		width /= 2;
		height /= 2;
	}

	for(int i=0; i<mNumMipMaps; i++) {
		int size = width * height / 2;
		if(size < 8) {
			size = 8;
		}

		mData[i] = new char[size];
		reader->read(mData[i], size);

		width *= 2;
		height *= 2;
	}
}

}