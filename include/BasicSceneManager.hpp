#ifndef BASICSCENEMANAGER_H_
#define BASICSCENEMANAGER_H_

#include "ISceneManager.hpp"
#include "glw/IOpenGlDevice.hpp"

#include "Camera.hpp"
#include "IdManager.hpp"
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
	
	virtual void setCamera(std::unique_ptr<ICamera> camera);
	
	virtual void drawAll();
	
	virtual ISceneNode* getSceneNode(Id id) const;
	virtual ISceneNode* getSceneNode(const std::string& name) const;
	virtual ICamera* getCamera() const;
	virtual ILight* getLight(Id id) const;
	virtual ILight* getLight(const std::string& name) const;

	virtual void destroySceneNode(Id id);
	virtual void destroySceneNode(const std::string& name);
	virtual void destroySceneNode(ISceneNode* node);
	virtual void destroyAllSceneNodes();
	virtual void destroyCamera();
	virtual void destroyLight(Id id);
	virtual void destroyLight(const std::string& name);
	virtual void destroyLight(ILight* node);
	virtual void destroyAllLights();

	virtual glmd::uint32 getNumSceneNodes() const;
	virtual glmd::uint32 getNumLights() const;

	virtual const std::vector<LightData>& getLightData();

	virtual void setDefaultShaderProgram(shaders::IShaderProgram* shaderProgram);
	shaders::IShaderProgram* getDefaultShaderProgram() const;

	virtual const glm::mat4& getModelMatrix() const;
	
	virtual models::IModelManager* getModelManager() const;
	virtual models::IBillboardManager* getBillboardManager() const;
	virtual shaders::IShaderProgramManager* getShaderProgramManager() const;

protected:
	std::vector< std::unique_ptr<ISceneNode> > sceneNodes_;
	std::unique_ptr<ISceneNode> rootSceneNode_;
	std::vector< std::unique_ptr<ILight> > lights_;
	std::unique_ptr<ICamera> camera_;
	
	glmd::uint32 nextSceneNodeId_;
	glmd::uint32 nextLightSceneNodeId_;
	
	IdManager idManager_;

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
