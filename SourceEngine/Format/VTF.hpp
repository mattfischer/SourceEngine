#ifndef FORMAT_VTF_HPP
#define FORMAT_VTF_HPP

#include "File/Space.hpp"

#include <string>

namespace Format {
namespace VTF {

const unsigned int FormatDXT1 = 0xd;
const unsigned int FormatDXT5 = 0xf;

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

static int dataSize(int width, int height, unsigned int format)
{
	int size = 0;
	switch(format) {
		case FormatDXT1:
			size = width * height / 2;
			if(size < 8) {
				size = 8;
			}
			break;

		case FormatDXT5:
			size = width * height;
			if(size < 16) {
				size = 16;
			}
			break;
	}

	return size;
}

static Header *open(File::Space *space, const std::string &name)
{
	return (Header*)space->read(name);
}

}
}
#endif