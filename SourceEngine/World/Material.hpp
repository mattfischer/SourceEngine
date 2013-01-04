#ifndef WORLD_MATERIAL_HPP
#define WORLD_MATERIAL_HPP

#include "File/Space.hpp"

#include "Format/VMT.hpp"

#include "Draw/Texture.hpp"

#include <string>

namespace World {

class Material {
public:
	Material(Format::VMT *vmt, File::Space *space);

	Draw::Texture *texture() { return mTexture; }

private:
	Draw::Texture *mTexture;
};

}

#endif