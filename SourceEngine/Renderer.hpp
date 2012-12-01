#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "SharedPointer.hpp"
#include "IReaderFactory.hpp"

class BSPFile;

class Renderer {
public:
	Renderer(BSPFile *bspFile, IReaderFactory *factory, int width, int height);

	void render();

	void rotate(int amount);
	void move(int amount);
	void rise(int amount);

private:
	BSPFile *mBspFile;
	float mX;
	float mY;
	float mZ;
	float mRotation;
};

#endif