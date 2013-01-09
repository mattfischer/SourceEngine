#ifndef FORMAT_MDL_HPP
#define FORMAT_MDL_HPP

#include "File/File.hpp"
#include "File/Space.hpp"

#include <string>

namespace Format {
namespace MDL {

#pragma pack(push, 1)
struct Vector {
	float x;
	float y;
	float z;
};

struct Texture {
	int	nameOffset;
	char *name() { return (char*)this + nameOffset; }

	int	flags;
	int	used;
	int	unused;

	int	material;
	int	clientMaterial;

	int	unused2[10];
};

struct Mesh {
	int					material;

	int					modelIndex;

	int					numVertices;
	int					vertexOffset;

	int					numFlexes;
	int					flexIndex;

	int					materialType;
	int					materialParam;

	int					meshId;

	Vector				center;

	int					unused[17];
};

struct Model {
	char				name[64];

	int					type;

	float				boundingRadius;

	int					numMeshes;
	int					meshIndex;
	Mesh *mesh(int n) { return (Mesh*)((char*)this + meshIndex) + n; }

	int					numVertices;
	int					vertexIndex;
	int					tangentsIndex;

	int					numAttachments;
	int					attachmentIndex;

	int					numEyeballs;
	int					eyeballIndex;

	int					unused[10];
};

struct BodyPart {
	int					nameOffset;
	char *name() { return ((char*)this) + nameOffset; }

	int					numModels;
	int					base;
	int					modelOffset;
	Model *model(int n) { return (Model*)((char*)this + modelOffset) + n; }
};

struct Header
{
	int		id;
	int		version;
	int     unused0;
	char	name[64];
	int		dataLength;

	Vector		eyePosition;
	Vector		illumPosition;
	Vector		hullMin;
	Vector		hullMax;
	Vector	  	viewBBMin;
	Vector	 	viewBBMax;

	int		flags;

	int		boneCount;
	int		boneOffset;

	int		boneControllerCount;
	int		boneControllerOffset;

	int		hitBoxCount;
	int		hitBoxOffset;

	int		localAnimCount;
	int		localAnimOffset;

	int		localSeqCount;
	int		localSeqOffset;

	int		activityListVersion;
	int		eventsIndexed;

	int		textureCount;
	int		textureOffset;
	Texture *texture(int n) { return (Texture*)((char*)this + textureOffset) + n; }

	int		textureDirCount;
	int		textureDirOffset;

	int		skinReferenceCount;
	int		skinReferenceFamilyCount;
	int     skinReferenceIndex;
	unsigned short *skin(int family, int reference) { return (unsigned short*)((char*)this + skinReferenceIndex) + family * skinReferenceCount + reference; }

	int		bodyPartCount;
	int		bodyPartOffset;
	BodyPart *bodyPart(int n) { return (BodyPart*)((char*)this + bodyPartOffset) + n; }

	int		attachmentCount;
	int		attachmentOffset;

	int		localNodeCount;
	int		localNodeIndex;
	int		localNodeNameIndex;

	int		flexDescCount;
	int		flexDescIndex;

	int		flexControllerCount;
	int		flexControllerIndex;

	int		flexRulesCount;
	int		flexRulesIndex;

	int		ikChainCount;
	int		ikChainIndex;

	int		mouthsCount;
	int		mouthsIndex;

	int		localPoseParamCount;
	int		localPoseParamIndex;

	int		surfacePropIndex;

	int		keyValueIndex;
	int		keyValueCount;

	int		ikLockCount;
	int		ikLockIndex;

	float	mass;
	int		contents;

	int		includeModelCount;
	int		includeModelIndex;

	int		virtualModel;

	int		animblocksNameIndex;
	int		animblocksCount;
	int		animblocksIndex;

	int		animblockModel;

	int		boneTableNameIndex;

	int		vertexBase;
	int		offsetBase;

	unsigned char directionaldotproduct;

	unsigned char rootLod;

	unsigned char numAllowedRootLods;

	unsigned char unused1;
	int		unused2;

	int		flexControllerUICount;
	int		flexControllerUIIindex;

	int		Header2Index;

	int		unused;
};
#pragma pack(pop)

static Header *open(File::Space *space, const std::string &name)
{
	return (Header*)space->read(name);
}

}
}
#endif