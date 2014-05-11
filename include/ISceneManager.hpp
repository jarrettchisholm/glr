#ifndef ISCENEMANAGER_H_
#define ISCENEMANAGER_H_

#include "ISceneNode.hpp"
#include "ICamera.hpp"
#include "ILight.hpp"
#include "environment/IEnvironmentManager.hpp"
#include "models/IModelManager.hpp"
#include "models/IBillboardManager.hpp"
#include "terrain/ITerrainManager.hpp"
#include "terrain/IFieldFunction.hpp"
#include "terrain/TerrainSettings.hpp"
#include "glw/shaders/IShaderProgramManager.hpp"

namespace glr
{

namespace glmd = glm::detail;

class ISceneManager
{
public:
	virtual ~ISceneManager()
	{
	}
	;

	virtual ISceneNode* createSceneNode(const std::string& name = std::string()) = 0;
	virtual ICamera* createCamera(glm::detail::uint32 speed = 1, glm::detail::uint32 rotationSpeed = 5) = 0;
	virtual ILight* createLight(const std::string& name = std::string()) = 0;

	virtual void setCamera(std::unique_ptr<ICamera> camera) = 0;

	virtual ISceneNode* getSceneNode(Id id) const = 0;
	virtual ISceneNode* getSceneNode(const std::string& name) const = 0;
	virtual ICamera* getCamera() const = 0;
	virtual ILight* getLight(Id id) const = 0;
	virtual ILight* getLight(const std::string& name) const = 0;

	virtual void destroySceneNode(Id id) = 0;
	virtual void destroySceneNode(const std::string& name) = 0;
	virtual void destroySceneNode(ISceneNode* node) = 0;
	virtual void destroyAllSceneNodes() = 0;
	virtual void destroyCamera() = 0;
	virtual void destroyLight(Id id) = 0;
	virtual void destroyLight(const std::string& name) = 0;
	virtual void destroyLight(ILight* node) = 0;
	virtual void destroyAllLights() = 0;

	virtual glmd::uint32 getNumSceneNodes() const = 0;
	virtual glmd::uint32 getNumLights() const = 0;

	virtual void drawAll() = 0;
	
	virtual void setDefaultShaderProgram(shaders::IShaderProgram* shaderProgram) = 0;
	
	virtual const glm::mat4& getModelMatrix() const = 0;

	virtual env::IEnvironmentManager* getEnvironmentManager() = 0;
	// TODO: make this just a getter?
	virtual terrain::ITerrainManager* getTerrainManager(terrain::IFieldFunction* fieldFunction = nullptr, terrain::TerrainSettings terrainSettings = terrain::TerrainSettings()) = 0;
	virtual shaders::IShaderProgramManager* getShaderProgramManager() const = 0;

	virtual const std::vector<LightData>& getLightData() = 0;
};

}

#endif /* ISCENEMANAGER_H_ */
