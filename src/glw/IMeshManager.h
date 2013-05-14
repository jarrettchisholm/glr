/*
 * IMeshManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef IMESHMANAGER_H_
#define IMESHMANAGER_H_

#include <string>

#include "Mesh.h"

namespace glr {
namespace glw {

class Mesh;

class IMeshManager {
public:
	virtual ~IMeshManager()
	{
	}
	;

	virtual Mesh* getMesh(const std::string path) = 0;
	virtual Mesh* addMesh(
		const std::string path, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors
	) = 0;
};

}
}
#endif /* IMESHMANAGER_H_ */
