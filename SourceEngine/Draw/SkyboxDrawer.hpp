#ifndef DRAW_SKYBOX_DRAWER_HPP
#define DRAW_SKYBOX_DRAWER_HPP

#include "World/Skybox.hpp"

namespace Draw {

class SkyboxDrawer {
public:
	void draw(World::Skybox *skybox);
};

}

#endif