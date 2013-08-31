/*
 * Animation.cpp
 *
 *  Created on: 2012-05-08
 *	  Author: jarrett
 */

#include <glm/gtc/matrix_transform.hpp>

#include "Animation.h"

#include "Constants.h"

namespace glr {
namespace glw {
	
Animation::Animation(
		IOpenGlDevice* openGlDevice,
		const std::string name, 
		glm::detail::float64 duration, 
		glm::detail::float64 ticksPerSecond, 
		std::map< std::string, AnimatedBoneNode > animatedBoneNodes
	) : openGlDevice_(openGlDevice), name_(name), duration_(duration), ticksPerSecond_(ticksPerSecond), animatedBoneNodes_(animatedBoneNodes)
{
	BOOST_LOG_TRIVIAL(debug) << "loading animation...";
	
	// Error check - default to 25 ticks per second
	ticksPerSecond_ = ( ticksPerSecond_ != 0.0f ? ticksPerSecond_ : 25.0f );
	
	currentTransforms_ = std::vector< glm::mat4 >();
	
	setupAnimationUbo();
	
	GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{
		// TODO: throw error
		BOOST_LOG_TRIVIAL(error) << "Error loading animation in opengl";
		BOOST_LOG_TRIVIAL(error) << "OpenGL error: " << err.name;
	}
	else
	{
		BOOST_LOG_TRIVIAL(debug) << "Successfully loaded animation.  Buffer id: " << bufferId_;
	}
}

Animation::~Animation()
{
	openGlDevice_->releaseBufferObject( bufferId_ );
}

void Animation::setupAnimationUbo()
{
	bufferId_ = openGlDevice_->createBufferObject(GL_UNIFORM_BUFFER, Constants::MAX_NUMBER_OF_BONES_PER_MESH * sizeof(glm::mat4), &currentTransforms_[0]);
}

void Animation::loadIntoVideoMemory()
{
	// TODO: implement
	
	glBindBuffer(GL_UNIFORM_BUFFER, bufferId_);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, currentTransforms_.size() * sizeof(glm::mat4), &currentTransforms_[0]);
	
	//GLuint bindPoint = openGlDevice_->bindBuffer( ubo );
	
	//shader->bindVariableByBindingName(shaders::IShader::BIND_TYPE_LIGHT, bindPoint);
}

void Animation::bind()
{
	// TODO: implement
	
	loadIntoVideoMemory();

	bindPoint_ = openGlDevice_->bindBuffer( bufferId_ );
}

GLuint Animation::getBufferId()
{
	return bufferId_;
}

GLuint Animation::getBindPoint()
{
	return bindPoint_;
}

/**
 * Will progress the animation on to the next key-frame.
 */
void Animation::tick(glmd::float32 elapsedTime)
{
	// TODO: implement
	
	/*
	glm::mat4 Identity;
	Identity.InitIdentity();
	
	float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float animationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);

	ReadNodeHeirarchy(animationTime, m_pScene->mRootNode, Identity);

	Transforms.resize(m_NumBones);

	for (uint i = 0 ; i < m_NumBones ; i++) {
		Transforms[i] = boneData[i].FinalTransformation;
	}
	*/	
}

/*
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
 
struct BoneNode {
	std::string name;
	glm::mat4 transformation;
	std::vector< BoneNode > children;
};
*/

glmd::uint32 Animation::findPosition(float animationTime, AnimatedBoneNode* animatedBoneNode)
{    
    for (glmd::uint32 i = 0; i < animatedBoneNode->positionTimes_.size() - 1; i++) {
        if (animationTime < (float)animatedBoneNode->positionTimes_[i + 1]) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}

glmd::uint32 Animation::findRotation(float animationTime, AnimatedBoneNode* animatedBoneNode)
{
    assert(animatedBoneNode->rotationTimes_.size() > 0);

    for (glmd::uint32 i = 0; i < animatedBoneNode->rotationTimes_.size() - 1; i++)
    {
        if (animationTime < (float)animatedBoneNode->rotationTimes_[i + 1])
        {
            return i;
        }
    }
    
    assert(0);

    return 0;
}


glmd::uint32 Animation::findScaling(float animationTime, AnimatedBoneNode* animatedBoneNode)
{
    assert(animatedBoneNode->scalingTimes_.size() > 0);
    
    for (glmd::uint32 i = 0; i < animatedBoneNode->scalingTimes_.size() - 1; i++)
    {
        if (animationTime < (float)animatedBoneNode->scalingTimes_[i + 1])
        {
            return i;
        }
    }
    
    assert(0);

    return 0;
}

void Animation::calcInterpolatedPosition(glm::vec3& Out, float animationTime, AnimatedBoneNode* animatedBoneNode)
{
    if (animatedBoneNode->positionTimes_.size() == 1)
    {
        Out = animatedBoneNode->positions_[0];
        return;
    }
            
    glmd::uint32 PositionIndex = findPosition(animationTime, animatedBoneNode);
    glmd::uint32 NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < animatedBoneNode->positionTimes_.size());
    float DeltaTime = (float)(animatedBoneNode->positionTimes_[NextPositionIndex] - animatedBoneNode->positionTimes_[PositionIndex]);
    float Factor = (animationTime - (float)animatedBoneNode->positionTimes_[PositionIndex]) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::vec3& Start = animatedBoneNode->positions_[PositionIndex];
    const glm::vec3& End = animatedBoneNode->positions_[NextPositionIndex];
    glm::vec3 Delta = End - Start;
    Out = Start + Factor * Delta;
}


void Animation::calcInterpolatedRotation(glm::quat& Out, float animationTime, AnimatedBoneNode* animatedBoneNode)
{
	// we need at least two values to interpolate...
    if (animatedBoneNode->rotationTimes_.size() == 1)
    {
        Out = animatedBoneNode->rotations_[0];
        return;
    }
    
    glmd::uint32 RotationIndex = findRotation(animationTime, animatedBoneNode);
    glmd::uint32 NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < animatedBoneNode->rotationTimes_.size());
    float DeltaTime = (float)(animatedBoneNode->rotationTimes_[NextRotationIndex] - animatedBoneNode->rotationTimes_[RotationIndex]);
    float Factor = (animationTime - (float)animatedBoneNode->rotationTimes_[RotationIndex]) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::quat& StartRotationQ = animatedBoneNode->rotations_[RotationIndex];
    const glm::quat& EndRotationQ   = animatedBoneNode->rotations_[NextRotationIndex];    
    Out = glm::mix(StartRotationQ, EndRotationQ, Factor);
    Out = glm::normalize( Out );
}


void Animation::calcInterpolatedScaling(glm::vec3& Out, float animationTime, AnimatedBoneNode* animatedBoneNode)
{
    if (animatedBoneNode->scalingTimes_.size() == 1)
    {
        Out = animatedBoneNode->scalings_[0];
        return;
    }

    glmd::uint32 ScalingIndex = findScaling(animationTime, animatedBoneNode);
    glmd::uint32 NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < animatedBoneNode->scalingTimes_.size());
    float DeltaTime = (float)(animatedBoneNode->scalingTimes_[NextScalingIndex] - animatedBoneNode->scalingTimes_[ScalingIndex]);
    float Factor = (animationTime - (float)animatedBoneNode->scalingTimes_[ScalingIndex]) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::vec3& Start = animatedBoneNode->scalings_[ScalingIndex];
    const glm::vec3& End   = animatedBoneNode->scalings_[NextScalingIndex];
    glm::vec3 Delta = End - Start;
    Out = Start + Factor * Delta;
}

void Animation::readNodeHeirarchy(glmd::float32 animationTime, glm::mat4& globalInverseTransform, BoneNode& rootBoneNode, BoneData& boneData, const glm::mat4& parentTransform)
{
	glm::mat4 nodeTransformation = glm::mat4( rootBoneNode.transformation );
	
	AnimatedBoneNode* animatedBoneNode = nullptr;
	
	if ( animatedBoneNodes_.find( rootBoneNode.name ) != animatedBoneNodes_.end() )
	{
		//BOOST_LOG_TRIVIAL(debug) << "Found animated bone node with name: " << rootBoneNode.name;
		animatedBoneNode = &(animatedBoneNodes_[rootBoneNode.name]);
	}
	else
	{
		// TODO: throw error?
		//BOOST_LOG_TRIVIAL(error) << "Error reading animation - unable to find animated bone node with name: " << rootBoneNode.name;
	}
	
	if (animatedBoneNode != nullptr)
	{
		// Interpolate scaling and generate scaling transformation matrix
		glm::vec3 Scaling;
		calcInterpolatedScaling(Scaling, animationTime, animatedBoneNode);
		glm::mat4 ScalingM = glm::scale( glm::mat4(1.0f), glm::vec3(Scaling.x, Scaling.y, Scaling.z) );
		
		// Interpolate rotation and generate rotation transformation matrix
		glm::quat RotationQ;
		calcInterpolatedRotation(RotationQ, animationTime, animatedBoneNode);
		glm::mat4 RotationM = glm::mat4_cast(RotationQ);

		// Interpolate translation and generate translation transformation matrix
		glm::vec3 Translation;
		calcInterpolatedPosition(Translation, animationTime, animatedBoneNode);
		glm::mat4 TranslationM = glm::translate( glm::mat4(1.0f), glm::vec3(Translation.x, Translation.y, Translation.z) );
		
		// Combine the above transformations
		nodeTransformation = TranslationM * RotationM * ScalingM;
	}
	   
	glm::mat4 globalTransformation = parentTransform * nodeTransformation;
	
	if (boneData.boneIndexMap.find( rootBoneNode.name ) != boneData.boneIndexMap.end()) {
		glmd::uint32 boneIndex = boneData.boneIndexMap[ rootBoneNode.name ];
		boneData.boneTransform[boneIndex].finalTransformation = globalInverseTransform * globalTransformation * boneData.boneTransform[boneIndex].boneOffset;
	}
	
	for (glmd::uint32 i = 0; i < rootBoneNode.children.size(); i++) {
		readNodeHeirarchy(animationTime, globalInverseTransform, rootBoneNode.children[i], boneData, globalTransformation);
	}
}

void Animation::generateBoneTransforms(glmd::float32 elapsedTime, glm::mat4& globalInverseTransformation, BoneNode& rootBoneNode, BoneData& boneData)
{
	currentTransforms_ = std::vector< glm::mat4 >();	
	
	glm::mat4 identity = glm::mat4();
	
	runningTime_ += elapsedTime;
	
	glmd::float32 timeInTicks_ = runningTime_ * ticksPerSecond_;
	glmd::float32 animationTime = fmod(timeInTicks_, (glmd::float32)duration_);

	readNodeHeirarchy(animationTime, globalInverseTransformation, rootBoneNode, boneData, identity);

	//transforms.resize(m_NumBones);
	currentTransforms_.resize( boneData.boneTransform.size() );

	for (glmd::uint32 i = 0; i < boneData.boneTransform.size(); i++) {
		currentTransforms_[i] = boneData.boneTransform[i].finalTransformation;
	}
}

}
}

