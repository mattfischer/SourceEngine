#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "File/IReaderFactory.hpp"
#include "File/BSP.hpp"

class Renderer {
public:
	Renderer(File::BSP *bspFile, File::IReaderFactory *factory, int width, int height);

	void render();

	void rotate(int amount);
	void move(int amount);
	void rise(int amount);

private:
	File::BSP *mBspFile;
	float mX;
	float mY;
	float mZ;
	float mRotation;
};

#endif