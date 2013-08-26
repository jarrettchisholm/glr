/*
 * IModel.h
 *
 *  Created on: 2012-10-20
 *      Author: jarrett
 */

#ifndef IMODEL_H_
#define IMODEL_H_

#include "glm/gtc/quaternion.hpp"

#include "../glw/shaders/IShaderProgram.h"

namespace glr {
namespace models {
	
struct MeshData {
	std::string name;
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec3 > normals;
	std::vector< glm::vec2 > textureCoordinates;
	std::vector< glm::vec4 > colors;
	std::vector< glm::vec2 > bones;
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



// Bone information
struct Bone {
	std::string name;
	glm::mat4 boneOffset;
	glm::mat4 finalTransformation;
};

struct BoneData {
	std::string name;
	std::map< std::string, glm::detail::uint32 > boneIndexMap;
	std::vector< Bone > boneTransform;
	//glm::mat4 inverseTransformation;
};


// Animation information
struct BoneNode {
	std::string name;
	glm::mat4 transformation;
	std::vector< BoneNode > children;
};

struct AnimatedBoneNode {
	std::string name;
	std::vector< glm::detail::float64 > positionTimes;
	std::vector< glm::detail::float64 > rotationTimes;
	std::vector< glm::detail::float64 > scalingTimes;
	std::vector< glm::vec3 > positions;
	std::vector< glm::quat > rotations;
	std::vector< glm::vec3 > scalings;
};

struct Animation {
	std::string name;
	glm::detail::float64 duration;
	glm::detail::float64 ticksPerSecond;
	
	std::map< std::string, AnimatedBoneNode > animatedBoneNodes;
};

struct AnimationData {
	std::string name;
	
	BoneNode rootBoneNode;
	std::map< std::string, Animation > animations;
};



// Model information
struct ModelData {
	MeshData meshData;
	MaterialData materialData;
	TextureData textureData;
	BoneData boneData;
	AnimationData animationData;
};
	
class IModel {
public:
	virtual ~IModel()
	{
	}
	;

	virtual void render(shaders::IShaderProgram* shader) = 0;
};
}
}
#endif /* IMODEL_H_ */
