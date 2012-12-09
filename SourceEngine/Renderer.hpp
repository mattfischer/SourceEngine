#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "File/IReaderFactory.hpp"
#include "File/BSP.hpp"
#include "Geo/Vector.hpp"

#include "Map.hpp"

class Renderer {
public:
	Renderer(Map *map, int width, int height);

	void render();

	void rotate(int yaw, int pitch);
	void move(int amount);
	void rise(int amount);

private:
	Map *mMap;

	Geo::Vector mPosition;
	float mYaw;
	float mPitch;
};

#endif