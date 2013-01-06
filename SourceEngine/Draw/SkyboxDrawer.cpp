#include "Draw/SkyboxDrawer.hpp"

namespace Draw {

void SkyboxDrawer::draw(World::Skybox *skybox)
{
	glDisable(GL_DEPTH_TEST);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	skybox->material(World::Skybox::SurfaceFront)->texture()->select();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100.0f, 100.0f, 100.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(100.0f, 100.0f, 100.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(100.0f, 100.0f, -100.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-100.0f, 100.0f, -100.0f);
	glEnd();

	skybox->material(World::Skybox::SurfaceBack)->texture()->select();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100.0f, -100.0f, 100.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(100.0f, -100.0f, 100.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(100.0f, -100.0f, -100.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-100.0f, -100.0f, -100.0f);
	glEnd();

	skybox->material(World::Skybox::SurfaceUp)->texture()->select();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100.0f, -100.0f, 100.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(100.0f, -100.0f, 100.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(100.0f, 100.0f, 100.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-100.0f, 100.0f, 100.0f);
	glEnd();

	skybox->material(World::Skybox::SurfaceDown)->texture()->select();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100.0f, -100.0f, -100.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(100.0f, -100.0f, -100.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(100.0f, 100.0f, -100.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-100.0f, 100.0f, -100.0f);
	glEnd();

	skybox->material(World::Skybox::SurfaceLeft)->texture()->select();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100.0f, -100.0f, 100.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-100.0f, 100.0f, 100.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-100.0f, 100.0f, -100.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-100.0f, -100.0f, -100.0f);
	glEnd();

	skybox->material(World::Skybox::SurfaceRight)->texture()->select();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(100.0f, -100.0f, 100.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(100.0f, 100.0f, 100.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(100.0f, 100.0f, -100.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(100.0f, -100.0f, -100.0f);
	glEnd();

	glEnable(GL_DEPTH_TEST);
}

}