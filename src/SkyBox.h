#ifndef SKYBOX_H_
#define SKYBOX_H_

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "ISkyBox.h"
#include "BasicSceneNode.h"

namespace glr {

class SkyBox : public virtual ISkyBox, public BasicSceneNode {
public:
	SkyBox(glw::IOpenGlDevice* openGlDevice);
	SkyBox(const std::string name, glw::IOpenGlDevice* openGlDevice);
	virtual ~SkyBox();

private:
	void initialize();
};

}
#endif /* SKYBOX_H_ */

