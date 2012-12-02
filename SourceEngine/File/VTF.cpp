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

unsigned char *readDXT1(IReader *reader, int width, int height)
{
	int srcSize = width * height / 2;
	if(srcSize < 8) {
		srcSize = 8;
	}

	char *src = new char[srcSize];
	reader->read(src, srcSize);

	unsigned char *dst = new unsigned char[width * height * 4];

	int blockWidth = width / 4;
	int blockHeight = height / 4;
	for(int blockY=0; blockY<blockHeight; blockY++) {
		for(int blockX=0; blockX<blockWidth; blockX++) {
			int srcStart = (blockY * blockWidth + blockX) * 8;
			unsigned short c[4];
			c[0] = *(unsigned short*)(&src[srcStart + 0]);
			c[1] = *(unsigned short*)(&src[srcStart + 2]);

			if(c[0] > c[1]) {
				c[2] = 2 * c[0] / 3 + c[1] / 3;
				c[3] = c[0] / 3 + 2 * c[1] / 3;
			} else {
				c[2] = c[0] / 2 + c[1] / 2;
				c[3] = 0;
			}

			for(int j=0; j<4; j++) {
				for(int i=0; i<4; i++) {
					int x = blockX * 4 + i;
					int y = blockY * 4 + j;
					if(x > width || y > height) {
						continue;
					}

					int pos = j * 4 + i;
					char bits = 0x3 & (src[srcStart + pos / 4 + 4] >> (6 - 2 * (pos % 4)));
					dst[4 * (y * width + x) + 0] = 0x1f & (c[bits] >> 11);
					dst[4 * (y * width + x) + 1] = 0x3f & (c[bits] >> 5);
					dst[4 * (y * width + x) + 2] = 0x1f & (c[bits] >> 0);
					dst[4 * (y * width + x) + 3] = 0;
				}
			}
		}
	}

	delete[] src;
	return dst;
}

VTF *VTF::open(IReaderFactory *factory, const std::string &name)
{
	VTF *ret = 0;
	std::string filename = "materials/" + name + ".vtf";
	IReader *reader = factory->open(filename);

	if(reader) {
		ret = new VTF(reader);
		delete reader;
	}

	return ret;
}

VTF::VTF(IReader *reader)
{
	Header header;

	reader->read((char*)&header, sizeof(header));
	reader->seek(header.headerSize);

	mWidth = header.width;
	mHeight = header.height;
	mNumMipMaps = header.mipmapCount;

	mLowResData = readDXT1(reader, header.lowResImageWidth, header.lowResImageHeight);

	mData = new unsigned char*[mNumMipMaps];
	for(int i=0; i<mNumMipMaps; i++) {
		int width = mWidth >> (mNumMipMaps - i - 1);
		if(width == 0) {
			width = 1;
		}

		int height = mHeight >> (mNumMipMaps - i - 1);
		if(height == 0) {
			height = 1;
		}

		mData[i] = readDXT1(reader, width, height);
	}
}

}