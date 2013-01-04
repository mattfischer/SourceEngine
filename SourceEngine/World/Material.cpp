#include "World/Material.hpp"

#include "Format/VTF.hpp"

namespace World {

Material::Material(Format::VMT *vmt, File::Space *space)
{
	mTexture = 0;

	if(vmt->hasParameter("$basetexture")) {
		const std::string &textureFilename = vmt->parameter("$basetexture");

		Format::VTF *vtf = Format::VTF::open(space, "materials/" + textureFilename + ".vtf");
		if(vtf) {
			mTexture = new Draw::Texture(vtf);
			delete vtf;
		}
	}
}

}