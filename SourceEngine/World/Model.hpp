#ifndef WORLD_MODEL_HPP
#define WORLD_MODEL_HPP

#include "File/MDL.hpp"
#include "File/VTX.hpp"
#include "File/VVD.hpp"
#include "File/IReaderFactory.hpp"

#include "Geo/BoxOriented.hpp"

#include "World/Material.hpp"

namespace World {

class Model {
public:
	Model(File::MDL *mdl, File::VVD *vvd, File::VTX *vtx, File::IReaderFactory *factory, const std::string &modelPath);

	File::MDL *mdl() { return mMdl; }
	File::VVD *vvd() { return mVvd; }
	File::VTX *vtx() { return mVtx; }

	int numMaterials() { return mNumMaterials; }
	Material *material(int material) { return mMaterials[material]; }

	const Geo::BoxOriented &box() { return mBox; }

private:
	File::MDL *mMdl;
	File::VVD *mVvd;
	File::VTX *mVtx;

	int mNumMaterials;
	Material **mMaterials;

	Geo::BoxOriented mBox;
};

}

#endif