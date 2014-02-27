#ifndef ISCENEMANAGER_H_
#define ISCENEMANAGER_H_

#include "ISceneNode.h"
#include "ICamera.h"
#include "ILight.h"
#include "models/IModelManager.h"
#include "glw/shaders/IShaderProgramManager.h"

namespace glr
{

namespace glmd = glm::detail;

class ISceneManager {
public:
	virtual ~ISceneManager()
	{
	}
	;

	virtual ISceneNode* createSceneNode(const std::string& name = std::string()) = 0;
	virtual ICamera* createCamera(glm::detail::uint32 speed = 1, glm::detail::uint32 rotationSpeed = 5) = 0;
	virtual ILight* createLight(const std::string& name = std::string()) = 0;

	virtual void addCamera(std::shared_ptr<ICamera> camera) = 0;
	virtual void addSceneNode(std::shared_ptr<ISceneNode> sceneNode) = 0;

	virtual ISceneNode* getSceneNode(glm::detail::uint32 id) = 0;
	virtual ISceneNode* getSceneNode(const std::string& name) = 0;
	virtual ICamera* getCamera() = 0;
	virtual ILight* getLight(glm::detail::uint32 id) = 0;
	virtual ILight* getLight(const std::string& name) = 0;

	virtual void destroySceneNode(glm::detail::uint32 id) = 0;
	virtual void destroySceneNode(const std::string& name) = 0;
	virtual void destroySceneNode(ISceneNode* node) = 0;
	virtual void destroyAllSceneNodes() = 0;
	virtual void destroyCamera() = 0;
	virtual void destroyLight(glm::detail::uint32 id) = 0;
	virtual void destroyLight(const std::string& name) = 0;
	virtual void destroyLight(ILight* node) = 0;
	virtual void destroyAllLights() = 0;

	virtual glmd::uint32 getNumSceneNodes() = 0;
	virtual glmd::uint32 getNumLights() = 0;

	virtual void drawAll() = 0;

	// @Deprecated(?)
	virtual ISceneNode* getRootSceneNode() = 0;
	
	virtual void setDefaultShaderProgram(shaders::IShaderProgram* shaderProgram) = 0;
	
	virtual const glm::mat4& getModelMatrix() = 0;

	virtual models::IModelManager* getModelManager() = 0;
	virtual shaders::IShaderProgramManager* getShaderProgramManager() = 0;

	virtual const std::vector<LightData>& getLightData() = 0;
};

}

#endif /* ISCENEMANAGER_H_ */
