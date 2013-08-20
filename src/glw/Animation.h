/*
 * Animation.h
 *
 *  Created on: 2012-05-08
 *      Author: jarrett
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <vector>

#include <GL/glew.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>


namespace glr {
namespace glw {
	
class Animation {
public:
	Animation(const std::string path, 
			std::vector< glm::vec3 > vertices);
	virtual ~Animation();

private:
};

}
}
#endif /* ANIMATION_H_ */
