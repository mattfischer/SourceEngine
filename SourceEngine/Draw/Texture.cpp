#include "Draw/Texture.hpp"

namespace Draw {

Texture::Texture(File::VTF *vtf)
{
	glGenTextures(1, &mTex);
	glBindTexture(GL_TEXTURE_2D, mTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	int numMip = vtf->numMipMaps() - 2;
	if(numMip < 6) {
		numMip = 6;
	}

	for(int i=0; i<numMip; i++) {
		int mip = numMip - i - 1;
		glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, vtf->width(mip), vtf->height(mip), 0, GL_RGBA, GL_UNSIGNED_BYTE, vtf->data(mip));
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