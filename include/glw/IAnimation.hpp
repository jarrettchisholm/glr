#ifndef IANIMATION_H_
#define IANIMATION_H_

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "IAnimation.hpp"
#include "IMesh.hpp"

#include "IGraphicsObject.hpp"

#include "AnimatedBoneNode.hpp"

#include "common/utilities/Macros.hpp"

namespace glr
{
namespace glw
{

struct BoneNode
{
	std::string name;
	glm::mat4 transformation;
	std::vector< BoneNode > children;
};

/**
 * Represents an animation in OpenGL.
 * 
 * Typical usage looks like this:
 * 
 * GLint bindPoint = shader->getBindPointByBindingName( shaders::IShader::BIND_TYPE_BONE );
 * 
 * if (bindPoint >= 0)
 * {
 * 		// This is the 'time slice' of the animation you want to use
 * 		animation->setAnimationTime( 1.3f );
 * 		// Only play frames within this range
 * 		animation->setFrameClampping( startFrame_, endFrame_ );
 * 
 * 		// Calculate the transformations that are used to animation the mesh
 * 		animation->calculate(globalInverseTransformation, mesh.getSkeleton(), mesh.getBones());
 * 
 * 		// Bind the transformations into OpenGL
 * 		animation->bind();
 * 
 * 		openGlDevice->bindBuffer( animation->getBufferId(), bindPoint );
 * }
 */
class IAnimation : public virtual IGraphicsObject
{
public:
	virtual ~IAnimation()
	{
	}
	;

	/**
	 * Returns the name of this animation.
	 * 
	 * @return The name of the animation.
	 */
	virtual const std::string& getName() const = 0;

	/**
	 * Binds the data in OpenGL, making it ready to use.
	 */
	virtual void bind() const = 0;
	
	// Inherited from IGraphicsObject - need to declare it here so that it is visible to anything using an IAnimation object.
	virtual void pushToVideoMemory() = 0;
	
	/**
	 * Will stream the provided transformation matrices into opengl memory.
	 * 
	 * NOTE: You should only use this method if you know what you are doing!
	 * 
	 * @param transformations
	 */
	virtual void pushToVideoMemory(const std::vector< glm::mat4 >& transformations) = 0;

	// TODO: Should we have this in the interface?
	virtual GLuint getBufferId() const = 0;

	// TODO: Should we have this in the interface?
	/**
	 * Will generate the transformation matrices to be used to animate the model with the given bone data.
	 * 
	 * @param globalInverseTransformation - Not sure what this is...
	 * @param rootBoneNode - Root node of the skeleton used for the current animation.
	 * @param boneData - Data about the bones we are generating the animation on.
	 */
	virtual void calculate(const glm::mat4& globalInverseTransformation, const BoneNode& rootBoneNode, const BoneData& boneData) = 0;
	
	/**
	 * Will generate the transformation matrices to be used to animate the model with the given bone data.
	 * 
	 * @param globalInverseTransformation - Not sure what this is...
	 * @param rootBoneNode - Root node of the skeleton used for the current animation.
	 * @param boneData - Data about the bones we are generating the animation on.
	 * @param indexCache - If passed in, represents a cache of previous rotation, position, and scaling node search indices.  The algorithm to find
	 * the correct indices for the animation will use the indexCache index values to determine where to begin the search for the next
	 * rotation, position, and scaling index.
	 */
	virtual void calculate(const glm::mat4& globalInverseTransformation, const BoneNode& rootBoneNode, const BoneData& boneData, std::vector<glm::detail::uint32>& indexCache) = 0;
	
	/**
	 * Will generate the transformation matrices to be used to animate the model with the given bone data.  It will store these transformations in the 'transformations' parameter.
	 * 
	 * This is useful for generating a set of animation transformations for later use (i.e. caching or pre-generating animation transformations).
	 * 
	 * Note: This method should only be used if you know what you are doing.
	 * 
	 * @param transformations The container to store the animations transformations in.
	 * @param globalInverseTransformation - Not sure what this is...
	 * @param rootBoneNode - Root node of the skeleton used for the current animation.
	 * @param boneData - Data about the bones we are generating the animation on.
	 */
	virtual void calculate(std::vector< glm::mat4 >& transformations, const glm::mat4& globalInverseTransformation, const BoneNode& rootBoneNode, const BoneData& boneData) = 0;
	
	/**
	 * Will generate the transformation matrices to be used to animate the model with the given bone data.  It will store these transformations in the 'transformations' parameter.
	 * 
	 * This is useful for generating a set of animation transformations for later use (i.e. caching or pre-generating animation transformations).
	 * 
	 * Note: This method should only be used if you know what you are doing.
	 * 
	 * @param transformations The container to store the animations transformations in.
	 * @param globalInverseTransformation - Not sure what this is...
	 * @param rootBoneNode - Root node of the skeleton used for the current animation.
	 * @param boneData - Data about the bones we are generating the animation on.
	 * @param indexCache - If passed in, represents a cache of previous rotation, position, and scaling node search indices.  The algorithm to find
	 * the correct indices for the animation will use the indexCache index values to determine where to begin the search for the next
	 * rotation, position, and scaling index.
	 */
	virtual void calculate(std::vector< glm::mat4 >& transformations, const glm::mat4& globalInverseTransformation, const BoneNode& rootBoneNode, const BoneData& boneData, std::vector<glm::detail::uint32>& indexCache) = 0;

	/**
	 * Will set the animation time to runningTime.
	 * 
	 * Animation time is the current time within the animation that should be used for generating the transformation matrices.
	 * 
	 * If the provided runningTime is greater than the total animation time, it will use the runningTime modulus total animation length as the time to use for generating 
	 * the transformation matrices.
	 * 
	 * @param runningTime
	 */
	virtual void setAnimationTime(glm::detail::float32 runningTime) = 0;

	/**
	 * Set the animation to run only between the given start and end frame.
	 * 
	 * Note: A value of 0 for the start and end frame will run through all frames of the animation.
	 * 
	 * endFrame must be larger than or equal to startFrame.  
	 * 
	 * A value below zero for the start or end frame is invalid.
	 * 
	 * @param startFrame
	 * @param endFrame
	 */
	virtual void setFrameClampping(glm::detail::uint32 startFrame, glm::detail::uint32 endFrame) = 0;
	
	// TESTING Debug
	virtual void printTransformations() = 0;
	virtual void printTransformations(const std::vector< glm::mat4 >& transformations) = 0;
};

}
}

#endif /* IANIMATION_H_ */
