#include "World/Model.hpp"

namespace World {

Model::Model(File::MDL *mdl, File::VVD *vvd, File::VTX *vtx, File::IReaderFactory *factory, const std::string &modelPath)
{
	mMdl = mdl;
	mVvd = vvd;
	mVtx = vtx;

	Geo::Point hullMin(mdl->hullMin().x, mdl->hullMin().y, mdl->hullMin().z);
	Geo::Point hullMax(mdl->hullMax().x, mdl->hullMax().y, mdl->hullMax().z);
	mBox = Geo::BoxOriented(hullMin, hullMax);

	mNumMaterials = mdl->numTextures();
	mMaterials = new Material*[mNumMaterials];
	for(int i=0; i<mNumMaterials; i++) {
		File::VMT *vmt = File::VMT::open(factory, "materials/" + modelPath + "/" + mdl->texture(i) + ".vmt");
		if(!vmt) {
			vmt = File::VMT::open(factory, "materials/" + mdl->texture(i) + ".vmt");
		}

		if(vmt) {
			mMaterials[i] = new Material(vmt, factory);
		} else {
			mMaterials[i] = 0;
		}
	}
}

}