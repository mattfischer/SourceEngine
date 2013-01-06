#ifndef WORLD_SKYBOX_HPP
#define WORLD_SKYBOX_HPP

#include "File/Space.hpp"

#include "World/Material.hpp"

#include <string>

namespace World {

class Skybox {
public:
	enum Surface {
		SurfaceUp,
		SurfaceDown,
		SurfaceLeft,
		SurfaceRight,
		SurfaceFront,
		SurfaceBack
	};

	Skybox(File::Space *space, const std::string &name);

	Material *material(Surface surface) { return mMaterials[surface]; }

private:
	Material *mMaterials[6];
};

}
#endif