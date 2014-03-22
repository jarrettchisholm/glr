#ifndef ENVIRONMENTMANAGER_H_
#define ENVIRONMENTMANAGER_H_

#include <vector>
#include <memory>

#include "IdManager.hpp"

#include "glw/IOpenGlDevice.hpp"
#include "models/IModelManager.hpp"

#include "environment/IEnvironmentManager.hpp"
#include "environment/SkyBox.hpp"

namespace glr
{
namespace env
{

class EnvironmentManager : public IEnvironmentManager
{
public:
	EnvironmentManager(glw::IOpenGlDevice* openGlDevice, models::IModelManager* modelManager);
	virtual ~EnvironmentManager();
	
	virtual ISky* createSkyBox();
	virtual ISky* getSkyBox(Id id);
	virtual ISky* getSkyBox(const std::string& name);
	
	virtual void destroySkyBox(Id id);
	virtual void destroySkyBox(const std::string& name);
	virtual void destroySkyBox(ISky* skybox);
	
	virtual void render();
	
	virtual void setFollowTarget(ISceneNode* target);

private:
	glw::IOpenGlDevice* openGlDevice_;
	models::IModelManager* modelManager_;
	ISceneNode* followTarget_;
	
	IdManager idManager_;

	std::vector< std::unique_ptr<SkyBox> > skyBoxes_;
};

}
}

#endif /* ENVIRONMENTMANAGER_H_ */
