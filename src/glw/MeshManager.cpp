/*
 * MeshManager.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include "MeshManager.h"


namespace glr {
namespace glw {
	
MeshManager::MeshManager(IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
}

MeshManager::~MeshManager()
{
}

Mesh* MeshManager::getMesh(const std::string filename)
{
	if ( meshes_.find(filename) != meshes_.end() )
	{
		BOOST_LOG_TRIVIAL(debug) << "Mesh found.";
		return meshes_[filename].get();
	}

	BOOST_LOG_TRIVIAL(debug) << "Mesh not found.";
	
	return nullptr;
}

Mesh* MeshManager::addMesh(
		const std::string path, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors
	)
{
	// TODO: implement
	
	return nullptr;
}

}
}
