/*
 * Material.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <GL/gl.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include "../common/compatibility/Types.h"

namespace oglre {

namespace engine {

using namespace compatibility;

class Material {
public:
	Material(const aiMaterial* mtl);
	virtual ~Material();

private:
	GLenum fill_mode_;
	sint32 ret1_, ret2_;
	glm::vec4 diffuse_;
	glm::vec4 specular_;
	glm::vec4 ambient_;
	glm::vec4 emission_;
	float32 shininess_, strength_;
	sint32 two_sided_;
	sint32 two_sided_true_;
	sint32 wireframe_;
	uint32 max_;	// changed: to unsigned

	void bind();
};

}

}

#endif /* MATERIAL_H_ */
