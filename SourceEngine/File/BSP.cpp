#include "File/BSP.hpp"

namespace File {

struct Lump {
    int offset;
    int length;
    int version;
    char fourCC[4];
};

enum {
	LUMP_ENTITIES,
	LUMP_PLANES,
	LUMP_TEXDATA,
	LUMP_VERTICES,
	LUMP_VISIBILITY,
	LUMP_NODES,
	LUMP_TEXINFO,
	LUMP_FACES,
	LUMP_LIGHTING,
	LUMP_OCCLUSION,
	LUMP_LEAFS,
	LUMP_FACEIDS,
	LUMP_EDGES,
	LUMP_SURFEDGES,
	LUMP_MODELS,
	LUMP_WORLDLIGHTS,
	LUMP_LEAFFACES,
	LUMP_LEAFBRUSHES,
	LUMP_BRUSHES,
	LUMP_BRUSHSIDES,
	LUMP_AREAS,
	LUMP_AREAPORTALS,
	LUMP_PORTALS,
	LUMP_UNUSED0 = LUMP_PORTALS,
	LUMP_PROPCOLLISION = LUMP_PORTALS,
	LUMP_CLUSTERS,
	LUMP_UNUSED1 = LUMP_CLUSTERS,
	LUMP_PROPHULLS = LUMP_CLUSTERS,
	LUMP_PORTALVERTS,
	LUMP_UNUSED2 = LUMP_PORTALVERTS,
	LUMP_PROPHULLVERTS = LUMP_PORTALVERTS,
	LUMP_CLUSTERPORTALS,
	LUMP_UNUSED3 = LUMP_CLUSTERPORTALS,
	LUMP_PROPTRIS = LUMP_CLUSTERPORTALS,
	LUMP_DISPINFO,
	LUMP_ORIGINALFACES,
	LUMP_PHYSDISP,
	LUMP_PHYSCOLLIDE,
	LUMP_VERTNORMALS,
	LUMP_VERTNORMALINDICES,
	LUMP_DISP_LIGHTMAP_ALPHAS,
	LUMP_DISP_VERTS,
	LUMP_DISP_LIGHTMAP_SAMPLE_POSITIONS,
	LUMP_GAME_LUMP,
	LUMP_LEAFWATERDATA,
	LUMP_PRIMITIVES,
	LUMP_PRIMVERTS,
	LUMP_PRIMINDICES,
	LUMP_PAKFILE,
	LUMP_CLIPPORTALVERTS,
	LUMP_CUBEMAPS,
	LUMP_TEXDATA_STRING_DATA,
	LUMP_TEXDATA_STRING_TABLE,
	LUMP_OVERLAYS,
	LUMP_LEAFMINDISTTOWATER,
	LUMP_FACE_MACRO_TEXTURE_INFO,
	LUMP_DISP_TRIS,
	LUMP_PHYSCOLLIDESURFACE,
	LUMP_PROP_BLOB = LUMP_PHYSCOLLIDESURFACE,
	LUMP_WATEROVERLAYS,
	LUMP_LIGHTMAPPAGES,
	LUMP_LEAF_AMBIENT_INDEX_HDR = LUMP_LIGHTMAPPAGES,
	LUMP_LIGHTMAPPAGEINFOS,
	LUMP_LEAF_AMBIENT_INDEX = LUMP_LIGHTMAPPAGEINFOS,
	LUMP_LIGHTING_HDR,
	LUMP_WORLDLIGHTS_HDR,
	LUMP_LEAF_AMBIENT_LIGHTING_HDR,
	LUMP_LEAF_AMBIENT_LIGHTING,
	LUMP_XZIPPAKFILE,
	LUMP_FACES_HDR,
	LUMP_MAP_FLAGS,
	LUMP_OVERLAY_FADES,
	LUMP_OVERLAY_SYSTEM_LEVELS,
	LUMP_PHYSLEVEL,
	LUMP_DISP_MULTIBLEND,
	NUM_LUMPS
};

struct Header {
    int magic;
    int version;
    Lump lumps[NUM_LUMPS];
    int revision;
};

template <typename T> void readLump(IReader *reader, T *&list, int &num, const Header &header, int lumpNum)
{
	const Lump &lump = header.lumps[lumpNum];

	num = lump.length / sizeof(T);
	list = new T[num];
	reader->seek(lump.offset);
	reader->read((char*)list, lump.length);
}

BSP *BSP::open(IReaderFactory *factory, const std::string &name)
{
	BSP *ret = 0;
	std::string filename = "maps/" + name + ".bsp";
	IReader *reader = factory->open(filename);

	if(reader) {
		ret = new BSP(reader);
		delete reader;
	}

	return ret;
}

BSP::BSP(IReader *reader)
{
    Header header;
    reader->read((char*)&header, sizeof(header));

	readLump(reader, mVertices, mNumVertices, header, LUMP_VERTICES);
	readLump(reader, mPlanes, mNumPlanes, header, LUMP_PLANES);
	readLump(reader, mNodes, mNumNodes, header, LUMP_NODES);
	readLump(reader, mLeaves, mNumLeaves, header, LUMP_LEAFS);
	readLump(reader, mEdges, mNumEdges, header, LUMP_EDGES);
	readLump(reader, mSurfEdges, mNumSurfEdges, header, LUMP_SURFEDGES);
	readLump(reader, mFaces, mNumFaces, header, LUMP_FACES);
	readLump(reader, mModels, mNumModels, header, LUMP_MODELS);
	readLump(reader, mTexInfos, mNumTexInfos, header, LUMP_TEXINFO);
	readLump(reader, mTexDatas, mNumTexDatas, header, LUMP_TEXDATA);
	readLump(reader, mLeafFaces, mNumLeafFaces, header, LUMP_LEAFFACES);

	int *stringTable;
	int stringTableLength;
	readLump(reader, stringTable, stringTableLength, header, LUMP_TEXDATA_STRING_TABLE);

	char *stringData;
	int stringDataLength;
	readLump(reader, stringData, stringDataLength, header, LUMP_TEXDATA_STRING_DATA);

	mTexDataStringTable = new std::string[stringTableLength];
	for(int i=0; i<stringTableLength; i++) {
		mTexDataStringTable[i] = std::string(&stringData[stringTable[i]]);
	}

	delete[] stringData;
	delete[] stringTable;

	unsigned char *visData;
	int visDataLength;
	readLump(reader, visData, visDataLength, header, LUMP_VISIBILITY);
	parseVisData(visData, visDataLength);
	delete[] visData;
}

void BSP::parseVisData(unsigned char *visData, int visDataLength)
{
	int numClusters = *(int*)visData;
	int *offsets = (int*)visData + 1;

	mVisData = new bool*[numClusters];
	for(int i=0; i<numClusters; i++) {
		int offset = offsets[i*2];

		mVisData[i] = new bool[numClusters];
		int cluster = 0;
		while(cluster < numClusters) {
			unsigned char byte = visData[offset];
			if(byte == 0) {
				offset++;
				byte = visData[offset];
				for(int j=0; j<byte * 8; j++) {
					mVisData[i][cluster] = false;
					cluster++;
				}
			} else {
				for(int j=0; j<8; j++) {
					mVisData[i][cluster] = (((byte >> j) & 0x1) == 0x1);
					cluster++;
				}
			}

			offset++;
		}
	}
}

}