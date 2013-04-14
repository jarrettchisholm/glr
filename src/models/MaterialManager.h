/*
 * MaterialManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef MATERIALMANAGER_H_
#define MATERIALMANAGER_H_

#include <memory>
#include <string>
#include <map>

#include <GL/glew.h>

// assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../IOpenGlDevice.h"

#include "IMaterialManager.h"

#include "Material.h"

namespace glr {
namespace models {
class MaterialManager : public IMaterialManager {
public:
	MaterialManager(IOpenGlDevice* openGlDevice);
	virtual ~MaterialManager();

	virtual Material* getMaterial(const std::string path);

private:
	aiLogStream stream;
	
	IOpenGlDevice* openGlDevice_;

	std::map< std::string, std::unique_ptr<Material> > materials_;
};
}
}
#endif /* MATERIALMANAGER_H_ */
