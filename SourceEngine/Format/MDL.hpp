#ifndef FORMAT_MDL_HPP
#define FORMAT_MDL_HPP

#include "File/File.hpp"
#include "File/Space.hpp"

#include <string>

namespace Format {

class MDL {
public:
	struct Vector {
		float x;
		float y;
		float z;
	};

	MDL(File::File *file);

	static MDL *open(File::Space *space, const std::string &filename);

	const std::string &texture(int texture) { return mTextures[texture]; }
	unsigned int numTextures() { return mNumTextures; }

	unsigned int numSkinZones() { return mNumSkinZones; }
	unsigned int numSkinFamilies() { return mNumSkinFamilies; }
	unsigned int skin(int family, int zone) { return mSkins[family * mNumSkinZones + zone]; }

	const Vector &hullMin() { return mHullMin; }
	const Vector &hullMax() { return mHullMax; }

private:
	std::string *mTextures;
	unsigned int mNumTextures;

	unsigned int mNumSkinZones;
	unsigned int mNumSkinFamilies;
	unsigned int *mSkins;

	Vector mHullMin;
	Vector mHullMax;
};

}
#endif