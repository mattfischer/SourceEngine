#include "Draw/ModelDrawer.hpp"

#include <GL/glew.h>

namespace Draw {

void ModelDrawer::draw(World::Model *model, const Geo::Point &position, const Geo::Orientation &orientation, Format::VHV::Header *vhv)
{
	glPushMatrix();
	glTranslatef(position.x(), position.y(), position.z());
	glRotatef(orientation.yaw(), 0, 0, 1);
	glRotatef(orientation.pitch(), 0, 1, 0);
	glRotatef(orientation.roll(), 1, 0, 0);

	glActiveTexture(GL_TEXTURE0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);

	int lodNum = 0;
	for(int bp=0; bp<model->numBodyParts(); bp++) {
		const World::Model::BodyPart &bodyPart = model->bodyPart(bp);
		for(int m=0; m<bodyPart.numModels; m++) {
			const World::Model::_Model &_model = bodyPart.models[m];
			const World::Model::Lod &lod = _model.lods[lodNum];
			for(int me=0; me<lod.numMeshes; me++) {
				const World::Model::Mesh &mesh = lod.meshes[me];
				for(int sg=0; sg<mesh.numStripGroups; sg++) {
					const World::Model::StripGroup &stripGroup = mesh.stripGroups[sg];
					for(int s=0; s<stripGroup.numStrips; s++) {
						const World::Model::Strip &strip = stripGroup.strips[s];

						World::Material *material = model->material(0);
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

						glEnableClientState(GL_VERTEX_ARRAY);
						glVertexPointer(3, GL_FLOAT, sizeof(Format::VVD::Vertex), &model->vertices(lodNum)->position);

						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glTexCoordPointer(2, GL_FLOAT, sizeof(Format::VVD::Vertex), &model->vertices(lodNum)->texCoord);

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