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
		face->material()->texture()->select();
	} else {
		glBindTexture(GL_TEXTURE_2D, 0);
		glColor3f(face->gray(), face->gray(), face->gray());
	}

	glBegin(GL_POLYGON);
	for(int i=0; i<face->numVertices(); i++) {
		const Geo::Point &vertex = face->vertex(i);
		const Geo::Coordinate &coordinate = face->textureCoordinate(i);

		glTexCoord2f(coordinate.u(), coordinate.v());
		glVertex3f(vertex.x(), vertex.y(), vertex.z());
	}
	glEnd();

	if(mDrawLightmaps) {
		glBindTexture(GL_TEXTURE_2D, face->lightmapTex());
		glColor3f(1.0f, 1.0f, 1.0f);

		glBlendFunc(GL_ZERO, GL_SRC_COLOR);
		glBegin(GL_POLYGON);
		for(int j=0; j<face->numVertices(); j++) {
			const Geo::Point &vertex = face->vertex(j);
			const Geo::Coordinate &lightmapCoordinate = face->lightmapCoordinate(j);

			glTexCoord2f(lightmapCoordinate.u(), lightmapCoordinate.v());
			glVertex3f(vertex.x(), vertex.y(), vertex.z());
		}
		glEnd();
	}

	mNumFacesDrawn++;
}

}