#include "World/ModelCache.hpp"

namespace World {

ModelCache::ModelCache(File::Space *space)
{
	mSpace = space;
}

Model *ModelCache::open(const std::string &filename)
{
	Model *model = 0;

	if(mMap.find(filename) == mMap.end()) {
		size_t pos = filename.find(".mdl");
		if(pos != filename.npos) {
			File::MDL *mdl = File::MDL::open(mSpace, filename);

			std::string vertices = filename;
			vertices.replace(pos, 4, ".vvd");
			File::VVD *vvd = File::VVD::open(mSpace, vertices);

			std::string mesh = filename;
			mesh.replace(pos, 4, ".vtx");
			File::VTX *vtx = File::VTX::open(mSpace, mesh);

			if(mdl && vvd && vtx) {
				std::string modelPath = filename.substr(0, filename.rfind("/"));
				model = new Model(mdl, vvd, vtx, mSpace, modelPath);
			}
		}
		mMap[filename] = model;
	} else {
		model = mMap[filename];
	}

	return model;
}

}