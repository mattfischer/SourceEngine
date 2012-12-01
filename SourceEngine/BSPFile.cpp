#include "BSPFile.hpp"

#include <vector>

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

template <typename T> void readLump(sp<IReader> reader, std::vector<T> &vector, const Header &header, int lumpNum)
{
	const Lump &lump = header.lumps[lumpNum];
	vector.resize(lump.length / sizeof(T));
	reader->seek(lump.offset);
	reader->read((char*)&vector[0], lump.length);
}

BSPFile::BSPFile(sp<IReader> reader)
: mReader(reader)
{
    Header header;
    mReader->read((char*)&header, sizeof(header));

	readLump(mReader, mVertices, header, LUMP_VERTICES);
	readLump(mReader, mEdges, header, LUMP_EDGES);
	readLump(mReader, mSurfEdges, header, LUMP_SURFEDGES);
	readLump(mReader, mFaces, header, LUMP_FACES);
	readLump(mReader, mModels, header, LUMP_MODELS);
	readLump(mReader, mTexInfos, header, LUMP_TEXINFO);
	readLump(mReader, mTexDatas, header, LUMP_TEXDATA);

	std::vector<int> stringTable;
	readLump(mReader, stringTable, header, LUMP_TEXDATA_STRING_TABLE);

	std::vector<char> stringData;
	readLump(mReader, stringData, header, LUMP_TEXDATA_STRING_DATA);

	for(unsigned int i=0; i<stringTable.size(); i++) {
		mTexDataStringTable.push_back(std::string(&stringData[stringTable[i]]));
	}
}
