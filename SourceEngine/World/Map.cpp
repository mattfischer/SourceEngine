#include "World/Map.hpp"

#include "File/BSP.hpp"

namespace World {

Map::Map(File::IReaderFactory *factory, const std::string &filename)
{
	File::BSP *file = File::BSP::open(factory, filename);

	mNumMaterials = file->numTexDatas();
	mMaterials = new Material*[mNumMaterials];
	for(unsigned int i=0; i<mNumMaterials; i++) {
		const File::BSP::TexData &texData = file->texData(i);
		const std::string &materialFilename = file->texDataString(texData.nameStringTableID);

		File::VMT *vmt = File::VMT::open(factory, "materials/" + materialFilename + ".vmt");
		if(vmt) {
			mMaterials[i] = new Material(vmt, factory);
		} else {
			mMaterials[i] = 0;
		}
	}

	mNumFaces = file->numFaces();
	mFaces = new Face*[mNumFaces];
	for(unsigned int i=0; i<mNumFaces; i++) {
		mFaces[i] = new Face(file, i, mMaterials);
	}

	mBsp = new BSP(file, mFaces);

	ModelCache *modelCache = new ModelCache(factory);

	mNumEntities = file->numEntities();
	mEntities = new Entity*[mNumEntities];

	for(unsigned int i=0; i<mNumEntities; i++) {
		mEntities[i] = new Entity(file, i, mBsp, modelCache);

		if(mEntities[i]->classname() == "info_player_start") {
			mPlayerStart = mEntities[i];
		}
	}

	mNumStaticProps = file->numStaticProps();
	mStaticProps = new StaticProp*[mNumStaticProps];
	for(unsigned int i=0; i<mNumStaticProps; i++) {
		mStaticProps[i] = new StaticProp(file, i, mBsp, modelCache);
	}
}

}