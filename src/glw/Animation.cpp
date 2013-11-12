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

Animation::Animation(IOpenGlDevice* openGlDevice, const std::string name) : openGlDevice_(openGlDevice), name_(name)
{
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
		LOG_ERROR( "Error loading animation in opengl" );
		LOG_ERROR( "OpenGL error: " + err.name );
	}
	else
	{
		//std::stringstream msg;
		//msg << "Successfully loaded animation.  Buffer id: " << bufferId_;
		//LOG_DEBUG( msg.str() );
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
	
	//LOG_DEBUG( "loading animation." );
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
		LOG_ERROR( "Error loading animation in opengl" );
		LOG_ERROR( "OpenGL error: " + err.name );
	}
	else
	{
		//std::stringstream msg;
		//msg << "Successfully loaded animation.  Buffer id: " << bufferId_;
		//LOG_DEBUG( msg.str() );
	}
}

Animation::Animation(const Animation& other)
{
	bufferId_ = 0;
	
	startFrame_ = 0;
	endFrame_ = 0;
	
	runningTime_ = 0.0f;
	
	ticksPerSecond_ = other.ticksPerSecond_;
	openGlDevice_ = other.openGlDevice_;
	name_ = other.name_;
	duration_ = other.duration_;
	animatedBoneNodes_ = other.animatedBoneNodes_;
	
	currentTransforms_ = std::vector< glm::mat4 >();
	
	setupAnimationUbo();
	
	GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{
		// TODO: throw error
		LOG_ERROR( "Error loading animation in opengl" );
		LOG_ERROR( "OpenGL error: " + err.name );
	}
	else
	{
		//std::stringstream msg;
		//msg << "Successfully loaded animation.  Buffer id: " << bufferId_;
		//LOG_DEBUG( msg.str() );
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

// TODO: Look at making this more efficient?
// Note: I couldn't just use glBufferSubData, as it didn't seem to synchronize when the previous buffer data was to be used for a draw call.
// Furthermore, it appears 'buffer orphaning' is not working either. That is, when I added 
// `glBufferData(GL_UNIFORM_BUFFER, currentTransforms_.size() * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);` before my call to `glBufferSubData`, 
// I get the same results.
// StackOverflow Question: http://stackoverflow.com/questions/19897461/glbuffersubdata-between-gldrawarrays-calls-mangling-data#19897905
void Animation::loadIntoVideoMemory()
{
	glBindBuffer(GL_UNIFORM_BUFFER, bufferId_);
	//glBufferData(GL_UNIFORM_BUFFER, currentTransforms_.size() * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, currentTransforms_.size() * sizeof(glm::mat4), &currentTransforms_[0]);
	void* d = glMapBufferRange(GL_UNIFORM_BUFFER, 0, currentTransforms_.size() * sizeof(glm::mat4), GL_MAP_WRITE_BIT);
	if (d != nullptr)
	{
		memcpy( d, &currentTransforms_[0], currentTransforms_.size() * sizeof(glm::mat4) );
		GLboolean r = glUnmapBuffer(GL_UNIFORM_BUFFER);
		
		if (r == GL_FALSE)
		{
			// Throw exception?
			GlError err = openGlDevice_->getGlError();
			LOG_ERROR( "Call to 'glUnmapBuffer' failed." );
			if (err.type != GL_NONE)
			{
				LOG_ERROR( "OpenGL error: " + err.name );
			}
			assert(0);
		}
	}
	else
	{
		// Throw exception?
		GlError err = openGlDevice_->getGlError();
		LOG_ERROR( "Call to 'glMapBufferRange' failed." );
		if (err.type != GL_NONE)
		{
			LOG_ERROR( "OpenGL error: " + err.name );
		}
		assert(0);
	}
}

void Animation::loadIntoVideoMemory(std::vector< glm::mat4 >& transformations)
{
	glBindBuffer(GL_UNIFORM_BUFFER, bufferId_);
	//glBufferData(GL_UNIFORM_BUFFER, transformations.size() * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, transformations.size() * sizeof(glm::mat4), &transformations[0]);
	void* d = glMapBufferRange(GL_UNIFORM_BUFFER, 0, transformations.size() * sizeof(glm::mat4), GL_MAP_WRITE_BIT);
	if (d != nullptr)
	{
		memcpy( d, &transformations[0], transformations.size() * sizeof(glm::mat4) );
		GLboolean r = glUnmapBuffer(GL_UNIFORM_BUFFER);
		
		if (r == GL_FALSE)
		{
			// Throw exception?
			GlError err = openGlDevice_->getGlError();
			LOG_ERROR( "Call to 'glUnmapBuffer' failed." );
			if (err.type != GL_NONE)
			{
				LOG_ERROR( "OpenGL error: " + err.name );
			}
			assert(0);
		}
	}
	else
	{
		// Throw exception?
		GlError err = openGlDevice_->getGlError();
		LOG_ERROR( "Call to 'glMapBufferRange' failed." );
		if (err.type != GL_NONE)
		{
			LOG_ERROR( "OpenGL error: " + err.name );
		}
		assert(0);
	}
}

/**
 * Will stream the latest transformation matrices into opengl memory, and will then bind the data to a bind point.
 */
void Animation::bind()
{
	loadIntoVideoMemory();
	
	//bindPoint_ = openGlDevice_->bindBuffer( bufferId_ );
	
	//std::cout << "animation: " << name_ << " | " << bufferId_ << " | " << bindPoint_ << std::endl;
}

void Animation::bind(std::vector< glm::mat4 >& transformations)
{
	loadIntoVideoMemory(transformations);

	//bindPoint_ = openGlDevice_->bindBuffer( bufferId_ );
}

GLuint Animation::getBufferId()
{
	return bufferId_;
}

GLuint Animation::getBindPoint()
{
	return bindPoint_;
}

void Animation::setAnimationTime(glmd::float32 runningTime)
{
	runningTime_ = runningTime;
}

void Animation::setFrameClampping(glm::detail::uint32 startFrame, glm::detail::uint32 endFrame)
{
	assert(startFrame >= 0);
	assert(endFrame >= startFrame);
	
	startFrame_ = startFrame;
	endFrame_ = endFrame;
}

const std::string& Animation::getName()
{
	return name_;
}

glmd::uint32 Animation::findPosition(glmd::float32 animationTime, AnimatedBoneNode* animatedBoneNode)
{
	assert(animatedBoneNode->positionTimes_.size() > 0);
	assert( indexCache_[0] >= 0 && indexCache_[0] < animatedBoneNode->positionTimes_.size() );
	
	// Search first from cache position
	for (glmd::uint32 i = indexCache_[0]; i < animatedBoneNode->positionTimes_.size() - 1; i++)
	{
		if (animationTime < (glmd::float32)animatedBoneNode->positionTimes_[i + 1])
		{
			indexCache_[0] = i;
			return i;
		}
	}
	
	// If not found, search from the beginning
	for (glmd::uint32 i = 0; i < animatedBoneNode->positionTimes_.size() - 1; i++)
	{
		if (animationTime < (glmd::float32)animatedBoneNode->positionTimes_[i + 1])
		{
			indexCache_[0] = i;
			return i;
		}
	}
	
	assert(0);

	return 0;
}

glmd::uint32 Animation::findRotation(glmd::float32 animationTime, AnimatedBoneNode* animatedBoneNode)
{
	assert(animatedBoneNode->rotationTimes_.size() > 0);
	assert( indexCache_[1] >= 0 && indexCache_[1] < animatedBoneNode->rotationTimes_.size() );
	
	// Search first from cache position
	for (glmd::uint32 i = indexCache_[1]; i < animatedBoneNode->rotationTimes_.size() - 1; i++)
	{
		if (animationTime < (glmd::float32)animatedBoneNode->rotationTimes_[i + 1])
		{
			indexCache_[1] = i;
			return i;
		}
	}
	
	// If not found, search from the beginning
	for (glmd::uint32 i = 0; i < animatedBoneNode->rotationTimes_.size() - 1; i++)
	{
		if (animationTime < (glmd::float32)animatedBoneNode->rotationTimes_[i + 1])
		{
			indexCache_[1] = i;
			return i;
		}
	}
	
	assert(0);

	return 0;
}


glmd::uint32 Animation::findScaling(glmd::float32 animationTime, AnimatedBoneNode* animatedBoneNode)
{
	assert(animatedBoneNode->scalingTimes_.size() > 0);
	assert( indexCache_[2] >= 0 && indexCache_[2] < animatedBoneNode->scalingTimes_.size() );
	
	// Search first from cache position
	for (glmd::uint32 i = indexCache_[2]; i < animatedBoneNode->scalingTimes_.size() - 1; i++)
	{
		if (animationTime < (glmd::float32)animatedBoneNode->scalingTimes_[i + 1])
		{
			indexCache_[2] = i;
			return i;
		}
	}
	
	// If not found, search from the beginning
	for (glmd::uint32 i = 0; i < animatedBoneNode->scalingTimes_.size() - 1; i++)
	{
		if (animationTime < (glmd::float32)animatedBoneNode->scalingTimes_[i + 1])
		{
			indexCache_[2] = i;
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
	
	//std::cout << "calc pos: " << positionIndex << " " << nextPositionIndex << " " << deltaTime << " " << factor << std::endl;
	
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
		//std::cout << "HERE - rotationTimes_.size() == 1";
		return;
	}
	
	//std::cout << "HERE - calcInterpolatedRotation";
	glmd::uint32 rotationIndex = findRotation(animationTime, animatedBoneNode);
	glmd::uint32 nextRotationIndex = (rotationIndex + 1);
	
	assert(nextRotationIndex < animatedBoneNode->rotationTimes_.size());
	
	glmd::float32 deltaTime = (glmd::float32)(animatedBoneNode->rotationTimes_[nextRotationIndex] - animatedBoneNode->rotationTimes_[rotationIndex]);
	glmd::float32 factor = (animationTime - (glmd::float32)animatedBoneNode->rotationTimes_[rotationIndex]) / deltaTime;
	//assert(factor >= 0.0f && factor <= 1.0f);
	const glm::quat& startRotationQuat = animatedBoneNode->rotations_[rotationIndex];
	const glm::quat& endRotationQuat   = animatedBoneNode->rotations_[nextRotationIndex];	
	//std::cout << "HERE - " << rotationIndex << " startRotationQuat: " << startRotationQuat.x << ", " << startRotationQuat.y << ", " << startRotationQuat.z << " factor: " << factor;
	//std::cout << "HERE - " << nextRotationIndex << " endRotationQuat: " << endRotationQuat.x << ", " << endRotationQuat.y << ", " << endRotationQuat.z;
	
	out = glm::slerp(startRotationQuat, endRotationQuat, factor);
	// TODO: I might not need to normalize the quaternion here...might already have been done in glm::mix??
	out = glm::normalize( out );
	//std::cout << "HERE - out: " << out.x << ", " << out.y << ", " << out.z << " factor: " << factor;
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
	//std::cout << "HERE - parentTransform: " << glm::to_string(parentTransform) << " time: " << animationTime;
	
	glm::mat4 nodeTransformation = glm::mat4( rootBoneNode.transformation );
	//std::cout << "HERE - nodeTransformation1: " << glm::to_string(nodeTransformation) << " time: " << animationTime;
	
	
	{
		auto it = animatedBoneNodes_.find( rootBoneNode.name );
		
		if ( it != animatedBoneNodes_.end() )
		{
			////std::cout << "Found animated bone node with name: " << rootBoneNode.name;
			AnimatedBoneNode* animatedBoneNode = &(it->second);
			
			// Clamp animation time between start and end frame
			if ( startFrame_ > 0 || endFrame_ > 0)
			{
				glmd::float32 st = (glmd::float32)animatedBoneNode->positionTimes_[startFrame_];
				glmd::float32 et = (glmd::float32)animatedBoneNode->positionTimes_[endFrame_];
				
				animationTime = fmod(animationTime, et) + st;
				
				//std::cout << "findPosition: " << startFrame_ << ", " << endFrame_ << " | " << st << ", " << et << " | " << animationTime << std::endl;			
			}
			
			// Interpolate scaling and generate scaling transformation matrix
			glm::vec3 Scaling;
			calcInterpolatedScaling(Scaling, animationTime, animatedBoneNode);
			glm::mat4 scalingM = glm::scale( glm::mat4(1.0f), glm::vec3(Scaling.x, Scaling.y, Scaling.z) );
			//std::cout << "HERE - Scaling: " << glm::to_string(Scaling) << " time: " << animationTime;
			
			// Interpolate rotation and generate rotation transformation matrix
			glm::quat rotationQ;
			calcInterpolatedRotation(rotationQ, animationTime, animatedBoneNode);
			glm::mat4 rotationM = glm::mat4_cast(rotationQ);
			//std::cout << "HERE - rotationQ: " << rotationQ.x << ", " << rotationQ.y << ", " << rotationQ.z << " time: " << animationTime;
			//std::cout << "HERE - rotationM: " << glm::to_string(rotationM) << " time: " << animationTime;
	
			// Interpolate translation and generate translation transformation matrix
			glm::vec3 translation;
			calcInterpolatedPosition(translation, animationTime, animatedBoneNode);
			//std::cout << "HERE - translation: " << glm::to_string(translation) << " time: " << animationTime;
			glm::mat4 translationM = glm::translate( glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z) );
			
			// Combine the above transformations
			nodeTransformation = translationM * rotationM * scalingM;
			//std::cout << "HERE - nodeTransformation2: " << glm::to_string(nodeTransformation) << " time: " << animationTime;
		}
		//else
		//{
			// TODO: throw error?
			//LOG_ERROR( "Error reading animation - unable to find animated bone node with name: " + rootBoneNode.name );
		//}
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransformation;
	//std::cout << "HERE - globalTransformation: " << glm::to_string(globalTransformation) << " time: " << animationTime;
	
	{
		auto it = boneData.boneIndexMap.find( rootBoneNode.name );
		
		if ( it != boneData.boneIndexMap.end() )
		{
			glmd::uint32 boneIndex = it->second;
			boneData.boneTransform[boneIndex].finalTransformation = globalInverseTransform * globalTransformation * boneData.boneTransform[boneIndex].boneOffset;
			//std::cout << name_ << " " << rootBoneNode.name << ": FOUND";// << glm::to_string( boneData.boneTransform[boneIndex].finalTransformation );
		}
		//else
		//{
			//std::cout << name_ << " " << rootBoneNode.name << ": Couldn't find.";
		//}
	}
	
	for (glmd::uint32 i = 0; i < rootBoneNode.children.size(); i++) {
		readNodeHeirarchy(animationTime, globalInverseTransform, rootBoneNode.children[i], boneData, globalTransformation);
	}
}

void Animation::generateBoneTransforms(glm::mat4& globalInverseTransformation, BoneNode& rootBoneNode, BoneData& boneData)
{
	std::vector<glmd::uint32> indexCache = std::vector<glmd::uint32>( 3 );
	generateBoneTransforms( globalInverseTransformation, rootBoneNode, boneData, indexCache);
}

void Animation::generateBoneTransforms(glm::mat4& globalInverseTransformation, BoneNode& rootBoneNode, BoneData& boneData, std::vector<glmd::uint32>& indexCache)
{
	indexCache_ = indexCache;
	
	assert( indexCache_.size() == 3 );
	
	glm::mat4 identity = glm::mat4();
	
	glmd::float32 timeInTicks_ = runningTime_ * ticksPerSecond_;
	glmd::float32 animationTime = fmod(timeInTicks_, (glmd::float32)duration_);
	
	//std::cout << "animationTime: " << animationTime << std::endl;

	currentTransforms_ = std::vector< glm::mat4 >( boneData.boneTransform.size(), identity );

	readNodeHeirarchy(animationTime, globalInverseTransformation, rootBoneNode, boneData, identity);
	//currentTransforms_ = std::vector< glm::mat4 >( 100, identity );
	
	//std::cout << boneData.boneTransform.size() << " " << runningTime_ << " " << duration_ << " " << name_;
	//std::cout << " | " << indexCache_[0] << ", " << indexCache_[1] << ", " << indexCache_[2] << " | " << startFrame_ << ", " << endFrame_ << std::endl;
	
	for (glmd::uint32 i = 0; i < boneData.boneTransform.size(); i++) {
		currentTransforms_[i] = boneData.boneTransform[i].finalTransformation;
	}		
}

// TODO: Testing this for now...I think maybe this isn't a good way to do it???  Not sure
void Animation::generateIdentityBoneTransforms(glmd::uint32 numBones)
{
	glm::mat4 identity = glm::mat4();
	
	currentTransforms_ = std::vector< glm::mat4 >( numBones, identity );
	
	std::cout << "generateIdentityBoneTransforms: " << currentTransforms_.size() << std::endl;
}

}
}

