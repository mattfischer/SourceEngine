#include "World/Model.hpp"

namespace World {

Model::Model(Format::MDL::Header *mdl, Format::VVD::Header *vvd, Format::VTX::Header *vtx, File::Space *space, const std::string &modelPath)
{
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

	mNumBodyParts = vtx->numBodyParts;
	mBodyParts = new BodyPart[mNumBodyParts];
	for(int bp=0; bp<vtx->numBodyParts; bp++) {
		Format::VTX::BodyPart *vtxBodyPart = vtx->bodyPart(bp);
		Format::MDL::BodyPart *mdlBodyPart = mdl->bodyPart(bp);
		BodyPart &bodyPart = mBodyParts[bp];
		bodyPart.numModels = vtxBodyPart->numModels;
		bodyPart.models = new _Model[bodyPart.numModels];
		for(int m=0; m<vtxBodyPart->numModels; m++) {
			Format::VTX::Model *vtxModel = vtxBodyPart->model(m);
			Format::MDL::Model *mdlModel = mdlBodyPart->model(m);
			_Model &model = bodyPart.models[m];
			model.numLods = vtxModel->numLods;
			model.lods = new Lod[model.numLods];
			for(int l=0; l<vtxModel->numLods; l++) {
				Format::VTX::Lod *vtxLod = vtxModel->lod(l);
				Lod &lod = model.lods[l];
				lod.numMeshes = vtxLod->numMeshes;
				lod.meshes = new Mesh[lod.numMeshes];
				for(int me=0; me<vtxLod->numMeshes; me++) {
					Format::VTX::Mesh *vtxMesh = vtxLod->mesh(me);
					Format::MDL::Mesh *mdlMesh = mdlModel->mesh(me);
					Mesh &mesh = lod.meshes[me];
					mesh.numStripGroups = vtxMesh->numStripGroups;
					mesh.stripGroups = new StripGroup[mesh.numStripGroups];
					for(int sg=0; sg<vtxMesh->numStripGroups; sg++) {
						Format::VTX::StripGroup *vtxStripGroup = vtxMesh->stripGroup(sg);
						StripGroup &stripGroup = mesh.stripGroups[sg];
						stripGroup.numStrips = vtxStripGroup->numStrips;
						stripGroup.strips = new Strip[stripGroup.numStrips];
						for(int s=0; s<vtxStripGroup->numStrips; s++) {
							Format::VTX::Strip *vtxStrip = vtxStripGroup->strip(s);
							Strip &strip = stripGroup.strips[s];
							strip.numIndices = vtxStrip->numIndices;
							strip.indices = new unsigned short[strip.numIndices];
							for(int i=0; i<vtxStrip->numIndices; i++) {
								unsigned short x = vtxStrip->vertexOffset + *vtxStripGroup->index(vtxStrip->indexOffset + i);
								strip.indices[i] = vtxStripGroup->vertex(x)->vertex + mdlMesh->vertexOffset;
							}
						}
					}
				}
			}
		}
	}

	Geo::Point hullMin(mdl->hullMin.x, mdl->hullMin.y, mdl->hullMin.z);
	Geo::Point hullMax(mdl->hullMax.x, mdl->hullMax.y, mdl->hullMax.z);
	mBox = Geo::BoxOriented(hullMin, hullMax);

	mNumMaterials = mdl->textureCount;
	mMaterials = new Material*[mNumMaterials];
	for(int i=0; i<mNumMaterials; i++) {
		std::string texture(mdl->texture(i)->name());
		Format::VMT *vmt = Format::VMT::open(space, "materials/" + modelPath + "/" + texture + ".vmt");
		if(!vmt) {
			vmt = Format::VMT::open(space, "materials/" + texture + ".vmt");
		}

		if(vmt) {
			mMaterials[i] = new Material(vmt, space);
		} else {
			mMaterials[i] = 0;
		}
	}
}

}