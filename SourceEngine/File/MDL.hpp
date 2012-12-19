#ifndef FILE_MDL_HPP
#define FILE_MDL_HPP

#include "File/IReader.hpp"
#include "File/IReaderFactory.hpp"

#include <string>

namespace File {

class MDL {
public:
	MDL(IReader *reader);

	static MDL *open(IReaderFactory *factory, const std::string &name);

	const std::string &texture(int texture) { return mTextures[texture]; }
	unsigned int numTextures() { return mNumTextures; }

	unsigned int numSkinZones() { return mNumSkinZones; }
	unsigned int numSkinFamilies() { return mNumSkinFamilies; }
	unsigned int skin(int family, int zone) { return mSkins[family * mNumSkinZones + zone]; }

private:
	std::string *mTextures;
	unsigned int mNumTextures;

	unsigned int mNumSkinZones;
	unsigned int mNumSkinFamilies;
	unsigned int *mSkins;
};

}
#endif