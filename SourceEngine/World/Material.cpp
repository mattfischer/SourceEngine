#include "World/Material.hpp"

#include "File/VTF.hpp"

namespace World {

Material::Material(File::VMT *vmt, File::Space *space)
{
	mTexture = 0;

	if(vmt->hasParameter("$basetexture")) {
		const std::string &textureFilename = vmt->parameter("$basetexture");

		File::VTF *vtf = File::VTF::open(space, "materials/" + textureFilename + ".vtf");
		if(vtf) {
			mTexture = new Draw::Texture(vtf);
			delete vtf;
		}
	}
}

}