#include "World/Model.hpp"

namespace World {

Model::Model(Format::MDL *mdl, Format::VVD::Header *vvd, Format::VTX *vtx, File::Space *space, const std::string &modelPath)
{
	mMdl = mdl;
	mVtx = vtx;

	mVertices = new Format::VVD::Vertex*[vvd->numLods];
	for(int lod=0; lod<vvd->numLods; lod++) {
		int numLodVertices = vvd->numLodVertices[lod];

		mVertices[lod] = new Format::VVD::Vertex[numLodVertices];
		if(vvd->numFixups == 0) {
			for(int v=0; v<numLodVertices; v++) {
				mVertices[lod][v] = *vvd->vertex(v);
			}
		} else {
			int v = 0;
			for(int i=0; i<vvd->numFixups; i++) {
				Format::VVD::Fixup *fixup = vvd->fixup(i);
				if(lod <= fixup->lod) {
					for(int j=0; j<fixup->numVertices; j++) {
						mVertices[lod][v] = *vvd->vertex(fixup->sourceVertexId + j);
						v++;
					}
				}
			}
		}
	}

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