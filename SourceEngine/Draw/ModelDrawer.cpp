#include "Draw/ModelDrawer.hpp"

#include <GL/glew.h>

namespace Draw {

void ModelDrawer::draw(World::Model *model, const Geo::Point &position, const Geo::Orientation &orientation, Format::VHV::Header *vhv)
{
	glPushMatrix();
	Geo::Transformation transformation = Geo::Transformation::translateRotate(position, orientation);
	glMultMatrixf(transformation.matrix().transpose().elements());

	glActiveTexture(GL_TEXTURE0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);

	float distance2 = (position - mCameraPosition).magnitude2();

	for(int bp=0; bp<model->numBodyParts(); bp++) {
		const World::Model::BodyPart &bodyPart = model->bodyPart(bp);
		for(int m=0; m<bodyPart.numModels; m++) {
			const World::Model::_Model &_model = bodyPart.models[m];
			int lodNum = 0;
			for(int lod=0; lod<_model.numLods; lod++) {
				float switchPoint2 = _model.lods[lod].switchPoint * _model.lods[lod].switchPoint;
				if(distance2 > switchPoint2) {
					lodNum = lod;
				} else {
					break;
				}
			}

			const World::Model::Lod &lod = _model.lods[lodNum];
			for(int me=0; me<lod.numMeshes; me++) {
				const World::Model::Mesh &mesh = lod.meshes[me];

				int materialIndex = model->skin(0, mesh.material);
				World::Material *material = model->material(materialIndex);
				if(material && material->texture()) {
					if(mDrawTextures) {
						material->texture()->select();
					} else {
						glBindTexture(GL_TEXTURE_2D, 0);
					}

					if(material->translucent()) {
						glEnable(GL_BLEND);
					} else {
						glDisable(GL_BLEND);
					}

					if(material->alphaTest()) {
						glEnable(GL_ALPHA_TEST);
					} else {
						glDisable(GL_ALPHA_TEST);
					}

					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				}

				for(int sg=0; sg<mesh.numStripGroups; sg++) {
					const World::Model::StripGroup &stripGroup = mesh.stripGroups[sg];
					for(int s=0; s<stripGroup.numStrips; s++) {
						const World::Model::Strip &strip = stripGroup.strips[s];

						glEnableClientState(GL_VERTEX_ARRAY);
						glVertexPointer(3, GL_FLOAT, sizeof(Format::VVD::Vertex), &model->vertices()->position);

						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glTexCoordPointer(2, GL_FLOAT, sizeof(Format::VVD::Vertex), &model->vertices()->texCoord);

						if(vhv) {
							glEnableClientState(GL_COLOR_ARRAY);
							glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Format::VHV::RGBA), vhv->vertices(lodNum));
						} else {
							glDisableClientState(GL_COLOR_ARRAY);
							glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
						}

						GLenum mode = GL_TRIANGLES;
						glDrawElements(mode, strip.numIndices, GL_UNSIGNED_SHORT, strip.indices);
					}
				}
			}
		}
	}

	glPopMatrix();
}

}