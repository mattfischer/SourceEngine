#include "Draw/Texture.hpp"

namespace Draw {

Texture::Texture(Format::VTF *vtf)
{
	glGenTextures(1, &mTex);
	glBindTexture(GL_TEXTURE_2D, mTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLenum internalFormat;
	switch(vtf->format()) {
		case Format::VTF::FormatDXT1:
			internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			break;

		case Format::VTF::FormatDXT5:
			internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
	}

	if(vtf->format() != Format::VTF::FormatUnknown) {
		for(int i=0; i<vtf->numMipMaps(); i++) {
			int mip = vtf->numMipMaps() - i - 1;
			glCompressedTexImage2D(GL_TEXTURE_2D, i, internalFormat, vtf->width(mip), vtf->height(mip), 0, vtf->dataSize(mip), vtf->data(mip));
		}
	}

	mWidth = vtf->width();
	mHeight = vtf->height();
}

void Texture::select()
{
	glBindTexture(GL_TEXTURE_2D, mTex);
	glColor3f(1.0f, 1.0f, 1.0f);
}

}