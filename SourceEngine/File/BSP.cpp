#include "File/BSP.hpp"

#include <fstream>

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

struct GameLumpHeader {
	int id;
	unsigned short flags;
	unsigned short version;
	int fileOfs;
	int fileLen;
};

template <typename T> void readLump(File *file, T *&list, size_t &num, const Header &header, int lumpNum)
{
	const Lump &lump = header.lumps[lumpNum];

	num = lump.length / sizeof(T);
	list = new T[num];
	file->seek(lump.offset);
	file->read(list, lump.length);
}

BSP *BSP::open(Space *space, const std::string &filename)
{
	BSP *ret = 0;
	File *file = space->open(filename);

	if(file) {
		ret = new BSP(file);
		delete file;
	}

	return ret;
}

BSP::BSP(File *file)
{
    Header header;
    file->read(&header, sizeof(header));

	readLump(file, mVertices, mNumVertices, header, LUMP_VERTICES);
	readLump(file, mPlanes, mNumPlanes, header, LUMP_PLANES);
	readLump(file, mNodes, mNumNodes, header, LUMP_NODES);
	readLump(file, mLeaves, mNumLeaves, header, LUMP_LEAFS);
	readLump(file, mEdges, mNumEdges, header, LUMP_EDGES);
	readLump(file, mSurfEdges, mNumSurfEdges, header, LUMP_SURFEDGES);
	readLump(file, mFaces, mNumFaces, header, LUMP_FACES_HDR);
	readLump(file, mModels, mNumModels, header, LUMP_MODELS);
	readLump(file, mTexInfos, mNumTexInfos, header, LUMP_TEXINFO);
	readLump(file, mTexDatas, mNumTexDatas, header, LUMP_TEXDATA);
	readLump(file, mLeafFaces, mNumLeafFaces, header, LUMP_LEAFFACES);

	int *stringTable;
	unsigned int stringTableLength;
	readLump(file, stringTable, stringTableLength, header, LUMP_TEXDATA_STRING_TABLE);

	char *stringData;
	unsigned int stringDataLength;
	readLump(file, stringData, stringDataLength, header, LUMP_TEXDATA_STRING_DATA);

	mTexDataStringTable = new std::string[stringTableLength];
	for(unsigned int i=0; i<stringTableLength; i++) {
		mTexDataStringTable[i] = std::string(&stringData[stringTable[i]]);
	}

	delete[] stringData;
	delete[] stringTable;

	unsigned char *visData;
	unsigned int visDataLength;
	readLump(file, visData, visDataLength, header, LUMP_VISIBILITY);
	parseVisData(visData, visDataLength);
	delete[] visData;

	Lump &entityLump = header.lumps[LUMP_ENTITIES];
	mEntityKeyValue = new KeyValue(file, entityLump.offset, entityLump.length);
	mNumEntities = mEntityKeyValue->numSections();
	mEntities = new Entity[mNumEntities];
	for(unsigned int i=0; i<mNumEntities; i++) {
		mEntities[i].section = &mEntityKeyValue->section(i);
	}

	Lump &lightingLump = header.lumps[LUMP_LIGHTING_HDR];
	mLighting = new unsigned char[lightingLump.length];
	file->seek(lightingLump.offset);
	file->read(mLighting, lightingLump.length);

	Lump &gameLump = header.lumps[LUMP_GAME_LUMP];
	file->seek(gameLump.offset);
	int numGameLumps;
	file->read(&numGameLumps, sizeof(int));
	GameLumpHeader *gameLumps = new GameLumpHeader[numGameLumps];
	file->read(gameLumps, sizeof(GameLumpHeader) * numGameLumps);

	for(int i=0; i<numGameLumps; i++) {
		if(gameLumps[i].id == 'sprp') {
			unsigned char *staticProps = new unsigned char[gameLumps[i].fileLen];
			parseStaticProps(file, gameLumps[i].fileOfs);
		}
	}

	unsigned char *pakfile = new unsigned char[header.lumps[LUMP_PAKFILE].length];
	file->seek(header.lumps[LUMP_PAKFILE].offset);
	file->read(pakfile, header.lumps[LUMP_PAKFILE].length);
	std::ofstream outfile("pakfile.zip", std::ios_base::binary | std::ios_base::out);
	outfile.write((const char*)pakfile, header.lumps[LUMP_PAKFILE].length);
}

void BSP::parseVisData(unsigned char *visData, int visDataLength)
{
	mNumClusters = *(int*)visData;
	int *offsets = (int*)visData + 1;

	mVisData = new bool*[mNumClusters];
	for(unsigned int i=0; i<mNumClusters; i++) {
		int offset = offsets[i*2];

		mVisData[i] = new bool[mNumClusters];
		unsigned int cluster = 0;
		while(cluster < mNumClusters) {
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

void BSP::parseStaticProps(File *file, int offset)
{
	file->seek(offset);
	file->read(&mNumStaticPropNames, sizeof(size_t));
	mStaticPropNames = new std::string[mNumStaticPropNames];
	for(unsigned int i=0; i<mNumStaticPropNames; i++) {
		char name[128];
		file->read(name, 128);
		mStaticPropNames[i] = std::string(name);
	}

	file->read(&mNumStaticPropLeaves, sizeof(size_t));
	mStaticPropLeaves = new unsigned short[mNumStaticPropLeaves];
	file->read(mStaticPropLeaves, sizeof(unsigned short) * (int)mNumStaticPropLeaves);

	file->read(&mNumStaticProps, sizeof(size_t));
	mStaticProps = new StaticProp[mNumStaticProps];
	file->read(mStaticProps, sizeof(StaticProp) * (int)mNumStaticProps);
}

}