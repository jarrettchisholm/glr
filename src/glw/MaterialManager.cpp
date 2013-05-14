/*
 * MaterialManager.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include "MaterialManager.h"


namespace glr {
namespace glw {
MaterialManager::MaterialManager(IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
}

MaterialManager::~MaterialManager()
{
}

Material* MaterialManager::getMaterial(const std::string filename)
{
	if ( materials_.find(filename) != materials_.end() )
	{
		BOOST_LOG_TRIVIAL(debug) << "Material found.";
		return materials_[filename].get();
	}

	BOOST_LOG_TRIVIAL(debug) << "Material not found.";
	
	return nullptr;
}

Material* MaterialManager::addMaterial(
		const std::string path,
		glm::vec4 ambient,
		glm::vec4 diffuse,
		glm::vec4 specular,
		glm::vec4 emission,
		glm::detail::float32 shininess,
		glm::detail::float32 strength
	)
{
	// TODO: implement
	
	return nullptr;
}
}
}
