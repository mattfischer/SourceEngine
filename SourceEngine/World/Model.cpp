#include "World/Model.hpp"

namespace World {

Model::Model(Format::MDL *mdl, Format::VVD::Header *vvd, Format::VTX::Header *vtx, File::Space *space, const std::string &modelPath)
{
	mMdl = mdl;

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

	Format::VTX::BodyPart *bodyPart = vtx->bodyPart(0);
	Format::VTX::Model *model = bodyPart->model(0);
	mNumLods = model->numLods;
	mLods = new Lod[mNumLods];
	for(int i=0; i<mNumLods; i++) {
		Format::VTX::Lod *lod = model->lod(i);
		int numIndices = 0;
		for(int j=0; j<lod->numMeshes; j++) {
			Format::VTX::Mesh *mesh = lod->mesh(j);
			for(int k=0; k<mesh->numStripGroups; k++) {
				numIndices += mesh->stripGroup(k)->numIndices;
			}
		}

		mLods[i].numIndices = numIndices;
		mLods[i].indices = new unsigned short[numIndices];

		int index = 0;
		int vertexOffset = 0;
		for(int j=0; j<lod->numMeshes; j++) {
			Format::VTX::Mesh *mesh = lod->mesh(j);
			for(int k=0; k<mesh->numStripGroups; k++) {
				Format::VTX::StripGroup *stripGroup = mesh->stripGroup(k);
				for(int l=0; l<stripGroup->numStrips; l++) {
					Format::VTX::Strip *strip = stripGroup->strip(l);
					for(int m=0; m<strip->numIndices; m++) {
						unsigned short x = strip->vertexOffset + *stripGroup->index(strip->indexOffset + m);
						mLods[i].indices[index] = stripGroup->vertex(x)->vertex + vertexOffset;
						index++;
					}
				}

				vertexOffset += stripGroup->numVertices;
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