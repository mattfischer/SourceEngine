#include "File/MDL.hpp"

namespace File {

struct Vector {
	float x;
	float y;
	float z;
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
 
	int		textureDirCount;
	int		textureDirOffset;
 
	int		skinReferenceCount;
	int		skinReferenceFamilyCount;
	int     skinReferenceIndex;
 
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

MDL::MDL(IReader *reader)
{
	Header header;

	reader->read(&header, sizeof(header));
}

MDL *MDL::open(IReaderFactory *factory, const std::string &name)
{
	MDL *ret = 0;
	IReader *reader = factory->open(name);

	if(reader) {
		ret = new MDL(reader);
		delete reader;
	}

	return ret;
}

}