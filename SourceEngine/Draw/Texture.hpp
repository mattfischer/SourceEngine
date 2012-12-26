#ifndef DRAW_TEXTURE_HPP
#define DRAW_TEXTURE_HPP

#include "File/VTF.hpp"

#include <windows.h>
#include "GL/gl.h"

#include <string>

namespace Draw {

class Texture {
public:
	Texture(File::VTF *vtf);

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