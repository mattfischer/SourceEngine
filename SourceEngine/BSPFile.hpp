#ifndef BSP_FILE_HPP
#define BSP_FILE_HPP

#include "SharedPointer.hpp"
#include "IReader.hpp"

#include <string>
#include <vector>

class BSPFile {
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

    BSPFile(sp<IReader> reader);

	std::vector<Model> &models() { return mModels; }
	std::vector<Face> &faces() { return mFaces; }
	std::vector<TexInfo> &texInfos() { return mTexInfos; }
	std::vector<TexData> &texDatas() { return mTexDatas; }
	std::vector<Edge> &edges() { return mEdges; }
	std::vector<Vector> &vertices() { return mVertices; }
	std::vector<int> &surfEdges() { return mSurfEdges; }
	std::vector<std::string> &texDataStringTable() { return mTexDataStringTable; }

private:
	sp<IReader> mReader;

	std::vector<Model> mModels;
	std::vector<Face> mFaces;
	std::vector<TexInfo> mTexInfos;
	std::vector<TexData> mTexDatas;
	std::vector<Edge> mEdges;
	std::vector<Vector> mVertices;
	std::vector<int> mSurfEdges;
	std::vector<std::string> mTexDataStringTable;
};

#endif