#ifndef SKYBOX_H_
#define SKYBOX_H_

#include <string>
#include <memory>
#include <utility>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "BasicSceneNode.hpp"
#include "environment/ISky.hpp"
#include "environment/SkyBoxPlane.hpp"

namespace glr
{
namespace env
{

class SkyBox : public virtual ISky, public BasicSceneNode
{
public:
	SkyBox(Id id, glw::IOpenGlDevice* openGlDevice, models::IModelManager* modelManager);
	SkyBox(Id id, std::string name, glw::IOpenGlDevice* openGlDevice, models::IModelManager* modelManager);
	virtual ~SkyBox();
	
	virtual void render();
	
	void move( const glm::vec3& movement );

private:
	void initialize(models::IModelManager* modelManager);
	
	std::vector< std::unique_ptr<SkyBoxPlane> > sides_;
	std::vector< std::unique_ptr<models::IModel> > models_;
};

}
}

#endif /* SKYBOX_H_ */
