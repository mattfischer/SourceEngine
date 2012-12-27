#include "World/Entity.hpp"

#include "StringUtils.hpp"

namespace World {

Entity::Entity(File::BSP *file, int number, ModelCache *modelCache)
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

		mModel = modelCache->open(modelFilename);
	}
}

}