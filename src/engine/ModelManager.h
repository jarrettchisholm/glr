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

/* gl.h we need OpenGL */
#include <GL/gl.h>

// assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../common/compatibility/Types.h"

#include "Model.h"

namespace icee {

namespace engine {
	
using namespace compatibility;

class ModelManager {
public:
	ModelManager();
	virtual ~ModelManager();

	Model* loadModel(const std::string path);
	void testLoadTexture();
	
	void testDrawTest1();
	
private:
	aiLogStream stream;
	
	std::map< std::string, std::unique_ptr<Model> > models_;
	
	
};

}

}

#endif /* MODELMANAGER_H_ */
