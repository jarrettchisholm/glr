#ifndef ISKYBOX_H_
#define ISKYBOX_H_

#include <vector>

#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"

#include "ISceneNode.h"


namespace glr {

class ISkyBox : public virtual ISceneNode {
public:
	virtual ~ISkyBox()
	{
	}
	;
};

}
#endif /* ISKYBOX_H_ */
