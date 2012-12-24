#include "Renderer.hpp"

#include "File/VMT.hpp"
#include "File/VTF.hpp"

#include "Geo/Transformation.hpp"

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

Renderer::Renderer(Map *map, int width, int height)
{
	mMap = map;

	glMatrixMode(GL_PROJECTION_MATRIX);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glLoadIdentity();
	float scale = 10;
	gluPerspective(70, (float)width / (float)height, 50, 10000);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glDepthFunc(GL_LEQUAL);
	for(unsigned int i=0; i<mMap->bsp()->numEntities(); i++) {
		const File::BSP::Entity &entity = mMap->bsp()->entity(i);
		if(entity.section->hasParameter("classname") && entity.section->parameter("classname") == "info_player_start") {
			const std::string &position = entity.section->parameter("origin");
			std::vector<std::string> posParts = StringUtils::split(position, " ");
			float x = (float)atof(posParts[0].c_str());
			float y = (float)atof(posParts[1].c_str());
			float z = (float)atof(posParts[2].c_str());
			mPosition = Geo::Point(x, y, z + 60);

			const std::string &rotation = entity.section->parameter("angles");
			std::vector<std::string> rotParts = StringUtils::split(rotation, " ");
			float pitch = (float)atof(rotParts[0].c_str());
			float yaw = (float)atof(rotParts[1].c_str());
			float roll = (float)atof(rotParts[2].c_str());

			mPitch = pitch;
			mYaw = yaw;
		}
	}

	mStartFrustum = Geo::Frustum(70, (float)width / (float)height);

	mFrustumCull = false;
	mUpdateFrustum = true;
	mLight = true;
}

void Renderer::rotate(int yaw, int pitch)
{
	mYaw += yaw;
	if(mYaw > 360) {
		mYaw -= 360;
	}

	if(mYaw < 0) {
		mYaw += 360;
	}

	mPitch += pitch;
	if(mPitch > 70) {
		mPitch = 70;
	}

	if(mPitch < -70) {
		mPitch = -70;
	}
}

void Renderer::move(int amount)
{
	float speed = 30;

	Geo::Vector delta(1, 0, 0);
	delta = Geo::Transformation::rotateZ(mYaw) * delta;
	mPosition = mPosition + speed * amount * delta;
};

void Renderer::strafe(int amount)
{
	float speed = 30;

	Geo::Vector delta(0, -1, 0);
	delta = Geo::Transformation::rotateZ(mYaw) * delta;
	mPosition = mPosition + speed * amount * delta;
};

void Renderer::rise(int amount)
{
	float speed = 30;

	mPosition = mPosition + speed * amount * Geo::Vector(0, 0, 1);
};

const Map::Leaf *findCameraLeaf(Map *map, const Geo::Point &position)
{
	Map::BSPBase *cursor = map->rootNode();
	while(cursor->type == Map::BSPBase::TypeNode) {
		int child;
		Map::Node *node = (Map::Node*)cursor;

		if(node->plane.pointInFront(position)) {
			child = 0;
		} else {
			child = 1;
		}

		cursor = node->children[child];
	}

	return (Map::Leaf*)cursor;
}

void Renderer::renderFace(const Map::Face &face)
{
	if(face.plane.normal() * (face.vertices[0] - mPosition)> 0) {
		mNumFacesCulled++;
		return;
	}

	glBlendFunc(GL_ONE, GL_ZERO);
	if(mTexture) {
		glBindTexture(GL_TEXTURE_2D, face.texture->tex);
		glColor3f(1.0f, 1.0f, 1.0f);
	} else {
		glBindTexture(GL_TEXTURE_2D, 0);
		glColor3f(face.gray, face.gray, face.gray);
	}

	glBegin(GL_POLYGON);
	for(int j=0; j<face.numVertices; j++) {
		Geo::Point &vertex = face.vertices[j];
		float s = face.textureVertices[0][0] * vertex.x() + face.textureVertices[0][1] * vertex.y() + face.textureVertices[0][2] * vertex.z() + face.textureVertices[0][3];
		float t = face.textureVertices[1][0] * vertex.x() + face.textureVertices[1][1] * vertex.y() + face.textureVertices[1][2] * vertex.z() + face.textureVertices[1][3];

		if(face.texture->vtf) {
			glTexCoord2f(s / face.texture->vtf->width(), t / face.texture->vtf->height());
		}

		glVertex3f(-vertex.y(), vertex.z(), -vertex.x());
	}
	glEnd();

	if(mLight) {
		glBindTexture(GL_TEXTURE_2D, face.lightMap);
		glColor3f(1.0f, 1.0f, 1.0f);

		glBlendFunc(GL_ZERO, GL_SRC_COLOR);
		glBegin(GL_POLYGON);
		for(int j=0; j<face.numVertices; j++) {
			Geo::Point &vertex = face.vertices[j];

			float s = face.lightMapVertices[0][0] * vertex.x() + face.lightMapVertices[0][1] * vertex.y() + face.lightMapVertices[0][2] * vertex.z() + face.lightMapVertices[0][3];
			float t = face.lightMapVertices[1][0] * vertex.x() + face.lightMapVertices[1][1] * vertex.y() + face.lightMapVertices[1][2] * vertex.z() + face.lightMapVertices[1][3];

			s -= face.lightMapMinU;
			t -= face.lightMapMinV;
			glTexCoord2f(s / face.lightMapWidth, t / face.lightMapHeight);

			glVertex3f(-vertex.y(), vertex.z(), -vertex.x());
		}
		glEnd();
	}

	mNumPolysDrawn++;
}

void Renderer::renderLeaf(const Map::Leaf *leaf, const Map::Leaf *cameraLeaf)
{
	if(leaf->number == -1) {
		return;
	}

	if(cameraLeaf->visibleLeaves != 0 && !cameraLeaf->visibleLeaves[leaf->number]) {
		return;
	}

	mNumVisLeaves++;

	if(mFrustumCull && mFrustum.boxOutside(leaf->bbox)) {
		mNumFrustumCulled++;
		return;
	}

	for(int j=0; j<leaf->numFaces; j++) {
		renderFace(*leaf->faces[j]);
	}
}

void Renderer::renderNode(const Map::Node *node, const Map::Leaf *cameraLeaf)
{
	if(mFrustumCull && mFrustum.boxOutside(node->bbox)) {
		mNumFrustumCulled++;
		return;
	}

	for(int i=0; i<2; i++) {
		int start = node->plane.pointInFront(mPosition) ? 0 : 1;
		int j = (i + start) % 2;

		switch(node->children[j]->type) {
			case Map::BSPBase::TypeNode:
				renderNode((Map::Node*)node->children[j], cameraLeaf);
				break;

			case Map::BSPBase::TypeLeaf:
				renderLeaf((Map::Leaf*)node->children[j], cameraLeaf);
				break;
		}
	}
}

void Renderer::renderModel(const Map::Model &model, const Geo::Point &position)
{
	if(model.mdl == 0 || model.vtx == 0 || model.vvd == 0) {
		return;
	}

	glPushMatrix();
	glTranslatef(-position.y(), position.z(), -position.x());
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);

	for(int b=0; b<model.vtx->numBodyParts(); b++) {
		const File::VTX::BodyPart &bodyPart = model.vtx->bodyPart(b);
		for(int m=0; m<bodyPart.numModels; m++) {
			File::VTX::Lod &lod = bodyPart.models[m].lods[0];
			for(int me=0; me<lod.numMeshes; me++) {
				File::VTX::Mesh &mesh = lod.meshes[me];
				glBindTexture(GL_TEXTURE_2D, model.textures[model.mdl->skin(0, me)].tex);
				for(int sg=0; sg<mesh.numStripGroups; sg++) {
					File::VTX::StripGroup &stripGroup = mesh.stripGroups[sg];
					for(int s=0; s<stripGroup.numStrips; s++) {
						File::VTX::Strip &strip = stripGroup.strips[s];
						if(strip.flags == 0x1) {
							glBegin(GL_TRIANGLE_STRIP);
						} else {
							glBegin(GL_TRIANGLES);
						}

						for(int v=0; v<strip.numVertices; v++) {
							File::VVD::Vertex &vertex = model.vvd->lod(0).vertices[strip.vertices[v]];
							glVertex3f(-vertex.position.y, vertex.position.z, -vertex.position.x);
							glTexCoord2f(vertex.texCoord.u, vertex.texCoord.v);
						}
						glEnd();
					}
				}
			}
		}
	}
	glPopMatrix();
}

void Renderer::render()
{
	mNumVisLeaves = 0;
	mNumFrustumCulled = 0;
	mNumPolysDrawn = 0;
	mNumFacesCulled = 0;

	if(mUpdateFrustum) {
		Geo::Transformation transformation = Geo::Transformation::translate(mPosition);
		transformation = transformation * Geo::Transformation::rotateZ(mYaw);
		transformation = transformation * Geo::Transformation::rotateY(-mPitch);

		mFrustum = mStartFrustum * transformation;
	}

	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPushMatrix();
	glRotatef(-mPitch, 1, 0, 0);
	glRotatef(-mYaw, 0, 1, 0);
	glTranslatef(mPosition.y(), -mPosition.z(), mPosition.x());

	const Map::Leaf *cameraLeaf = findCameraLeaf(mMap, mPosition);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderNode(mMap->rootNode(), cameraLeaf);

	for(unsigned int i=0; i<mMap->numEntities(); i++) {
		const Map::Entity &entity = mMap->entity(i);
		if(entity.model) {
			renderModel(*entity.model, entity.position);
		}
	}

	glPopMatrix();
}
