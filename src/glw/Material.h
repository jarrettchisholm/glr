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

#include "IOpenGlDevice.h"
#include "shaders/IShaderProgram.h"

namespace glr {
namespace glw {
	
class IOpenGlDevice;
	
struct MaterialData
{
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;	
	glm::vec4 emission;
	//glm::detail::float32 shininess;
	//glm::detail::float32 strength;
};
	
class Material {
public:
	Material(
		IOpenGlDevice* openGlDevice,
		const std::string path,
		glm::vec4 ambient,
		glm::vec4 diffuse,
		glm::vec4 specular,
		glm::vec4 emission,
		glm::detail::float32 shininess,
		glm::detail::float32 strength
	);
	virtual ~Material();

	void bind();
	GLuint getBufferId();
	GLuint getBindPoint();

private:
	void loadIntoVideoMemory();
	
	IOpenGlDevice* openGlDevice_;
	GLuint bufferId_;
	GLuint bindPoint_;

	GLenum fill_mode_;
	glm::detail::int32 ret1_, ret2_;
	glm::vec4 ambient_;
	glm::vec4 diffuse_;
	glm::vec4 specular_;
	glm::vec4 emission_;
	glm::detail::float32 shininess_, strength_;
	
	MaterialData materialData_;
	
	glm::detail::int32 two_sided_;
	glm::detail::int32 two_sided_true_;
	glm::detail::int32 wireframe_;
	glm::detail::uint32 max_;               // changed: to unsigned
};

}
}
#endif /* MATERIAL_H_ */
