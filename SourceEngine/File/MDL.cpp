#include "File/MDL.hpp"

namespace File {

struct Header
{
	int		id;
	int		version;
	int     unused0;
	char	name[64];
	int		dataLength;
 
	MDL::Vector		eyePosition;
	MDL::Vector		illumPosition;
	MDL::Vector		hullMin;
	MDL::Vector		hullMax;
	MDL::Vector	  	viewBBMin;
	MDL::Vector	 	viewBBMax;
 
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

struct Texture {
	int	nameOffset;
	int	flags;
	int	used;
	int	unused;

	int	material;
	int	clientMaterial;

	int	unused2[10];
};

MDL::MDL(IReader *reader)
{
	char *data = new char[reader->size()];
	reader->read(data, reader->size());

	Header header;

	memcpy(&header, data, sizeof(header));

	mHullMin = header.hullMin;
	mHullMax = header.hullMax;

	mNumTextures = header.textureCount;
	mTextures = new std::string[mNumTextures];
	for(unsigned int i=0; i<mNumTextures; i++) {
		Texture texture;
		char *texturePointer = data + header.textureOffset + i * sizeof(Texture);
		memcpy(&texture, texturePointer, sizeof(Texture));
		char *name = texturePointer + texture.nameOffset;
		mTextures[i] = std::string(name);
	}

	mNumSkinZones = header.skinReferenceCount;
	mNumSkinFamilies = header.skinReferenceFamilyCount;
	mSkins = new unsigned int[mNumSkinZones * mNumSkinFamilies];
	unsigned short *skinTable = (unsigned short*)(data + header.skinReferenceIndex);
	for(unsigned int i=0; i<mNumSkinZones * mNumSkinFamilies; i++) {
		mSkins[i] = skinTable[i];
	}

	delete[] data;
}

MDL *MDL::open(IReaderFactory *factory, const std::string &filename)
{
	MDL *ret = 0;
	IReader *reader = factory->open(filename);

	if(reader) {
		ret = new MDL(reader);
		delete reader;
	}

	return ret;
}

}