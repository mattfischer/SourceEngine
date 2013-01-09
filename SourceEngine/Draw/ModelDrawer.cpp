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

	for(int b=0; b<model->vtx()->numBodyParts(); b++) {
		const Format::VTX::BodyPart &bodyPart = model->vtx()->bodyPart(b);
		for(int m=0; m<bodyPart.numModels; m++) {
			Format::VTX::Lod &lod = bodyPart.models[m].lods[0];
			for(int me=0; me<lod.numMeshes; me++) {
				Format::VTX::Mesh &mesh = lod.meshes[me];
				World::Material *material = model->material(model->mdl()->skin(0, 0));
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
					Format::VTX::StripGroup &stripGroup = mesh.stripGroups[sg];
					for(int s=0; s<stripGroup.numStrips; s++) {
						Format::VTX::Strip &strip = stripGroup.strips[s];

						glEnableClientState(GL_VERTEX_ARRAY);
						glVertexPointer(3, GL_FLOAT, sizeof(Format::VVD::Vertex), &model->vertices(0)->position);

						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glTexCoordPointer(2, GL_FLOAT, sizeof(Format::VVD::Vertex), &model->vertices(0)->texCoord);

						if(vhv) {
							glEnableClientState(GL_COLOR_ARRAY);
							glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Format::VHV::RGBA), vhv->vertices(0));
						} else {
							glDisableClientState(GL_COLOR_ARRAY);
							glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
						}

						GLenum mode;
						if(strip.flags == 0x1) {
							mode = GL_TRIANGLES;
						} else {
							mode = GL_TRIANGLE_STRIP;
						}
						glDrawElements(mode, strip.numVertices, GL_UNSIGNED_SHORT, strip.vertices);
					}
				}
			}
		}
	}
	glPopMatrix();
}

}