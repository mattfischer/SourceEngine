#include "Draw/Texture.hpp"

namespace Draw {

Texture::Texture(Format::VTF::Header *vtf)
{
	glGenTextures(1, &mTex);
	glBindTexture(GL_TEXTURE_2D, mTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLenum internalFormat = 0;
	switch(vtf->highResImageFormat) {
		case Format::VTF::FormatDXT1:
			internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			break;

		case Format::VTF::FormatDXT5:
			internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
	}

	char *data = (char*)vtf + vtf->headerSize;
	data += Format::VTF::dataSize(vtf->lowResImageWidth, vtf->lowResImageHeight, vtf->lowResImageFormat);

	if(internalFormat != 0) {
		for(int i=0; i<vtf->mipmapCount; i++) {
			int mip = vtf->mipmapCount - i - 1;

			int width = vtf->width >> mip;
			if(width == 0) {
				width = 1;
			}

			int height = vtf->height >> mip;
			if(height == 0) {
				height = 1;
			}

			int dataSize = Format::VTF::dataSize(width, height, vtf->highResImageFormat);
			for(int frame = 0; frame<vtf->frames; frame++) {
				if(frame == 0) {
					glCompressedTexImage2D(GL_TEXTURE_2D, mip, internalFormat, width, height, 0, dataSize, data);
				}

				data += dataSize;
			}
		}
	}

	mWidth = vtf->width;
	mHeight = vtf->height;
}

void Texture::select()
{
	glBindTexture(GL_TEXTURE_2D, mTex);
	glColor3f(1.0f, 1.0f, 1.0f);
}

}