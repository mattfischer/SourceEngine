#include "World/Skybox.hpp"

#include "Format/VMT.hpp"

namespace World {

Skybox::Skybox(File::Space *space, const std::string &name)
{
	const char *suffixes[] = { "UP", "DN", "LF", "RT", "FT", "BK" };

	for(int i=0; i<6; i++) {
		std::string filename = "materials/skybox/" + name + suffixes[i] + ".vmt";

		Format::VMT *vmt = Format::VMT::open(space, filename);
		mMaterials[i] = new Material(vmt, space);
	}
}

}