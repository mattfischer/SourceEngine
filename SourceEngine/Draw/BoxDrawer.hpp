#ifndef DRAW_BOX_DRAWER_HPP
#define DRAW_BOX_DRAWER_HPP

#include "Geo/BoxAligned.hpp"
#include "Geo/BoxOriented.hpp"

namespace Draw {

class BoxDrawer {
public:
	void draw(const Geo::BoxAligned &box);
	void draw(const Geo::BoxOriented &box);

private:
};

}

#endif