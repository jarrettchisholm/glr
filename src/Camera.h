/*
 * Camera.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "ICamera.h"
#include "BasicSceneNode.h"

//#include "IInputListener.h"

namespace glr {
class Camera : public virtual ICamera, public BasicSceneNode {
public:
	Camera(IMatrixData* matrixData, IOpenGlDevice* openGlDevice);
	Camera(const std::string name, IMatrixData* matrixData, IOpenGlDevice* openGlDevice);
	virtual ~Camera();

	// inherited from ICamera
	virtual bool isActive();

	virtual void render();

	virtual void attach(models::IModel* model);

	virtual void move(MOVE_DIRECTION dir, bool enabled);
	// up/down
	virtual void rotateX(glm::detail::float32 degrees);
	// left/right
	virtual void rotateY(glm::detail::float32 degrees);

	virtual void tick(glm::detail::float32 time);

	virtual const glm::mat4& getViewMatrix();

private:
	glm::quat rotation_;
	glm::detail::int32 prevX_, prevY_;

	glm::mat4 viewMatrix_;

	char movement_[NUM_MOVE_DIRECTIONS];
	glm::detail::float32 moveSpeed_, rotSpeed_;
	void clearMovementBuffer();
	glm::detail::float32 xRot_, yRot_;

	void initialize();
};
}
#endif /* CAMERA_H_ */
