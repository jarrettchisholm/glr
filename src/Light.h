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

namespace glr {
class Light : public virtual ILight, public DefaultSceneNode {
public:
	Light();
	Light(const std::string name);
	virtual ~Light();


	// inherited from ILight
	virtual void render();

	virtual void attach(models::IModel* model);

	virtual void setLightData(LightData data);

	virtual const LightData& getLightData();

private:
	LightData lightData_;

	void initialize();
};
}
#endif /* LIGHT_H_ */
