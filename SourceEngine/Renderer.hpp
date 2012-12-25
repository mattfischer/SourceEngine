#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "File/IReaderFactory.hpp"
#include "File/BSP.hpp"
#include "Geo/Point.hpp"
#include "Geo/Frustum.hpp"

#include "Map.hpp"

class Renderer {
public:
	Renderer(Map *map, int width, int height);

	void render();

	void rotate(int yaw, int pitch);
	void move(int amount);
	void strafe(int amount);
	void rise(int amount);

	void frustumCull(bool cull) { mFrustumCull = cull; }
	void updateFrustum(bool update) { mUpdateFrustum = update; }
	void texture(bool texture) { mTexture = texture; }
	void light(bool light) { mLight = light; }

	int numFrustumCulled() { return mNumFrustumCulled; }
	int numPolysDrawn() { return mNumPolysDrawn; }
	int numVisLeaves() { return mNumVisLeaves; }
	int numFacesCulled() { return mNumFacesCulled; }

private:

	void renderFace(const Map::Face &face);
	void renderLeaf(const Map::Leaf *leaf, const Map::Leaf *cameraLeaf);
	void renderNode(const Map::Node *node, const Map::Leaf *cameraLeaf);
	void renderModel(const Map::Model &model, const Geo::Point &position, float pitch, float yaw, float roll);

	Map *mMap;

	Geo::Point mPosition;
	float mYaw;
	float mPitch;
	Geo::Frustum mStartFrustum;
	Geo::Frustum mFrustum;
	bool mFrustumCull;
	bool mUpdateFrustum;
	bool mTexture;
	bool mLight;
	int mNumPolysDrawn;
	int mNumFrustumCulled;
	int mNumVisLeaves;
	int mNumFacesCulled;
	Geo::Vector mViewVector;
};

#endif