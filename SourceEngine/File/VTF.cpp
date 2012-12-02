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

unsigned short interp565(unsigned short a, unsigned short b, int num, int den)
{
	unsigned short ra = 0x1f & (a >> 11);
	unsigned short rb = 0x1f & (b >> 11);
	unsigned short _r = ra * num / den + (den - num) * rb / den;

	unsigned short ga = 0x3f & (a >> 5);
	unsigned short gb = 0x3f & (b >> 5);
	unsigned short _g = ga * num / den + (den - num) * gb / den;

	unsigned short ba = 0x1f & a;
	unsigned short bb = 0x1f & b;
	unsigned short _b = ba * num / den + (den - num) * bb / den;

	return (_r << 11) | (_g << 5) | _b;
}

unsigned int unpack565(unsigned short c)
{
	unsigned int clong = c;
	return 0xff000000 | ((clong << 19) & 0x00f80000) | ((clong << 5) & 0x0000fc00) | 
		   ((clong >> 8) & 0x000000f8);
}

unsigned int interp8888(unsigned int a, unsigned int b, int num, int den)
{
	unsigned int ra = 0xff & (a >> 16);
	unsigned int rb = 0xff & (b >> 16);
	unsigned int _r = ra * num / den + (den - num) * rb / den;

	unsigned int ga = 0xff & (a >> 8);
	unsigned int gb = 0xff & (b >> 8);
	unsigned int _g = ga * num / den + (den - num) * gb / den;

	unsigned int ba = 0xff & (a >> 0);
	unsigned int bb = 0xff & (b >> 0);
	unsigned int _b = ba * num / den + (den - num) * bb / den;

	return 0xff000000 | (_r << 16) | (_g << 8) | (_b << 0);
}

unsigned char *readDXT1(IReader *reader, int width, int height)
{
	int srcSize = width * height / 2;
	if(srcSize < 8) {
		srcSize = 8;
	}

	unsigned char *src = new unsigned char[srcSize];
	reader->read((char*)src, srcSize);

	unsigned int *dst = new unsigned int[width * height];

	int blockWidth = width / 4;
	int blockHeight = height / 4;
	for(int blockY=0; blockY<blockHeight; blockY++) {
		for(int blockX=0; blockX<blockWidth; blockX++) {
			int srcStart = (blockY * blockWidth + blockX) * 8;
			unsigned short c0 = *(unsigned short*)(&src[srcStart + 0]);
			unsigned short c1 = *(unsigned short*)(&src[srcStart + 2]);
			unsigned int c[4];
			c[0] = unpack565(c0);
			c[1] = unpack565(c1);

			if(c0 > c1) {
				c[2] = interp8888(c[0], c[1], 2, 3);
				c[3] = interp8888(c[0], c[1], 1, 3);
			} else {
				c[2] = interp8888(c[0], c[1], 1, 2);
				c[3] = 0;
			}

			for(int j=0; j<4; j++) {
				for(int i=0; i<4; i++) {
					int x = blockX * 4 + i;
					int y = blockY * 4 + j;
					if(x >= width || y >= height) {
						continue;
					}

					int pos = j * 4 + i;
					unsigned int bits = *(unsigned int*)&src[srcStart + 4];
					dst[y * width + x] = c[0x3 & (bits >> (pos * 2))];
				}
			}
		}
	}

	delete[] src;
	return (unsigned char*)dst;
}

unsigned char *readDXT5(IReader *reader, int width, int height)
{
	int srcSize = width * height;
	if(srcSize < 16) {
		srcSize = 16;
	}

	unsigned char *src = new unsigned char[srcSize];
	reader->read((char*)src, srcSize);

	unsigned int *dst = new unsigned int[width * height];

	int blockWidth = width / 4;
	int blockHeight = height / 4;
	for(int blockY=0; blockY<blockHeight; blockY++) {
		for(int blockX=0; blockX<blockWidth; blockX++) {
			int srcStart = (blockY * blockWidth + blockX) * 16 + 8;
			unsigned short c0 = *(unsigned short*)(&src[srcStart + 0]);
			unsigned short c1 = *(unsigned short*)(&src[srcStart + 2]);
			unsigned int c[4];
			c[0] = unpack565(c0);
			c[1] = unpack565(c1);

			if(c0 > c1) {
				c[2] = interp8888(c[0], c[1], 2, 3);
				c[3] = interp8888(c[0], c[1], 1, 3);
			} else {
				c[2] = interp8888(c[0], c[1], 1, 2);
				c[3] = 0;
			}

			for(int j=0; j<4; j++) {
				for(int i=0; i<4; i++) {
					int x = blockX * 4 + i;
					int y = blockY * 4 + j;
					if(x >= width || y >= height) {
						continue;
					}

					int pos = j * 4 + i;
					unsigned int bits = *(unsigned int*)&src[srcStart + 4];
					dst[y * width + x] = c[0x3 & (bits >> (pos * 2))];
				}
			}
		}
	}

	delete[] src;
	return (unsigned char*)dst;
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

		switch(header.highResImageFormat) {
			case 0xd:
				mData[i] = readDXT1(reader, width, height);
				break;

			case 0xf:
				mData[i] = readDXT5(reader, width, height);
				break;

			default:
				mData[i] = 0;
				break;
		}
	}
}

}