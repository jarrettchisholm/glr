/*
 * Light.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "ILight.h"
#include "DefaultSceneNode.h"

namespace oglre {


class Light: public virtual ILight, public DefaultSceneNode {
public:
	Light();
	Light(glm::vec3 position, glm::vec3 lookAt, bool active = true);
	virtual ~Light();
	
	
	// inherited from ILight
	virtual void render();
	
	virtual void attach(models::IModel* model);
	virtual void setVisible(bool isVisible);

private:

	void initialize();
};

}

#endif /* LIGHT_H_ */
