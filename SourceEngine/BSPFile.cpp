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
	LUMP_UNUSED0,
	LUMP_PROPCOLLISION,
	LUMP_CLUSTERS,
	LUMP_UNUSED1,
	LUMP_PROPHULLS,
	LUMP_PORTALVERTS,
	LUMP_UNUSED2,
	LUMP_PROPHULLVERTS,
	LUMP_CLUSTERPORTALS,
	LUMP_UNUSED3,
	LUMP_PROPTRIS,
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
	LUMP_PROP_BLOB,
	LUMP_WATEROVERLAYS,
	LUMP_LIGHTMAPPAGES,
	LUMP_LEAF_AMBIENT_INDEX_HDR,
	LUMP_LIGHTMAPPAGEINFOS,
	LUMP_LEAF_AMBIENT_INDEX,
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

struct BSPVector {
	float x;
	float y;
	float z;
};

struct BSPPlane {
	BSPVector normal;
	float dist;
	int type;
};

struct BSPFace {
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

struct BSPEdge {
	short int v[2];
};

struct BSPModel {
	BSPVector mins;
	BSPVector maxs;
	BSPVector origin;
	int headNode;
	int firstFace;
	int numFaces;
};

char *readLump(IReader &reader, const Lump &lump)
{
	char *buffer = new char[lump.length];
	reader.seek(lump.offset);
	reader.read(buffer, lump.length);
	return buffer;
}
BSPFile::BSPFile(IReader &reader)
: mReader(reader)
{
    Header header;
    mReader.read((char*)&header, sizeof(header));

	BSPVector *vertices = (BSPVector*)readLump(mReader, header.lumps[LUMP_VERTICES]);
	BSPEdge *edges = (BSPEdge*)readLump(mReader, header.lumps[LUMP_EDGES]);
	int *surfEdges = (int*)readLump(mReader, header.lumps[LUMP_SURFEDGES]);
	BSPFace *faces = (BSPFace*)readLump(mReader, header.lumps[LUMP_FACES]);
	BSPModel *models = (BSPModel*)readLump(mReader, header.lumps[LUMP_MODELS]);

	mModel = new Model;
	mModel->numPolys = models[0].numFaces;
	mModel->polys = new Poly[mModel->numPolys];

	for(int i=0; i<mModel->numPolys; i++) {
		BSPFace *face = &faces[models[0].firstFace + i];
		Poly *poly = &mModel->polys[i];

		poly->numPoints = face->numEdges;
		poly->points = new Point[poly->numPoints];
		poly->gray = (rand() % 255) / 255.0f;
		for(int j=0; j<poly->numPoints; j++) {
			int surfEdge = surfEdges[face->firstEdge + j];
			int vertex;
			if(surfEdge > 0) {
				vertex = edges[surfEdge].v[0];
			} else {
				vertex = edges[-surfEdge].v[1];
			}
			poly->points[j].x = vertices[vertex].x;
			poly->points[j].y = vertices[vertex].y;
			poly->points[j].z = vertices[vertex].z;
		}
	}
}
