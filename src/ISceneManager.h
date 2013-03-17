/*
 * ISceneManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef ISCENEMANAGER_H_
#define ISCENEMANAGER_H_

#include "ISceneNode.h"
#include "ICamera.h"
#include "ILight.h"
#include "models/IModelManager.h"
#include "shaders/IShaderProgramManager.h"

namespace oglre {

class ISceneManager {
public:	
	virtual ~ISceneManager() {
	}
	;

	virtual ISceneNode* createSceneNode( const std::string name ) = 0;
	virtual ICamera* createCamera( const std::string name, glm::detail::uint32 speed = 1, glm::detail::uint32 rotationSpeed = 5 ) = 0;
	virtual ILight* createLight( const std::string name ) = 0;
	
	virtual ISceneNode* getSceneNode( const std::string& name ) = 0;
	virtual ICamera* getCamera( const std::string& name ) = 0;
	virtual ILight* getLight( const std::string& name ) = 0;
	
	virtual void destroySceneNode( const std::string& name ) = 0;
	virtual void destroySceneNode( ISceneNode* node ) = 0;
	virtual void destroyAllSceneNodes() = 0;
	virtual void destroyCamera( const std::string& name ) = 0;
	virtual void destroyCamera( ICamera* node ) = 0;
	virtual void destroyAllCameras() = 0;
	virtual void destroyLight( const std::string& name ) = 0;
	virtual void destroyLight( ILight* node ) = 0;
	virtual void destroyAllLights() = 0;
	
	virtual glmd::uint32 getNumSceneNodes() = 0;
	virtual glmd::uint32 getNumCameras() = 0;
	virtual glmd::uint32 getNumLights() = 0;
	
	virtual void drawAll() = 0;
	
	virtual ISceneNode* getRootSceneNode() = 0;
	
	virtual models::IModelManager* getModelManager() = 0;
	virtual shaders::IShaderProgramManager* getShaderProgramManager() = 0;
};

}

#endif /* ISCENEMANAGER_H_ */
