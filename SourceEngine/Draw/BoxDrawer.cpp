#include "Draw/BoxDrawer.hpp"

#include <GL/glew.h>

namespace Draw {

void BoxDrawer::draw(const Geo::BoxOriented &box)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_DEPTH_TEST);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

	glBegin(GL_LINE_LOOP);
	glVertex3f(box.point().x(), box.point().y(), box.point().z());
	glVertex3f(box.point().x() + box.vector(0).x(), box.point().y(), box.point().z());
	glVertex3f(box.point().x() + box.vector(0).x(), box.point().y() + box.vector(1).y(), box.point().z());
	glVertex3f(box.point().x(), box.point().y() + box.vector(1).y(), box.point().z());
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(box.point().x(), box.point().y(), box.point().z() + box.vector(2).z());
	glVertex3f(box.point().x() + box.vector(0).x(), box.point().y(), box.point().z() + box.vector(2).z());
	glVertex3f(box.point().x() + box.vector(0).x(), box.point().y() + box.vector(1).y(), box.point().z() + box.vector(2).z());
	glVertex3f(box.point().x(), box.point().y() + box.vector(1).y(), box.point().z() + box.vector(2).z());
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(box.point().x(), box.point().y(), box.point().z());
	glVertex3f(box.point().x(), box.point().y(), box.point().z() + box.vector(2).z());

	glVertex3f(box.point().x() + box.vector(0).x(), box.point().y(), box.point().z());
	glVertex3f(box.point().x() + box.vector(0).x(), box.point().y(), box.point().z() + box.vector(2).z());

	glVertex3f(box.point().x() + box.vector(0).x(), box.point().y() + box.vector(1).y(), box.point().z());
	glVertex3f(box.point().x() + box.vector(0).x(), box.point().y() + box.vector(1).y(), box.point().z() + box.vector(2).z());

	glVertex3f(box.point().x(), box.point().y() + box.vector(1).y(), box.point().z());
	glVertex3f(box.point().x(), box.point().y() + box.vector(1).y(), box.point().z() + box.vector(2).z());
	glEnd();

	glEnable(GL_DEPTH_TEST);
}

void BoxDrawer::draw(const Geo::BoxAligned &box)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_DEPTH_TEST);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

	glBegin(GL_LINE_LOOP);
	glVertex3f(box.minPoint().x(), box.minPoint().y(), box.minPoint().z());
	glVertex3f(box.maxPoint().x(), box.minPoint().y(), box.minPoint().z());
	glVertex3f(box.maxPoint().x(), box.maxPoint().y(), box.minPoint().z());
	glVertex3f(box.minPoint().x(), box.maxPoint().y(), box.minPoint().z());
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(box.minPoint().x(), box.minPoint().y(), box.maxPoint().z());
	glVertex3f(box.maxPoint().x(), box.minPoint().y(), box.maxPoint().z());
	glVertex3f(box.maxPoint().x(), box.maxPoint().y(), box.maxPoint().z());
	glVertex3f(box.minPoint().x(), box.maxPoint().y(), box.maxPoint().z());
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(box.minPoint().x(), box.minPoint().y(), box.minPoint().z());
	glVertex3f(box.minPoint().x(), box.minPoint().y(), box.maxPoint().z());

	glVertex3f(box.maxPoint().x(), box.minPoint().y(), box.minPoint().z());
	glVertex3f(box.maxPoint().x(), box.minPoint().y(), box.maxPoint().z());

	glVertex3f(box.maxPoint().x(), box.maxPoint().y(), box.minPoint().z());
	glVertex3f(box.maxPoint().x(), box.maxPoint().y(), box.maxPoint().z());

	glVertex3f(box.minPoint().x(), box.maxPoint().y(), box.minPoint().z());
	glVertex3f(box.minPoint().x(), box.maxPoint().y(), box.maxPoint().z());
	glEnd();

	glEnable(GL_DEPTH_TEST);
}

}