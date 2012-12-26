#include "World/Entity.hpp"

#include "StringUtils.hpp"

namespace World {

Entity::Entity(File::IReaderFactory *factory, File::BSP *file, int number)
{
	const File::BSP::Entity &fileEntity = file->entity(number);

	mModel = 0;

	if(fileEntity.section->hasParameter("classname")) {
		mClassname = fileEntity.section->parameter("classname");
	}

	if(fileEntity.section->hasParameter("origin")) {
		const std::string &position = fileEntity.section->parameter("origin");
		std::vector<std::string> posParts = StringUtils::split(position, " ");
		float x = (float)atof(posParts[0].c_str());
		float y = (float)atof(posParts[1].c_str());
		float z = (float)atof(posParts[2].c_str());
		mPosition = Geo::Point(x, y, z);
	}

	if(fileEntity.section->hasParameter("angles")) {
		const std::string &angles = fileEntity.section->parameter("angles");
		std::vector<std::string> angleParts = StringUtils::split(angles, " ");
		float pitch = (float)atof(angleParts[0].c_str());
		float yaw = (float)atof(angleParts[1].c_str());
		float roll = (float)atof(angleParts[2].c_str());
		mOrientation = Geo::Orientation(pitch, yaw, roll);
	}

	if(fileEntity.section->hasParameter("model")) {
		const std::string &modelFilename = fileEntity.section->parameter("model");

		size_t pos = modelFilename.find(".mdl");
		if(modelFilename[0] != '*' && pos != modelFilename.npos) {
			File::MDL *mdl = File::MDL::open(factory, modelFilename);

			std::string vertices = modelFilename;
			vertices.replace(pos, 4, ".vvd");
			File::VVD *vvd = File::VVD::open(factory, vertices);

			std::string mesh = modelFilename;
			mesh.replace(pos, 4, ".vtx");
			File::VTX *vtx = File::VTX::open(factory, mesh);

			if(mdl && vvd && vtx) {
				mModel = new Model(mdl, vvd, vtx, factory);
			}
		}
	}
}

}