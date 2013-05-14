/*
 * IModel.h
 *
 *  Created on: 2012-10-20
 *      Author: jarrett
 */

#ifndef IMODEL_H_
#define IMODEL_H_

#include "../glw/shaders/IShaderProgram.h"

namespace glr {
namespace models {
	
struct MeshData {
	std::string name;
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec3 > normals;
	std::vector< glm::vec2 > textureCoordinates;
	std::vector< glm::vec4 > colors;
};

struct MaterialData {
	std::string name;
	GLenum fill_mode;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 emission;
	glm::detail::float32 shininess;
	glm::detail::float32 strength;
};

struct TextureData {
	std::string filename;
};

struct AnimationData {
	std::string name;
	// TODO: animation data
};

struct ModelData {
	MeshData meshData;
	MaterialData materialData;
	TextureData textureData;
	AnimationData animationData;
};
	
class IModel {
public:
	IModel()
	{
	}
	;

	virtual ~IModel()
	{
	}
	;

	virtual void render(shaders::IShaderProgram* shader) = 0;
};
}
}
#endif /* IMODEL_H_ */
