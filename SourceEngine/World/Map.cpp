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

	mSpace = multiSpace;

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

	mModelCache = new ModelCache(multiSpace);

	Entity::Point *playerStart;
	for(unsigned int i=0; i<file->numEntities(); i++) {
		mEntities[i] = 0;
		if(file->entity(i).section->parameter("classname") == "worldspawn") {
			mWorldSpawn = new Entity::WorldSpawn(file->entity(i).section);
		}

		if(file->entity(i).section->parameter("classname") == "info_player_start") {
			playerStart = new Entity::Point(file->entity(i).section);
		}
	}

	mPlayer = new Entity::Point("player");
	mPlayer->setPosition(playerStart->position() + Geo::Vector(0, 0, 60));
	mPlayer->setOrientation(playerStart->orientation());
	mEntities[mNumEntities - 1] = mPlayer;

	if(worldSpawn()->skyname() != "") {
		mSkybox = new Skybox(multiSpace, worldSpawn()->skyname());
	}

	mNumStaticProps = file->numStaticProps();
	mStaticProps = new Entity::Prop::Static*[mNumStaticProps];
	for(unsigned int i=0; i<mNumStaticProps; i++) {
		mStaticProps[i] = new Entity::Prop::Static(file, i, this);
	}

	delete zipSpace;
	delete multiSpace;
}

}