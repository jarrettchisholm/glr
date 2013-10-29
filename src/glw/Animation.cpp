/*
 * Animation.cpp
 *
 *  Created on: 2012-05-08
 *	  Author: jarrett
 */

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Animation.h"

#include "Constants.h"

namespace glr {
namespace glw {

Animation::Animation(IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
	name_ = std::string("NULL");
	duration_ = 0.0f;
	ticksPerSecond_ = 0.0f;
	animatedBoneNodes_ = std::map< std::string, AnimatedBoneNode >();
	runningTime_ = 0.0f;
	
	bufferId_ = 0;
	
	startFrame_ = 0;
	endFrame_ = 0;
	
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
		//BOOST_LOG_TRIVIAL(debug) << "Successfully loaded animation.  Buffer id: " << bufferId_;
	}
}

Animation::Animation(
		IOpenGlDevice* openGlDevice,
		const std::string name, 
		glm::detail::float64 duration, 
		glm::detail::float64 ticksPerSecond, 
		std::map< std::string, AnimatedBoneNode > animatedBoneNodes
	) : openGlDevice_(openGlDevice), name_(name), duration_(duration), ticksPerSecond_(ticksPerSecond), animatedBoneNodes_(animatedBoneNodes), runningTime_(0.0f)
{
	// We probably shouldn't have an animation object at all if it has no animated bone nodes...
	assert( animatedBoneNodes_.size() != 0 );
	
	//BOOST_LOG_TRIVIAL(debug) << "loading animation...";
	bufferId_ = 0;
	
	startFrame_ = 0;
	endFrame_ = 0;
	
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
		//BOOST_LOG_TRIVIAL(debug) << "Successfully loaded animation.  Buffer id: " << bufferId_;
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
	glBindBuffer(GL_UNIFORM_BUFFER, bufferId_);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, currentTransforms_.size() * sizeof(glm::mat4), &currentTransforms_[0]);
}

/**
 * Will stream the latest transformation matrices into opengl memory, and will then bind the data to a bind point.
 */
void Animation::bind()
{
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
 * Will set the animation time to runningTime.
 */
void Animation::setAnimationTime(glmd::float32 runningTime)
{
	runningTime_ = runningTime;
}

void Animation::setFrameClampping(glm::detail::uint32 startFrame, glm::detail::uint32 endFrame)
{
	
}

const std::string Animation::getName()
{
	return name_;
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

glmd::uint32 Animation::findPosition(glmd::float32 animationTime, AnimatedBoneNode* animatedBoneNode)
{
	assert(animatedBoneNode->positionTimes_.size() > 0);
	
    for (glmd::uint32 i = 0; i < animatedBoneNode->positionTimes_.size() - 1; i++)
    {
        if (animationTime < (glmd::float32)animatedBoneNode->positionTimes_[i + 1])
        {
            return i;
        }
    }
    
    assert(0);

    return 0;
}

glmd::uint32 Animation::findRotation(glmd::float32 animationTime, AnimatedBoneNode* animatedBoneNode)
{
    assert(animatedBoneNode->rotationTimes_.size() > 0);

    for (glmd::uint32 i = 0; i < animatedBoneNode->rotationTimes_.size() - 1; i++)
    {
        if (animationTime < (glmd::float32)animatedBoneNode->rotationTimes_[i + 1])
        {
            return i;
        }
    }
    
    assert(0);

    return 0;
}


glmd::uint32 Animation::findScaling(glmd::float32 animationTime, AnimatedBoneNode* animatedBoneNode)
{
    assert(animatedBoneNode->scalingTimes_.size() > 0);
    
    for (glmd::uint32 i = 0; i < animatedBoneNode->scalingTimes_.size() - 1; i++)
    {
        if (animationTime < (glmd::float32)animatedBoneNode->scalingTimes_[i + 1])
        {
            return i;
        }
    }
    
    assert(0);

    return 0;
}

void Animation::calcInterpolatedPosition(glm::vec3& out, glmd::float32 animationTime, AnimatedBoneNode* animatedBoneNode)
{
    if (animatedBoneNode->positionTimes_.size() == 1)
    {
        out = animatedBoneNode->positions_[0];
        return;
    }
            
    glmd::uint32 positionIndex = findPosition(animationTime, animatedBoneNode);
    glmd::uint32 nextPositionIndex = (positionIndex + 1);
    
    assert(nextPositionIndex < animatedBoneNode->positionTimes_.size());
    
    glmd::float32 deltaTime = (glmd::float32)(animatedBoneNode->positionTimes_[nextPositionIndex] - animatedBoneNode->positionTimes_[positionIndex]);
    glmd::float32 factor = (animationTime - (glmd::float32)animatedBoneNode->positionTimes_[positionIndex]) / deltaTime;
    
    std::cout << "calc pos: " << positionIndex << " " << nextPositionIndex << " " << deltaTime << " " << factor << std::endl;
    
    //assert(factor >= 0.0f && factor <= 1.0f);
    const glm::vec3& start = animatedBoneNode->positions_[positionIndex];
    const glm::vec3& end = animatedBoneNode->positions_[nextPositionIndex];
    glm::vec3 delta = end - start;
    out = start + factor * delta;
}


void Animation::calcInterpolatedRotation(glm::quat& out, glmd::float32 animationTime, AnimatedBoneNode* animatedBoneNode)
{
	// we need at least two values to interpolate...
    if (animatedBoneNode->rotationTimes_.size() == 1)
    {
        out = animatedBoneNode->rotations_[0];
        //BOOST_LOG_TRIVIAL(debug) << "HERE - rotationTimes_.size() == 1";
        return;
    }
    
    //BOOST_LOG_TRIVIAL(debug) << "HERE - calcInterpolatedRotation";
    glmd::uint32 rotationIndex = findRotation(animationTime, animatedBoneNode);
    glmd::uint32 nextRotationIndex = (rotationIndex + 1);
    
    assert(nextRotationIndex < animatedBoneNode->rotationTimes_.size());
    
    glmd::float32 deltaTime = (glmd::float32)(animatedBoneNode->rotationTimes_[nextRotationIndex] - animatedBoneNode->rotationTimes_[rotationIndex]);
    glmd::float32 factor = (animationTime - (glmd::float32)animatedBoneNode->rotationTimes_[rotationIndex]) / deltaTime;
    //assert(factor >= 0.0f && factor <= 1.0f);
    const glm::quat& startrotationQ = animatedBoneNode->rotations_[rotationIndex];
    const glm::quat& endrotationQ   = animatedBoneNode->rotations_[nextRotationIndex];    
    //BOOST_LOG_TRIVIAL(debug) << "HERE - " << rotationIndex << " startrotationQ: " << startrotationQ.x << ", " << startrotationQ.y << ", " << startrotationQ.z << " factor: " << factor;
    //BOOST_LOG_TRIVIAL(debug) << "HERE - " << nextRotationIndex << " endrotationQ: " << endrotationQ.x << ", " << endrotationQ.y << ", " << endrotationQ.z;
    
    out = glm::slerp(startrotationQ, endrotationQ, factor);
    // TODO: I might not need to normalize the quaternion here...might already have been done in glm::mix??
	out = glm::normalize( out );
	//BOOST_LOG_TRIVIAL(debug) << "HERE - out: " << out.x << ", " << out.y << ", " << out.z << " factor: " << factor;
}


void Animation::calcInterpolatedScaling(glm::vec3& out, glmd::float32 animationTime, AnimatedBoneNode* animatedBoneNode)
{
    if (animatedBoneNode->scalingTimes_.size() == 1)
    {
        out = animatedBoneNode->scalings_[0];
        return;
    }

    glmd::uint32 scalingIndex = findScaling(animationTime, animatedBoneNode);
    glmd::uint32 nextScalingIndex = (scalingIndex + 1);
    
    assert(nextScalingIndex < animatedBoneNode->scalingTimes_.size());
    
    glmd::float32 deltaTime = (glmd::float32)(animatedBoneNode->scalingTimes_[nextScalingIndex] - animatedBoneNode->scalingTimes_[scalingIndex]);
    glmd::float32 factor = (animationTime - (glmd::float32)animatedBoneNode->scalingTimes_[scalingIndex]) / deltaTime;
    //assert(factor >= 0.0f && factor <= 1.0f);
    const glm::vec3& start = animatedBoneNode->scalings_[scalingIndex];
    const glm::vec3& end   = animatedBoneNode->scalings_[nextScalingIndex];
    glm::vec3 delta = end - start;
    out = start + factor * delta;
}

void Animation::readNodeHeirarchy(glmd::float32 animationTime, glm::mat4& globalInverseTransform, BoneNode& rootBoneNode, BoneData& boneData, const glm::mat4& parentTransform)
{
	//BOOST_LOG_TRIVIAL(debug) << "HERE - parentTransform: " << glm::to_string(parentTransform) << " time: " << animationTime;
	
	glm::mat4 nodeTransformation = glm::mat4( rootBoneNode.transformation );
	//BOOST_LOG_TRIVIAL(debug) << "HERE - nodeTransformation1: " << glm::to_string(nodeTransformation) << " time: " << animationTime;
	
	AnimatedBoneNode* animatedBoneNode = nullptr;
	auto it = animatedBoneNodes_.find( rootBoneNode.name );
	
	if ( it != animatedBoneNodes_.end() )
	{
		////BOOST_LOG_TRIVIAL(debug) << "Found animated bone node with name: " << rootBoneNode.name;
		animatedBoneNode = &(it->second);
	}
	else
	{
		// TODO: throw error?
		//BOOST_LOG_TRIVIAL(error) << "Error reading animation - unable to find animated bone node with name: " << rootBoneNode.name;
	}
	
	if (animatedBoneNode != nullptr)
	{
		// Clamp animation time between start and end frame
		if ( startFrame_ > 0 || endFrame_ > 0)
		{
			glmd::float32 st = (glmd::float32)animatedBoneNode->positionTimes_[startFrame_];
			glmd::float32 et = (glmd::float32)animatedBoneNode->positionTimes_[endFrame_];
			
			animationTime = fmod(animationTime, et) + st;
			
			std::cout << "findPosition: " << startFrame_ << ", " << endFrame_ << " | " << st << ", " << et << " | " << animationTime << std::endl;			
		}
		
		// Interpolate scaling and generate scaling transformation matrix
		glm::vec3 Scaling;
		calcInterpolatedScaling(Scaling, animationTime, animatedBoneNode);
		glm::mat4 scalingM = glm::scale( glm::mat4(1.0f), glm::vec3(Scaling.x, Scaling.y, Scaling.z) );
		//BOOST_LOG_TRIVIAL(debug) << "HERE - Scaling: " << glm::to_string(Scaling) << " time: " << animationTime;
		
		// Interpolate rotation and generate rotation transformation matrix
		glm::quat rotationQ;
		calcInterpolatedRotation(rotationQ, animationTime, animatedBoneNode);
		glm::mat4 rotationM = glm::mat4_cast(rotationQ);
		//BOOST_LOG_TRIVIAL(debug) << "HERE - rotationQ: " << rotationQ.x << ", " << rotationQ.y << ", " << rotationQ.z << " time: " << animationTime;
		//BOOST_LOG_TRIVIAL(debug) << "HERE - rotationM: " << glm::to_string(rotationM) << " time: " << animationTime;

		// Interpolate translation and generate translation transformation matrix
		glm::vec3 translation;
		calcInterpolatedPosition(translation, animationTime, animatedBoneNode);
		//BOOST_LOG_TRIVIAL(debug) << "HERE - translation: " << glm::to_string(translation) << " time: " << animationTime;
		glm::mat4 translationM = glm::translate( glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z) );
		
		// Combine the above transformations
		nodeTransformation = translationM * rotationM * scalingM;
		//BOOST_LOG_TRIVIAL(debug) << "HERE - nodeTransformation2: " << glm::to_string(nodeTransformation) << " time: " << animationTime;
	}
	   
	glm::mat4 globalTransformation = parentTransform * nodeTransformation;
	//BOOST_LOG_TRIVIAL(debug) << "HERE - globalTransformation: " << glm::to_string(globalTransformation) << " time: " << animationTime;
	
	if (boneData.boneIndexMap.find( rootBoneNode.name ) != boneData.boneIndexMap.end())
	{
		glmd::uint32 boneIndex = boneData.boneIndexMap[ rootBoneNode.name ];
		boneData.boneTransform[boneIndex].finalTransformation = globalInverseTransform * globalTransformation * boneData.boneTransform[boneIndex].boneOffset;
		//BOOST_LOG_TRIVIAL(debug) << name_ << " " << rootBoneNode.name << ": FOUND";// << glm::to_string( boneData.boneTransform[boneIndex].finalTransformation );
	}
	else
	{
		//BOOST_LOG_TRIVIAL(debug) << name_ << " " << rootBoneNode.name << ": Couldn't find.";
	}
	
	for (glmd::uint32 i = 0; i < rootBoneNode.children.size(); i++) {
		readNodeHeirarchy(animationTime, globalInverseTransform, rootBoneNode.children[i], boneData, globalTransformation);
	}
}

/**
 * Will generate the transformation matrices to be used to animate the model with the given bone data.
 */
void Animation::generateBoneTransforms(glm::mat4& globalInverseTransformation, BoneNode& rootBoneNode, BoneData& boneData, glmd::uint32 startFrame, glmd::uint32 endFrame)
{
	assert(startFrame >= 0);
	assert(endFrame >= startFrame);
	
	glm::mat4 identity = glm::mat4();
	//duration_= 2.9f;
	
	glmd::float32 timeInTicks_ = runningTime_ * ticksPerSecond_;
	glmd::float32 animationTime = fmod(timeInTicks_, (glmd::float32)duration_);
	
	startFrame_ = startFrame;
	endFrame_ = endFrame;
	
	//std::cout << "animationTime: " << animationTime << std::endl;

	//BOOST_LOG_TRIVIAL(debug) << "animationTime: " << animationTime;
	readNodeHeirarchy(animationTime, globalInverseTransformation, rootBoneNode, boneData, identity);
	
	currentTransforms_ = std::vector< glm::mat4 >( boneData.boneTransform.size(), identity );
	//currentTransforms_ = std::vector< glm::mat4 >( 100, identity );
	
	//BOOST_LOG_TRIVIAL(debug) << boneData.boneTransform.size() << " " << runningTime_ << " " << duration_ << " " << name_;
	
	for (glmd::uint32 i = 0; i < boneData.boneTransform.size(); i++) {
		currentTransforms_[i] = boneData.boneTransform[i].finalTransformation;
		
		//if (name_.compare("Walk") == 0)
		//	currentTransforms_[i] = glm::mat4(1.0);
		//BOOST_LOG_TRIVIAL(debug) << "NUM" << i << " " << glm::to_string( currentTransforms_[i] );
	}		
}

// TODO: Testing this for now...I think maybe this isn't a good way to do it???  Not sure
void Animation::generateIdentityBoneTransforms(glmd::uint32 numBones)
{
	glm::mat4 identity = glm::mat4() * 1.0f;
	
	currentTransforms_ = std::vector< glm::mat4 >( numBones, identity );
	
	std::cout << "generateIdentityBoneTransforms: " << currentTransforms_.size() << std::endl;
}

}
}

