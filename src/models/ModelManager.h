/*
 * ModelManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef MODELMANAGER_H_
#define MODELMANAGER_H_

#include <memory>
#include <string>
#include <map>

#include <GL/glew.h>

// assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "IModelManager.h"
//#include "../glw/MaterialManager.h"
//#include "../glw/TextureManager.h"
//#include "../glw/MeshManager.h"

#include "../glw/IOpenGlDevice.h"

#include "Model.h"

namespace glr {
namespace models {
class ModelManager : public IModelManager {
public:
	ModelManager(glw::IOpenGlDevice* openGlDevice);
	virtual ~ModelManager();

	IModel* loadModel(const std::string path);

	//void testLoadTexture();
	//void testDrawTest1();

private:	
	glw::IOpenGlDevice* openGlDevice_;

	aiLogStream stream;

	std::map< std::string, std::unique_ptr<Model> > models_;
	
	//glw::IMaterialManager* materialManager_;
	//glw::ITextureManager* textureManager_;
	//glw::IMeshManager* meshManager_;
};
}
}
#endif /* MODELMANAGER_H_ */
