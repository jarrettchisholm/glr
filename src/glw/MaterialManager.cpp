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

Material* MaterialManager::getMaterial(const std::string name)
{
	if ( materials_.find(name) != materials_.end() )
	{
		BOOST_LOG_TRIVIAL(debug) << "Material found.";
		return materials_[name].get();
	}

	BOOST_LOG_TRIVIAL(debug) << "Material not found.";
	
	return nullptr;
}

Material* MaterialManager::addMaterial(
		const std::string name,
		glm::vec4 ambient,
		glm::vec4 diffuse,
		glm::vec4 specular,
		glm::vec4 emission,
		glm::detail::float32 shininess,
		glm::detail::float32 strength
	)
{
	BOOST_LOG_TRIVIAL(debug) << "Loading material...";

	if ( materials_.find(name) != materials_.end() && materials_[name].get() != nullptr )
	{
		BOOST_LOG_TRIVIAL(debug) << "Material already exists.";
		return materials_[name].get();
	}

	materials_[name] = std::unique_ptr<Material>(new Material(openGlDevice_, name, ambient, diffuse, specular, emission, shininess, strength));

	return materials_[name].get();
}
}
}
