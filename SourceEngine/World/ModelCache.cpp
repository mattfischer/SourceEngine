#include "World/ModelCache.hpp"

namespace World {

ModelCache::ModelCache(File::IReaderFactory *factory)
{
	mFactory = factory;
}

Model *ModelCache::open(const std::string &filename)
{
	Model *model = 0;

	if(mMap.find(filename) == mMap.end()) {
		size_t pos = filename.find(".mdl");
		if(pos != filename.npos) {
			File::MDL *mdl = File::MDL::open(mFactory, filename);

			std::string vertices = filename;
			vertices.replace(pos, 4, ".vvd");
			File::VVD *vvd = File::VVD::open(mFactory, vertices);

			std::string mesh = filename;
			mesh.replace(pos, 4, ".vtx");
			File::VTX *vtx = File::VTX::open(mFactory, mesh);

			if(mdl && vvd && vtx) {
				model = new Model(mdl, vvd, vtx, mFactory);
			}
		}
		mMap[filename] = model;
	} else {
		model = mMap[filename];
	}

	return model;
}

}