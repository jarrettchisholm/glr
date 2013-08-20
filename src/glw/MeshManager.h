/*
 * MeshManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef MESHMANAGER_H_
#define MESHMANAGER_H_

#include <memory>
#include <string>
#include <map>

#include "IMeshManager.h"

#include "IOpenGlDevice.h"

#include "Mesh.h"

namespace glr {
namespace glw {
class MeshManager : public IMeshManager {
public:
	MeshManager(IOpenGlDevice* openGlDevice);
	virtual ~MeshManager();

	virtual Mesh* getMesh(const std::string path);
	virtual Mesh* addMesh(
		const std::string path, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		std::vector< glm::vec2 > bones
	);
	
private:	
	IOpenGlDevice* openGlDevice_;

	std::map< std::string, std::unique_ptr<Mesh> > meshes_;
};
}
}
#endif /* MESHMANAGER_H_ */
