#ifndef FILE_BSP_HPP
#define FILE_BSP_HPP

#include "File/IReaderFactory.hpp"

#include <string>

namespace File {

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

	BSP(IReaderFactory *factory, const std::string &name);

	int numModels() { return mNumModels; }
	const Model &model(int model) { return mModels[model]; }

	int numFaces() { return mNumFaces; }
	const Face &face(int face) { return mFaces[face]; }

	int numTexInfos() { return mNumTexInfos; }
	const TexInfo &texInfo(int texInfo) { return mTexInfos[texInfo]; }

	int numTexDatas() { return mNumTexDatas; }
	const TexData &texData(int texData) { return mTexDatas[texData]; }

	int numEdges() { return mNumEdges; }
	const Edge &edge(int edge) { return mEdges[edge]; }

	int numVertices() { return mNumVertices; }
	const Vector &vertex(int vertex) { return mVertices[vertex]; }

	int numSurfEdges() { return mNumSurfEdges; }
	const int surfEdge(int surfEdge) { return mSurfEdges[surfEdge]; }

	int numTexDataStrings() { return mNumTexDataStrings; }
	const std::string &texDataString(int texDataString) { return mTexDataStringTable[texDataString]; }

private:
	int mNumModels;
	Model *mModels;

	int mNumFaces;
	Face *mFaces;

	int mNumTexInfos;
	TexInfo *mTexInfos;

	int mNumTexDatas;
	TexData *mTexDatas;

	int mNumEdges;
	Edge *mEdges;

	int mNumVertices;
	Vector *mVertices;

	int mNumSurfEdges;
	int *mSurfEdges;

	int mNumTexDataStrings;
	std::string *mTexDataStringTable;
};

}

#endif