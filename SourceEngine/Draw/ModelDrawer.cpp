#include "Draw/ModelDrawer.hpp"

namespace Draw {

void ModelDrawer::draw(World::Model *model, const Geo::Point &position, const Geo::Orientation &orientation)
{
	glPushMatrix();
	glTranslatef(position.x(), position.y(), position.z());
	glRotatef(orientation.yaw(), 0, 0, 1);
	glRotatef(orientation.pitch(), 0, -1, 0);
	glRotatef(orientation.roll(), -1, 0, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glBlendFunc(GL_ONE, GL_ZERO);

	for(int b=0; b<model->vtx()->numBodyParts(); b++) {
		const File::VTX::BodyPart &bodyPart = model->vtx()->bodyPart(b);
		for(int m=0; m<bodyPart.numModels; m++) {
			File::VTX::Lod &lod = bodyPart.models[m].lods[bodyPart.models[m].numLods - 1];
			for(int me=0; me<lod.numMeshes; me++) {
				File::VTX::Mesh &mesh = lod.meshes[me];
				World::Material *material = model->material(model->mdl()->skin(0, me));
				if(material && material->texture()) {
					material->texture()->select();
				}

				for(int sg=0; sg<mesh.numStripGroups; sg++) {
					File::VTX::StripGroup &stripGroup = mesh.stripGroups[sg];
					for(int s=0; s<stripGroup.numStrips; s++) {
						File::VTX::Strip &strip = stripGroup.strips[s];
						if(strip.flags == 0x1) {
							glBegin(GL_TRIANGLES);
						} else {
							glBegin(GL_TRIANGLE_STRIP);
						}

						for(int v=0; v<strip.numVertices; v++) {
							File::VVD::Vertex &vertex = model->vvd()->lod(bodyPart.models[m].numLods - 1).vertices[strip.vertices[v]];
							glTexCoord2f(vertex.texCoord.u, vertex.texCoord.v);
							glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
						}
						glEnd();
					}
				}
			}
		}
	}
	glPopMatrix();
}

}