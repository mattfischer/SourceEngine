#ifndef FORMAT_BSP_HPP
#define FORMAT_BSP_HPP

#include "File/Space.hpp"

#include "Format/KeyValue.hpp"

#include <string>

namespace Format {

class BSP {
public:
	struct Vector {
		float x;
		float y;
		float z;
	};

	struct Plane {
		Vector normal;
		float dist;
		int type;
	};

	struct Node {
		int	planeNum;
		int	children[2];
		short mins[3];
		short maxs[3];
		unsigned short firstFace;
		unsigned short numFaces;
		short area;
		short padding;
	};

	struct Leaf {
		int	contents;
		short cluster;
		short area:9;
		short flags:7;
		short mins[3];
		short maxs[3];
		unsigned short firstLeafFace;
		unsigned short numLeafFaces;
		unsigned short firstLeafBrush;
		unsigned short numLeafBrushes;
		short leafWaterDataID;
	};

	struct Face {
		unsigned short	planeNum;
		unsigned char	side;
		unsigned char	onNode;
		int				firstEdge;
		short			numEdges;
		short			texInfo;
		short			dispInfo;
		short			surfaceFogVolumeID;
		unsigned char	styles[4];
		int				lightOfs;
		float			area;
		int				lightmapTextureMinsInLuxels[2];
		int				lightmapTextureSizeInLuxels[2];
		int				origFace;
		unsigned short	numPrims;
		unsigned short	firstPrimID;
		unsigned int	smoothingGroups;
	};

	struct Edge {
		short int v[2];
	};

	struct Model {
		Vector mins;
		Vector maxs;
		Vector origin;
		int headNode;
		int firstFace;
		int numFaces;
	};

	struct TexInfo {
		float textureVecs[2][4];
		float lightmapVecs[2][4];
		int	flags;
		int	texdata;
	};

	struct TexData {
		Vector reflectivity;
		int	nameStringTableID;
		int	width;
		int height;
		int	view_width;
		int view_height;
	};

	struct Entity {
		const KeyValue::Section *section;
	};

	struct StaticProp
	{
		Vector origin;
		Vector angles;
		unsigned short propType;
		unsigned short firstLeaf;
		unsigned short leafCount;
		unsigned char solid;
		unsigned char flags;
		int	skin;
		float fadeMinDist;
		float fadeMaxDist;
		Vector lightingOrigin;

		float forcedFadeScale;

		unsigned short minDXLevel;
		unsigned short maxDXLevel;

		unsigned char minCPULevel;
		unsigned char maxCPULevel;
		unsigned char minGPULevel;
		unsigned char maxGPULevel;

		unsigned int diffuseModulation;
	};

	BSP(File::File *file);

	size_t numModels() { return mNumModels; }
	const Model &model(int model) { return mModels[model]; }

	size_t numFaces() { return mNumFaces; }
	const Face &face(int face) { return mFaces[face]; }

	size_t numTexInfos() { return mNumTexInfos; }
	const TexInfo &texInfo(int texInfo) { return mTexInfos[texInfo]; }

	size_t numTexDatas() { return mNumTexDatas; }
	const TexData &texData(int texData) { return mTexDatas[texData]; }

	size_t numEdges() { return mNumEdges; }
	const Edge &edge(int edge) { return mEdges[edge]; }

	size_t numVertices() { return mNumVertices; }
	const Vector &vertex(int vertex) { return mVertices[vertex]; }

	size_t numSurfEdges() { return mNumSurfEdges; }
	const int surfEdge(int surfEdge) { return mSurfEdges[surfEdge]; }

	size_t numTexDataStrings() { return mNumTexDataStrings; }
	const std::string &texDataString(int texDataString) { return mTexDataStringTable[texDataString]; }

	size_t numNodes() { return mNumNodes; }
	const Node &node(int node) { return mNodes[node]; }

	size_t numLeaves() { return mNumLeaves; }
	const Leaf &leaf(int leaf) { return mLeaves[leaf]; }

	size_t numPlanes() { return mNumPlanes; }
	const Plane &plane(int plane) { return mPlanes[plane]; }

	size_t numLeafFaces() { return mNumLeafFaces; }
	const unsigned short leafFace(int leafFace) { return mLeafFaces[leafFace]; }

	size_t numClusters() { return mNumClusters; }
	bool clusterVisibleFrom(int cameraCluster, int cluster) { return mVisData[cameraCluster][cluster]; }

	size_t numEntities() { return mNumEntities; }
	const Entity &entity(int entity) { return mEntities[entity]; }

	const unsigned char *lighting(int offset) { return mLighting + offset; }

	size_t numStaticPropNames() { return mNumStaticPropNames; }
	const std::string &staticPropName(int staticPropName) { return mStaticPropNames[staticPropName]; }

	size_t numStaticPropLeaves() { return mNumStaticPropLeaves; }
	unsigned short staticPropLeaf(int staticPropLeaf) { return mStaticPropLeaves[staticPropLeaf]; }

	size_t numStaticProps() { return mNumStaticProps; }
	const StaticProp &staticProp(int staticProp) { return mStaticProps[staticProp]; }

	static BSP *open(File::Space *space, const std::string &filename);

private:
	size_t mNumModels;
	Model *mModels;

	size_t mNumFaces;
	Face *mFaces;

	size_t mNumTexInfos;
	TexInfo *mTexInfos;

	size_t mNumTexDatas;
	TexData *mTexDatas;

	size_t mNumEdges;
	Edge *mEdges;

	size_t mNumVertices;
	Vector *mVertices;

	size_t mNumSurfEdges;
	int *mSurfEdges;

	size_t mNumTexDataStrings;
	std::string *mTexDataStringTable;

	size_t mNumNodes;
	Node *mNodes;

	size_t mNumLeaves;
	Leaf *mLeaves;

	size_t mNumPlanes;
	Plane *mPlanes;

	size_t mNumLeafFaces;
	unsigned short *mLeafFaces;

	bool **mVisData;
	size_t mNumClusters;

	KeyValue *mEntityKeyValue;
	Entity *mEntities;
	size_t mNumEntities;

	unsigned char *mLighting;

	size_t mNumStaticPropNames;
	std::string *mStaticPropNames;

	size_t mNumStaticPropLeaves;
	unsigned short *mStaticPropLeaves;

	size_t mNumStaticProps;
	StaticProp *mStaticProps;

	void parseVisData(unsigned char *visData, int visDataLength);
	void parseStaticProps(File::File *file, int offset);
};

}

#endif