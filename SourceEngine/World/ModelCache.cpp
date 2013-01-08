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
			Format::MDL *mdl = Format::MDL::open(mSpace, filename);

			std::string vertices = filename;
			vertices.replace(pos, 4, ".vvd");
			Format::VVD::Header *vvd = Format::VVD::open(mSpace, vertices);

			std::string mesh = filename;
			mesh.replace(pos, 4, ".vtx");
			Format::VTX *vtx = Format::VTX::open(mSpace, mesh);

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