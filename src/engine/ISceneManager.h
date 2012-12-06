/*
 * ISceneManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef ISCENEMANAGER_H_
#define ISCENEMANAGER_H_

#include "ISceneNode.h"
#include "ICameraSceneNode.h"
#include "IModelManager.h"

#include "../common/compatibility/Types.h"

namespace icee {

namespace engine {

using namespace compatibility;

class ISceneManager {
public:
	virtual ~ISceneManager() {
	}
	;

	virtual ISceneNode* addDefaultSceneNode(const char* name, ISceneNode* parent = 0, sint32 id = -1) = 0;
	virtual ISceneNode* addSceneNode(const char* name, ISceneNode* parent = 0, sint32 id = -1) = 0;
	virtual ICameraSceneNode* addCamera(vmath::Vector3f position, vmath::Vector3f lookAt) = 0;
	virtual ICameraSceneNode* addCameraFPS(vmath::Vector3f position, vmath::Vector3f lookAt, uint32 speed, uint32 rotationSpeed) = 0;
	virtual void drawAll() = 0;
	
	virtual IModelManager* getModelManager() = 0;

protected:
	std::vector<ISceneNode*> sceneNodes_;
};

}

}

#endif /* ISCENEMANAGER_H_ */
