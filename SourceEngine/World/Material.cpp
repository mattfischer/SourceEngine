#include "World/Material.hpp"

#include "File/VTF.hpp"

namespace World {

Material::Material(File::VMT *vmt, File::IReaderFactory *factory)
{
	if(vmt->hasParameter("$basetexture")) {
		const std::string &textureFilename = vmt->parameter("$basetexture");

		File::VTF *vtf = File::VTF::open(factory, "materials/" + textureFilename + ".vtf");
		if(vtf) {
			mTexture = new Draw::Texture(vtf);
		}
	} else {
		mTexture = 0;
	}
}

}