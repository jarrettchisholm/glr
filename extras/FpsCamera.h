#ifndef FPSCAMERA_H_
#define FPSCAMERA_H_

#include "glm/glm.hpp"

#include "GlrInclude.h"

namespace glr
{
namespace extras
{

class FpsCamera : public Camera {
public:
	FpsCamera(glw::IOpenGlDevice* openGlDevice, glmd::float32 speed);
	virtual ~FpsCamera();

	bool isActive();

	void moveForward();
	void moveBack();
	void moveLeft();
	void moveRight();
	
	virtual void rotate(const glm::detail::float32& degrees, const glm::vec3& axis);

	/**
	 * 
	 */
	void tick(glmd::float32 time);

private:
	glmd::float32 speed_;

	void initialize();
};

}
}

#endif /* FPSCAMERA_H_ */
