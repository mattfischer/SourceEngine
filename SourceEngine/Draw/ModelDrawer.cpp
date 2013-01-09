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

	GLenum mode = GL_TRIANGLES;
	glDrawElements(mode, model->lod(0).numIndices, GL_UNSIGNED_SHORT, model->lod(0).indices);
	glPopMatrix();
}

}