#include "World/Material.hpp"

#include "Format/VTF.hpp"

namespace World {

Material::Material(Format::VMT *vmt, File::Space *space)
{
	mTexture = 0;

	if(vmt->hasParameter("$basetexture")) {
		std::string textureFilename = "materials/" + vmt->parameter("$basetexture");

		if(textureFilename.find(".vtf") == textureFilename.npos) {
			textureFilename += ".vtf";
		}

		Format::VTF *vtf = Format::VTF::open(space, textureFilename);
		if(vtf) {
			mTexture = new Draw::Texture(vtf);
			delete vtf;
		}
	}

	mTranslucent = false;
	if(vmt->hasParameter("$translucent")) {
		const std::string &translucent = vmt->parameter("$translucent");

		if(translucent == "1") {
			mTranslucent = true;
		}
	}

	if(vmt->hasParameter("$alphatest")) {
		const std::string &alphatest = vmt->parameter("$alphatest");

		if(alphatest == "1") {
			mAlphaTest = true;
		}
	}
}

}