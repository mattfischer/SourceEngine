#include "Draw/FaceDrawer.hpp"

#include <GL/glew.h>

namespace Draw {

void FaceDrawer::newFrame()
{
	mNumFacesDrawn = 0;
	mNumFacesCulled = 0;
}

void FaceDrawer::draw(World::Face *face)
{
	if(face->plane().normal() * (face->vertex(0) - mPosition)> 0) {
		mNumFacesCulled++;
		return;
	}

	if(mDrawTextures && face->material() && face->material()->texture()) {
		glActiveTexture(GL_TEXTURE0);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		face->material()->texture()->select();
	} else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glColor3f(face->gray(), face->gray(), face->gray());
	}

	if(mDrawLightmaps) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, face->lightmapTex());
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	} else {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}

	glBlendFunc(GL_ONE, GL_ZERO);
	glBegin(GL_POLYGON);
	for(int i=0; i<face->numVertices(); i++) {
		const Geo::Point &vertex = face->vertex(i);
		const Geo::Coordinate &coordinate = face->textureCoordinate(i);
		const Geo::Coordinate &lightmapCoordinate = face->lightmapCoordinate(i);

		glMultiTexCoord2f(GL_TEXTURE0, coordinate.u(), coordinate.v());
		glMultiTexCoord2f(GL_TEXTURE1, lightmapCoordinate.u(), lightmapCoordinate.v());
		glVertex3f(vertex.x(), vertex.y(), vertex.z());
	}
	glEnd();

	mNumFacesDrawn++;
}

}