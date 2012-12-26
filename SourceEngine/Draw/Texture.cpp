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

	for(int i=0; i<vtf->numMipMaps(); i++) {
		glTexImage2D(GL_TEXTURE_2D, vtf->numMipMaps() - i - 1, GL_RGBA, vtf->width(i), vtf->height(i), 0, GL_RGBA, GL_UNSIGNED_BYTE, vtf->data(i));
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