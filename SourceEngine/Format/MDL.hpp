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