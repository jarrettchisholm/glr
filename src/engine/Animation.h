/*
 * Animation.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <vector>

#include <GL/gl.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include "../common/compatibility/Types.h"

//#include "../../vmath/Vector3f.h"


namespace icee {

namespace engine {

using namespace compatibility;

class Animation {
public:
	Animation(const aiAnimation* mesh);
	virtual ~Animation();

private:

};

}

}

#endif /* ANIMATION_H_ */
