#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "File/IReaderFactory.hpp"
#include "File/BSP.hpp"
#include "Geo/Vector.hpp"
#include "Geo/Frustum.hpp"

#include "Map.hpp"

class Renderer {
public:
	Renderer(Map *map, int width, int height);

	void render();

	void rotate(int yaw, int pitch);
	void move(int amount);
	void rise(int amount);

	void frustumCull(bool cull) { mFrustumCull = cull; }
	void updateFrustum(bool update) { mUpdateFrustum = update; }

	int numFrustumCulled() { return mNumFrustumCulled; }
	int numPolysDrawn() { return mNumPolysDrawn; }
	int numVisLeaves() { return mNumVisLeaves; }
	int numFacesCulled() { return mNumFacesCulled; }

private:

	void renderFace(const Map::Face &face);
	void renderLeaf(const Map::Leaf *leaf, const Map::Leaf *cameraLeaf);
	void renderNode(const Map::Node *node, const Map::Leaf *cameraLeaf);

	Map *mMap;

	Geo::Vector mPosition;
	float mYaw;
	float mPitch;
	Geo::Frustum mStartFrustum;
	Geo::Frustum mFrustum;
	bool mFrustumCull;
	bool mUpdateFrustum;
	int mNumPolysDrawn;
	int mNumFrustumCulled;
	int mNumVisLeaves;
	int mNumFacesCulled;
	Geo::Vector mViewVector;
};

#endif