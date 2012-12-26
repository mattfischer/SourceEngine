#ifndef WORLD_MATERIAL_HPP
#define WORLD_MATERIAL_HPP

#include "File/IReaderFactory.hpp"
#include "File/VMT.hpp"
#include "Draw/Texture.hpp"

#include <string>

namespace World {

class Material {
public:
	Material(File::VMT *vmt, File::IReaderFactory *factory);

	Draw::Texture *texture() { return mTexture; }

private:
	Draw::Texture *mTexture;
};

}

#endif