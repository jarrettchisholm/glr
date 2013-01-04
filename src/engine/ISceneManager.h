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
#include "models/IModelManager.h"

namespace oglre {

namespace engine {

class ISceneManager {
public:	
	virtual ~ISceneManager() {
	}
	;

	virtual ISceneNode* createDefaultSceneNode( const std::string name, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 lookAt = glm::vec3(1, 1, 1) ) = 0;
	virtual ISceneNode* createSceneNode( const std::string name, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 lookAt = glm::vec3(1, 1, 1) ) = 0;
	virtual ICameraSceneNode* createCamera( const std::string name, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 lookAt = glm::vec3(1, 1, 1), glm::detail::uint32 speed = 1, glm::detail::uint32 rotationSpeed = 5 ) = 0;
	virtual ICameraSceneNode* createCameraFPS( const std::string name, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 lookAt = glm::vec3(1, 1, 1), glm::detail::uint32 speed = 1, glm::detail::uint32 rotationSpeed = 5 ) = 0;
	virtual void drawAll() = 0;
	
	virtual models::IModelManager* getModelManager() = 0;
};

}

}

#endif /* ISCENEMANAGER_H_ */
