#include "World/Model.hpp"

namespace World {

Model::Model(Format::MDL *mdl, Format::VVD *vvd, Format::VTX *vtx, File::Space *space, const std::string &modelPath)
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
		Format::VMT *vmt = Format::VMT::open(space, "materials/" + modelPath + "/" + mdl->texture(i) + ".vmt");
		if(!vmt) {
			vmt = Format::VMT::open(space, "materials/" + mdl->texture(i) + ".vmt");
		}

		if(vmt) {
			mMaterials[i] = new Material(vmt, space);
		} else {
			mMaterials[i] = 0;
		}
	}
}

}