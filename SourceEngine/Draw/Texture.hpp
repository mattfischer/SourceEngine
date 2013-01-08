#ifndef DRAW_TEXTURE_HPP
#define DRAW_TEXTURE_HPP

#include "Format/VTF.hpp"

#include "GL/glew.h"

#include <string>

namespace Draw {

class Texture {
public:
	Texture(Format::VTF::Header *vtf);

	void select();

	int width() { return mWidth; }
	int height() { return mHeight; }

private:
	GLuint mTex;

	int mWidth;
	int mHeight;
};

}

#endif