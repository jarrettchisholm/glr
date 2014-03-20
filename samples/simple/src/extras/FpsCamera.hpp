#ifndef FPSCAMERA_H_
#define FPSCAMERA_H_

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"

#include "GlrInclude.hpp"

namespace glr
{
namespace extras
{

class FpsCamera
{
public:
	FpsCamera(ICamera* camera, glmd::float32 speed);
	virtual ~FpsCamera();

	bool isActive();

	void moveForward();
	void moveBack();
	void moveLeft();
	void moveRight();
	
	ICamera* getCamera();
	
	void rotate(const glm::detail::float32& degrees, const glm::vec3& axis);

	/**
	 * 
	 */
	void tick(glmd::float32 time);

private:
	ICamera* camera_;
	glmd::float32 speed_;

	void initialize();
};

}
}

#endif /* FPSCAMERA_H_ */
