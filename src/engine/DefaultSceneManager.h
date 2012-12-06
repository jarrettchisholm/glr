/*
 * DefaultSceneManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef DEFAULTSCENEMANAGER_H_
#define DEFAULTSCENEMANAGER_H_

#include "ISceneManager.h"

namespace icee {

namespace engine {

using namespace compatibility;

class DefaultSceneManager: public ISceneManager {
public:
	DefaultSceneManager();
	virtual ~DefaultSceneManager();

	virtual ISceneNode* addDefaultSceneNode(const char* name, ISceneNode* parent = 0, sint32 id = -1);
	virtual ISceneNode* addSceneNode(const char* name, ISceneNode* parent = 0, sint32 id = -1);
	virtual ICameraSceneNode* addCamera(vmath::Vector3f position, vmath::Vector3f lookAt);
	virtual ICameraSceneNode* addCameraFPS(vmath::Vector3f position, vmath::Vector3f lookAt,
			uint32 speed, uint32 rotationSpeed);
	virtual void drawAll();
	
	virtual IModelManager* getModelManager();
};

}

}

#endif /* DEFAULTSCENEMANAGER_H_ */
