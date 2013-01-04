#include "World/Map.hpp"

#include "Format/BSP.hpp"

#include "File/ZipSpace.hpp"
#include "File/MultiSpace.hpp"

namespace World {

Map::Map(File::Space *space, const std::string &filename)
{
	Format::BSP *file = Format::BSP::open(space, filename);

	File::ZipSpace *zipSpace = new File::ZipSpace(file->pakfile(), file->pakfileSize());
	File::MultiSpace *multiSpace = new File::MultiSpace();
	multiSpace->addSpace(zipSpace);
	multiSpace->addSpace(space);

	mNumMaterials = file->numTexDatas();
	mMaterials = new Material*[mNumMaterials];
	for(unsigned int i=0; i<mNumMaterials; i++) {
		const Format::BSP::TexData &texData = file->texData(i);
		const std::string &materialFilename = file->texDataString(texData.nameStringTableID);

		Format::VMT *vmt = Format::VMT::open(multiSpace, "materials/" + materialFilename + ".vmt");
		if(vmt) {
			mMaterials[i] = new Material(vmt, multiSpace);
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

	ModelCache *modelCache = new ModelCache(multiSpace);

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
		mStaticProps[i] = new StaticProp(file, i, mBsp, multiSpace, modelCache);
	}

	delete zipSpace;
	delete multiSpace;
}

}