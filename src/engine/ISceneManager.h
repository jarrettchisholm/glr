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

namespace oglre {

namespace engine {

class ISceneManager {
public:
	virtual ~ISceneManager() {
	}
	;

	virtual ISceneNode* addDefaultSceneNode(const char* name, ISceneNode* parent = 0, glm::detail::int32 id = -1) = 0;
	virtual ISceneNode* addSceneNode(const char* name, ISceneNode* parent = 0, glm::detail::int32 id = -1) = 0;
	virtual ICameraSceneNode* addCamera(glm::vec3 position, glm::vec3 lookAt) = 0;
	virtual ICameraSceneNode* addCameraFPS(glm::vec3 position, glm::vec3 lookAt, glm::detail::uint32 speed, glm::detail::uint32 rotationSpeed) = 0;
	virtual void drawAll() = 0;
	
	virtual IModelManager* getModelManager() = 0;

protected:
	std::vector<ISceneNode*> sceneNodes_;
};

}

}

#endif /* ISCENEMANAGER_H_ */
