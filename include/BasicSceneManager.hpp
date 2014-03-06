#ifndef BASICSCENEMANAGER_H_
#define BASICSCENEMANAGER_H_

#include "ISceneManager.hpp"
#include "glw/IOpenGlDevice.hpp"

#include "Camera.hpp"
#include "glw/shaders/ShaderProgramManager.hpp"

namespace glr
{

class BasicSceneManager : public ISceneManager
{
public:
	BasicSceneManager(shaders::IShaderProgramManager* shaderProgramManager, glw::IOpenGlDevice* openGlDevice);
	virtual ~BasicSceneManager();

	virtual ISceneNode* createSceneNode(const std::string& name = std::string());
	virtual ICamera* createCamera(glm::detail::uint32 speed = 1, glm::detail::uint32 rotationSpeed = 5);
	virtual ILight* createLight(const std::string& name = std::string());
	
	virtual void addCamera(std::shared_ptr<ICamera> camera);
	virtual void addSceneNode(std::shared_ptr<ISceneNode> sceneNode);
	
	virtual void drawAll();
	
	virtual ISceneNode* getSceneNode(glm::detail::uint32 id);
	virtual ISceneNode* getSceneNode(const std::string& name);
	virtual ICamera* getCamera();
	virtual ILight* getLight(glm::detail::uint32 id);
	virtual ILight* getLight(const std::string& name);

	virtual void destroySceneNode(glm::detail::uint32 id);
	virtual void destroySceneNode(const std::string& name);
	virtual void destroySceneNode(ISceneNode* node);
	virtual void destroyAllSceneNodes();
	virtual void destroyCamera();
	virtual void destroyLight(glm::detail::uint32 id);
	virtual void destroyLight(const std::string& name);
	virtual void destroyLight(ILight* node);
	virtual void destroyAllLights();

	virtual glmd::uint32 getNumSceneNodes();
	virtual glmd::uint32 getNumLights();

	virtual ISceneNode* getRootSceneNode();

	virtual const std::vector<LightData>& getLightData();

	virtual void setDefaultShaderProgram(shaders::IShaderProgram* shaderProgram);
	shaders::IShaderProgram* getDefaultShaderProgram();

	virtual const glm::mat4& getModelMatrix();
	
	virtual models::IModelManager* getModelManager();
	virtual models::IBillboardManager* getBillboardManager();
	virtual shaders::IShaderProgramManager* getShaderProgramManager();

protected:
	std::vector< std::shared_ptr<ISceneNode> > sceneNodes_;
	std::shared_ptr<ISceneNode> rootSceneNode_;
	std::vector< std::shared_ptr<ILight> > lights_;
	std::shared_ptr<ICamera> camera_;
	
	glmd::uint32 nextSceneNodeId_;
	glmd::uint32 nextLightSceneNodeId_;

	std::unique_ptr<models::IModelManager> modelManager_;
	std::unique_ptr<models::IBillboardManager> billboardManager_;
	
	shaders::IShaderProgramManager* shaderProgramManager_;
	glw::IOpenGlDevice* openGlDevice_;

	std::vector<LightData> lightData_;
	
	shaders::IShaderProgram* defaultShaderProgram_;

	glm::mat4 modelMatrix_;
};

}

#endif /* BASICSCENEMANAGER_H_ */
