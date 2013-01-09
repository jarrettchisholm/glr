/*
 * DefaultSceneManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef DEFAULTSCENEMANAGER_H_
#define DEFAULTSCENEMANAGER_H_

#include "ISceneManager.h"

#include "CameraSceneNode.h"

namespace oglre {

namespace engine {

class DefaultSceneManager: public ISceneManager {
public:
	DefaultSceneManager();
	virtual ~DefaultSceneManager();

	virtual ISceneNode* createDefaultSceneNode( const std::string name, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 lookAt = glm::vec3(1, 1, 1) );
	virtual ISceneNode* createSceneNode( const std::string name, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 lookAt = glm::vec3(1, 1, 1) );
	virtual ICameraSceneNode* createCamera( const std::string name, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 lookAt = glm::vec3(1, 1, 1), glm::detail::uint32 speed = 1, glm::detail::uint32 rotationSpeed = 5 );
	virtual ICameraSceneNode* createCameraFPS( const std::string name, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 lookAt = glm::vec3(1, 1, 1), glm::detail::uint32 speed = 1, glm::detail::uint32 rotationSpeed = 5 );
	virtual void drawAll();
	
	virtual models::IModelManager* getModelManager();
	
	CameraSceneNode* getActiveCameraSceneNode();
	
private:
	std::vector<ISceneNode*> sceneNodes_;
	std::vector<CameraSceneNode*> cameras_;
};

}

}

#endif /* DEFAULTSCENEMANAGER_H_ */
