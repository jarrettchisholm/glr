/*
 * Material.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <GL/glew.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include "../IMatrixData.h"
#include "../shaders/IShaderProgram.h"

namespace glr {
namespace models {
class Material {
public:
	Material(const aiMaterial* mtl);
	virtual ~Material();

	void bind(IMatrixData* matrixData, shaders::IShaderProgram* shader);

private:
	GLenum fill_mode_;
	glm::detail::int32 ret1_, ret2_;
	glm::vec4 diffuse_;
	glm::vec4 specular_;
	glm::vec4 ambient_;
	glm::vec4 emission_;
	glm::detail::float32 shininess_, strength_;
	glm::detail::int32 two_sided_;
	glm::detail::int32 two_sided_true_;
	glm::detail::int32 wireframe_;
	glm::detail::uint32 max_;               // changed: to unsigned
};
}
}
#endif /* MATERIAL_H_ */
