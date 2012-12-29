/*
 * DefaultSceneManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef DEFAULTSCENEMANAGER_H_
#define DEFAULTSCENEMANAGER_H_

#include "ISceneManager.h"

namespace oglre {

namespace engine {

class DefaultSceneManager: public ISceneManager {
public:
	DefaultSceneManager();
	virtual ~DefaultSceneManager();

	virtual ISceneNode* addDefaultSceneNode(const char* name, ISceneNode* parent = 0, glm::detail::int32 id = -1);
	virtual ISceneNode* addSceneNode(const char* name, ISceneNode* parent = 0, glm::detail::int32 id = -1);
	virtual ICameraSceneNode* addCamera(glm::vec3 position, glm::vec3 lookAt);
	virtual ICameraSceneNode* addCameraFPS(glm::vec3 position, glm::vec3 lookAt,
			glm::detail::uint32 speed, glm::detail::uint32 rotationSpeed);
	virtual void drawAll();
	
	virtual IModelManager* getModelManager();
};

}

}

#endif /* DEFAULTSCENEMANAGER_H_ */
