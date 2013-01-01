#include "Draw/ModelDrawer.hpp"

#include <GL/glew.h>

namespace Draw {

void ModelDrawer::draw(World::Model *model, const Geo::Point &position, const Geo::Orientation &orientation)
{
	glPushMatrix();
	glTranslatef(position.x(), position.y(), position.z());
	glRotatef(orientation.yaw(), 0, 0, 1);
	glRotatef(orientation.pitch(), 0, 1, 0);
	glRotatef(orientation.roll(), 1, 0, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);

	for(int b=0; b<model->vtx()->numBodyParts(); b++) {
		const File::VTX::BodyPart &bodyPart = model->vtx()->bodyPart(b);
		for(int m=0; m<bodyPart.numModels; m++) {
			File::VTX::Lod &lod = bodyPart.models[m].lods[bodyPart.models[m].numLods - 1];
			for(int me=0; me<lod.numMeshes; me++) {
				File::VTX::Mesh &mesh = lod.meshes[me];
				World::Material *material = model->material(model->mdl()->skin(0, 0));
				if(material && material->texture()) {
					material->texture()->select();
				}

				for(int sg=0; sg<mesh.numStripGroups; sg++) {
					File::VTX::StripGroup &stripGroup = mesh.stripGroups[sg];
					for(int s=0; s<stripGroup.numStrips; s++) {
						File::VTX::Strip &strip = stripGroup.strips[s];

						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glEnableClientState(GL_VERTEX_ARRAY);
						glVertexPointer(3, GL_FLOAT, sizeof(File::VVD::Vertex), &model->vvd()->lod(bodyPart.models[m].numLods - 1).vertices[0].position);
						glTexCoordPointer(2, GL_FLOAT, sizeof(File::VVD::Vertex), &model->vvd()->lod(bodyPart.models[m].numLods - 1).vertices[0].texCoord);
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