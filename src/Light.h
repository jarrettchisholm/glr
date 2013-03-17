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
	Light(const std::string name);
	virtual ~Light();
	
	
	// inherited from ILight
	virtual void render();
	
	virtual void attach(models::IModel* model);

private:
	glm::vec4 ambient_;
	glm::vec4 diffuse_;
	glm::vec4 specular_;
	
	void initialize();
};

}

#endif /* LIGHT_H_ */
