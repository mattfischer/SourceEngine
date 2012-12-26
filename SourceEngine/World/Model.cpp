#include "World/Model.hpp"

namespace World {

Model::Model(File::MDL *mdl, File::VVD *vvd, File::VTX *vtx, File::IReaderFactory *factory)
{
	mMdl = mdl;
	mVvd = vvd;
	mVtx = vtx;

	mNumMaterials = mdl->numTextures();
	mMaterials = new Material*[mNumMaterials];
	for(int i=0; i<mNumMaterials; i++) {
		File::VMT *vmt = 0; //File::VMT::open(factory, "materials/" + mdl->texture(i) + ".vmt");
		if(vmt) {
			mMaterials[i] = new Material(vmt, factory);
		} else {
			mMaterials[i] = 0;
		}
	}
}

}