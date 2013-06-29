/*
 * FpsCamera.h
 *
 *  Created on: 2013-06-19
 *      Author: jarrett
 */

#ifndef FPSCAMERA_H_
#define FPSCAMERA_H_

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "GLRInclude.h"

namespace glr {
namespace extras {

class FpsCamera : public Camera {
public:
	FpsCamera(const std::string name, glw::IOpenGlDevice* openGlDevice, glmd::float32 speed);
	virtual ~FpsCamera();

	bool isActive();

	void moveForward();
	void moveBack();
	void moveLeft();
	void moveRight();
	
	virtual void rotate(const glm::vec3& radians);

	void tick(glmd::float32 time);

private:
	glmd::float32 speed_;

	void initialize();
};

}
}
#endif /* FPSCAMERA_H_ */
