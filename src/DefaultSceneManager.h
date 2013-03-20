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
#include "shaders/ShaderProgramManager.h"

namespace glr {

class DefaultSceneManager: public ISceneManager {
public:
	DefaultSceneManager(shaders::ShaderProgramManager* shaderProgramManager);
	virtual ~DefaultSceneManager();

	virtual ISceneNode* createSceneNode( const std::string name );
	virtual ICamera* createCamera( const std::string name, glm::detail::uint32 speed = 1, glm::detail::uint32 rotationSpeed = 5 );
	virtual ILight* createLight( const std::string name );
	virtual void drawAll();
	
	virtual models::IModelManager* getModelManager();
	virtual shaders::IShaderProgramManager* getShaderProgramManager();
	
	virtual ISceneNode* getSceneNode( const std::string& name );
	virtual ICamera* getCamera( const std::string& name );
	virtual ILight* getLight( const std::string& name );
	
	virtual void destroySceneNode( const std::string& name );
	virtual void destroySceneNode( ISceneNode* node );
	virtual void destroyAllSceneNodes();
	virtual void destroyCamera( const std::string& name );
	virtual void destroyCamera( ICamera* node );
	virtual void destroyAllCameras();
	virtual void destroyLight( const std::string& name );
	virtual void destroyLight( ILight* node );
	virtual void destroyAllLights();
	
	virtual glmd::uint32 getNumSceneNodes();
	virtual glmd::uint32 getNumCameras();
	virtual glmd::uint32 getNumLights();
	
	virtual ISceneNode* getRootSceneNode();
	
	ICamera* getActiveCameraSceneNode();
	const glm::mat4& getModelMatrix();
	
private:
	std::map<std::string, std::shared_ptr<ISceneNode>> sceneNodes_;
	std::shared_ptr<ISceneNode> rootSceneNode_;
	std::map<std::string, std::shared_ptr<ICamera>> cameras_;
	std::map<std::string, std::shared_ptr<ILight>> lights_;
	
	std::unique_ptr<models::IModelManager> modelManager_;
	shaders::ShaderProgramManager* shaderProgramManager_;
	
	glm::mat4 modelMatrix_;
};

}

#endif /* DEFAULTSCENEMANAGER_H_ */
