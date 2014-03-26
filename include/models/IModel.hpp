#ifndef IMODEL_H_
#define IMODEL_H_

#include "IRenderable.hpp"
#include "glw/shaders/IShaderProgram.hpp"

#include "glw/IMesh.hpp"
#include "glw/ITexture.hpp"
#include "glw/IAnimation.hpp"

#include "Id.hpp"

namespace glr
{
namespace models
{
	
struct MeshData
{
	std::string name;
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec3 > normals;
	std::vector< glm::vec2 > textureCoordinates;
	std::vector< glm::vec4 > colors;
	std::vector< glw::VertexBoneData > bones;
};

struct MaterialData
{
	std::string name;
	GLenum fill_mode;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 emission;
	glm::detail::float32 shininess;
	glm::detail::float32 strength;
};

struct TextureData
{
	std::string filename;
	glw::TextureSettings settings;
};



// Bone information is kept in IMesh.hpp


// Animation information
// Note: BoneNode struct is kept in IAnimation.hpp

struct AnimatedBoneNode
{
	std::string name;
	std::vector< glm::detail::float64 > positionTimes;
	std::vector< glm::detail::float64 > rotationTimes;
	std::vector< glm::detail::float64 > scalingTimes;
	std::vector< glm::vec3 > positions;
	std::vector< glm::quat > rotations;
	std::vector< glm::vec3 > scalings;
};

struct AnimationData
{
	std::string name;
	glm::detail::float64 duration;
	glm::detail::float64 ticksPerSecond;
	
	std::map< std::string, AnimatedBoneNode > animatedBoneNodes;
};

struct AnimationSet
{
	std::string name;
	
	glw::BoneNode rootBoneNode;
	std::map< std::string, AnimationData > animations;
	glm::mat4 globalInverseTransformation;
};



// Model information
struct ModelData
{
	MeshData meshData;
	MaterialData materialData;
	TextureData textureData;
	glw::BoneData boneData;
};

/**
 * 
 */
class IModel : public virtual IRenderable
{
public:
	virtual ~IModel()
	{
	}
	;
	
	/**
	 * Get the id of this model.
	 * 
	 * @return The id of this model.
	 */
	virtual const Id& getId() const = 0;
	
	/**
	 * Get the name of this model.
	 * 
	 * @return The name of this model.
	 */
	virtual const std::string& getName() const = 0;

	/**
	 * Plays the given animation for this model.
	 * 
	 * @param animation
	 */
	virtual void playAnimation(glw::IAnimation* animation, glm::detail::float32 animationTime = 0.0f, bool loop = true) = 0;

	/**
	 * Plays the given animation for this model.
	 * 
	 * @param animation
	 */
	virtual void playAnimation(glw::IAnimation* animation, glm::detail::uint32 startFrame, glm::detail::uint32 endFrame, glm::detail::float32 animationTime = 0.0f, bool loop = true) = 0;
	
	// Sets the animation time for animation currently playing
	// TODO: Should this be here?
	virtual void setAnimationTime(glm::detail::float32 animationTime) = 0;
	
	/**
	 * Stop the currently playing animation for this model.
	 */
	virtual void stopAnimation() = 0;

	/**
	 * Returns the current animation that affects this model.
	 * 
	 * @return A pointer to the current animation that affects this model, or nullptr if there is no current animation.
	 */
	virtual glw::IAnimation* getPlayingAnimation() const = 0;

	/**
	 * Sets the current animation.  If nullptr is sent in as the animation, then no animation will be used when rendering
	 * this model.
	 * 
	 * @param animation The animation to use as the current animation.
	 */
	virtual std::vector<glw::IAnimation*> getAnimations() const = 0;
};

}
}

#endif /* IMODEL_H_ */
