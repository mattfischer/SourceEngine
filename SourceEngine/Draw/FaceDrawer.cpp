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

	glBlendFunc(GL_ONE, GL_ZERO);
	if(mDrawTextures && face->material() && face->material()->texture()) {
		glActiveTexture(GL_TEXTURE0);
		face->material()->texture()->select();

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, face->lightmapTex());
	} else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glColor3f(face->gray(), face->gray(), face->gray());
	}

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