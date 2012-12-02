#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "File/IReaderFactory.hpp"
#include "File/BSP.hpp"

#include "Map.hpp"

class Renderer {
public:
	Renderer(Map *map, int width, int height);

	void render();

	void rotate(int amount);
	void move(int amount);
	void rise(int amount);

private:
	Map *mMap;

	float mX;
	float mY;
	float mZ;
	float mRotation;
};

#endif