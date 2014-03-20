#include <sstream>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "glw/Animation.hpp"

#include "glw/Constants.hpp"

#include "exceptions/GlException.hpp"

namespace glr
{
namespace glw
{

Animation::Animation(IOpenGlDevice* openGlDevice, std::string name) : openGlDevice_(openGlDevice), name_(std::move(name))
{
	duration_ = 0.0f;
	ticksPerSecond_ = 0.0f;
	animatedBoneNodes_ = std::map< std::string, AnimatedBoneNode >();
	runningTime_ = 0.0f;
	
	bufferId_ = 0;
	
	startFrame_ = 0;
	endFrame_ = 0;
	
	currentTransforms_ = std::vector< glm::mat4 >( Constants::MAX_NUMBER_OF_BONES_PER_MESH, glm::mat4() );
	
	setupAnimationUbo();
}

Animation::Animation(
		IOpenGlDevice* openGlDevice,
		std::string name, 
		glm::detail::float64 duration, 
		glm::detail::float64 ticksPerSecond, 
		std::map< std::string, AnimatedBoneNode > animatedBoneNodes
	) : openGlDevice_(openGlDevice), name_(std::move(name)), duration_(duration), ticksPerSecond_(ticksPerSecond), animatedBoneNodes_(animatedBoneNodes), runningTime_(0.0f)
{
	// We probably shouldn't have an animation object at all if it has no animated bone nodes...
	assert( animatedBoneNodes_.size() != 0 );
	
	//LOG_DEBUG( "loading animation." );
	bufferId_ = 0;
	
	startFrame_ = 0;
	endFrame_ = 0;
	
	// Error check - default to 25 ticks per second
	ticksPerSecond_ = ( ticksPerSecond_ != 0.0f ? ticksPerSecond_ : 25.0f );
	
	currentTransforms_ = std::vector< glm::mat4 >( Constants::MAX_NUMBER_OF_BONES_PER_MESH, glm::mat4() );
	
	setupAnimationUbo();
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
	
	currentTransforms_ = std::vector< glm::mat4 >( Constants::MAX_NUMBER_OF_BONES_PER_MESH, glm::mat4() );
	
	setupAnimationUbo();
}

Animation::~Animation()
{
	openGlDevice_->releaseBufferObject( bufferId_ );
}

void Animation::setupAnimationUbo()
{
	bufferId_ = openGlDevice_->createBufferObject(GL_UNIFORM_BUFFER, Constants::MAX_NUMBER_OF_BONES_PER_MESH * sizeof(glm::mat4), &currentTransforms_[0]);
	
	GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{
		// Cleanup
		openGlDevice_->releaseBufferObject( bufferId_ );
		
		std::string msg = std::string("Error while loading animation '") + name_ + std::string("' in OpenGl: ") + err.name;
		LOG_ERROR( msg );
		throw exception::GlException( msg );
	}
	else
	{
		std::stringstream ss;
		ss << "Successfully loaded animation.  Buffer id: " << bufferId_;
		LOG_DEBUG( ss.str() );
	}
}

// TODO: Look at making this more efficient?
// Note: I couldn't just use glBufferSubData, as it didn't seem to synchronize when the previous buffer data was to be used for a draw call.
// Furthermore, it appears 'buffer orphaning' is not working either. That is, when I added 
// `glBufferData(GL_UNIFORM_BUFFER, currentTransforms_.size() * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);` before my call to `glBufferSubData`, 
// I get the same results.
// StackOverflow Question: http://stackoverflow.com/questions/19897461/glbuffersubdata-between-gldrawarrays-calls-mangling-data#19897905
void Animation::loadIntoVideoMemory()
{
	loadIntoVideoMemory( currentTransforms_ );
}

void Animation::loadIntoVideoMemory(const std::vector< glm::mat4 >& transformations)
{
	glBindBuffer(GL_UNIFORM_BUFFER, bufferId_);
	//glBufferData(GL_UNIFORM_BUFFER, currentTransforms_.size() * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, currentTransforms_.size() * sizeof(glm::mat4), &currentTransforms_[0]);
	if (currentTransforms_.size() > 0)
	{
		void* d = glMapBufferRange(GL_UNIFORM_BUFFER, 0, currentTransforms_.size() * sizeof(glm::mat4), GL_MAP_WRITE_BIT);
		
		if (d != nullptr)
		{
			memcpy( d, &currentTransforms_[0], currentTransforms_.size() * sizeof(glm::mat4) );
			GLboolean r = glUnmapBuffer(GL_UNIFORM_BUFFER);
			
			if (r == GL_FALSE)
			{
				GlError err = openGlDevice_->getGlError();
				
				// Cleanup
				//glBindBuffer(0);
				
				std::string msg = std::string("Call to 'glUnmapBuffer' failed with error: ") + err.name;
				LOG_ERROR( msg );
				throw exception::GlException( msg );
				
			}
		}
		else
		{
			GlError err = openGlDevice_->getGlError();
				
			// Cleanup
			//glBindBuffer(0);
			
			std::string msg = std::string("Call to 'glMapBufferRange' failed with error: ") + err.name;
			LOG_ERROR( msg );
			throw exception::GlException( msg );
		}
	}
}

void Animation::bind()
{
	loadIntoVideoMemory();
}

void Animation::bind(const std::vector< glm::mat4 >& transformations)
{
	loadIntoVideoMemory(transformations);
}

GLuint Animation::getBufferId() const
{
	return bufferId_;
}

GLuint Animation::getBindPoint() const
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

const std::string& Animation::getName() const
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
		return;
	}

	glmd::uint32 rotationIndex = findRotation(animationTime, animatedBoneNode);
	glmd::uint32 nextRotationIndex = (rotationIndex + 1);
	
	assert(nextRotationIndex < animatedBoneNode->rotationTimes_.size());
	
	glmd::float32 deltaTime = (glmd::float32)(animatedBoneNode->rotationTimes_[nextRotationIndex] - animatedBoneNode->rotationTimes_[rotationIndex]);
	glmd::float32 factor = (animationTime - (glmd::float32)animatedBoneNode->rotationTimes_[rotationIndex]) / deltaTime;
	//assert(factor >= 0.0f && factor <= 1.0f);
	const glm::quat& startRotationQuat = animatedBoneNode->rotations_[rotationIndex];
	const glm::quat& endRotationQuat   = animatedBoneNode->rotations_[nextRotationIndex];
	
	out = glm::slerp(startRotationQuat, endRotationQuat, factor);
	// TODO: I might not need to normalize the quaternion here...might already have been done in glm::mix??
	out = glm::normalize( out );
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

void Animation::readNodeHeirarchy(std::vector< glm::mat4 >& transformations, glmd::float32 animationTime, const glm::mat4& globalInverseTransform, const BoneNode& rootBoneNode, const BoneData& boneData, const glm::mat4& parentTransform)
{
	glm::mat4 nodeTransformation = glm::mat4( rootBoneNode.transformation );
	
	{
		auto it = animatedBoneNodes_.find( rootBoneNode.name );
		
		if ( it != animatedBoneNodes_.end() )
		{
			AnimatedBoneNode* animatedBoneNode = &(it->second);
			
			// Clamp animation time between start and end frame
			if ( startFrame_ > 0 || endFrame_ > 0)
			{
				glmd::float32 st = (glmd::float32)animatedBoneNode->positionTimes_[startFrame_];
				glmd::float32 et = (glmd::float32)animatedBoneNode->positionTimes_[endFrame_];
				
				animationTime = fmod(animationTime, et) + st;
			}
			
			// Interpolate scaling and generate scaling transformation matrix
			glm::vec3 Scaling;
			calcInterpolatedScaling(Scaling, animationTime, animatedBoneNode);
			glm::mat4 scalingM = glm::scale( glm::mat4(1.0f), glm::vec3(Scaling.x, Scaling.y, Scaling.z) );
			
			// Interpolate rotation and generate rotation transformation matrix
			glm::quat rotationQ;
			calcInterpolatedRotation(rotationQ, animationTime, animatedBoneNode);
			glm::mat4 rotationM = glm::mat4_cast(rotationQ);
	
			// Interpolate translation and generate translation transformation matrix
			glm::vec3 translation;
			calcInterpolatedPosition(translation, animationTime, animatedBoneNode);
			glm::mat4 translationM = glm::translate( glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z) );
			
			// Combine the above transformations
			nodeTransformation = translationM * rotationM * scalingM;
		}
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransformation;
	
	{
		auto it = boneData.boneIndexMap.find( rootBoneNode.name );
		
		if ( it != boneData.boneIndexMap.end() )
		{
			glmd::uint32 boneIndex = it->second;
			transformations[boneIndex] = globalInverseTransform * globalTransformation * boneData.boneTransform[boneIndex].boneOffset;
		}
	}
	
	for (glmd::uint32 i = 0; i < rootBoneNode.children.size(); i++)
	{
		readNodeHeirarchy(transformations, animationTime, globalInverseTransform, rootBoneNode.children[i], boneData, globalTransformation);
	}
}

void Animation::calculate(const glm::mat4& globalInverseTransformation, const BoneNode& rootBoneNode, const BoneData& boneData)
{
	std::vector<glmd::uint32> indexCache = std::vector<glmd::uint32>( 3 );
	calculate( globalInverseTransformation, rootBoneNode, boneData, indexCache);
}

void Animation::calculate(const glm::mat4& globalInverseTransformation, const BoneNode& rootBoneNode, const BoneData& boneData, std::vector<glmd::uint32>& indexCache)
{
	currentTransforms_ = std::vector< glm::mat4 >( boneData.boneTransform.size(), glm::mat4() );

	calculate( currentTransforms_, globalInverseTransformation, rootBoneNode, boneData, indexCache);
}

void Animation::calculate(std::vector< glm::mat4 >& transformations, const glm::mat4& globalInverseTransformation, const BoneNode& rootBoneNode, const BoneData& boneData)
{
	std::vector<glmd::uint32> indexCache = std::vector<glmd::uint32>( 3 );
	calculate( transformations, globalInverseTransformation, rootBoneNode, boneData, indexCache );
}

void Animation::calculate(std::vector< glm::mat4 >& transformations, const glm::mat4& globalInverseTransformation, const BoneNode& rootBoneNode, const BoneData& boneData, std::vector<glmd::uint32>& indexCache)
{
	assert(transformations.size() >= boneData.boneTransform.size());
	assert( indexCache.size() == 3 );

	indexCache_ = indexCache;
	
	glmd::float32 timeInTicks_ = runningTime_ * ticksPerSecond_;
	glmd::float32 animationTime = fmod(timeInTicks_, (glmd::float32)duration_);
	
	readNodeHeirarchy( transformations, animationTime, globalInverseTransformation, rootBoneNode, boneData, glm::mat4() );
}

// TODO: Testing this for now...I think maybe this isn't a good way to do it???  Not sure
void Animation::generateIdentityBoneTransforms(glmd::uint32 numBones)
{
	glm::mat4 identity = glm::mat4();
	
	currentTransforms_ = std::vector< glm::mat4 >( numBones, identity );
	
	LOG_DEBUG("generateIdentityBoneTransforms: " << currentTransforms_.size() );
}

}
}
