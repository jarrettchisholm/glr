#include <sstream>
#include <utility>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "common/logger/Logger.hpp"
#include "common/utilities/Macros.hpp"

#include "glw/Animation.hpp"

#include "glw/Constants.hpp"

#include "exceptions/GlException.hpp"

namespace glr
{
namespace glw
{

Animation::Animation()
{
	openGlDevice_ = nullptr;
	name_ = std::string();
	duration_ = 0.0f;
	ticksPerSecond_ = 0.0f;
	animatedBoneNodes_ = std::map< std::string, AnimatedBoneNode >();
	runningTime_ = 0.0f;
	
	bufferId_ = 0;
	
	startFrame_ = 0;
	endFrame_ = 0;
	
	currentTransforms_ = std::vector< glm::mat4 >();
	
	isLocalDataLoaded_ = false;
	isVideoMemoryAllocated_ = false;
	isDirty_ = false;
	
	loadLocalData();
}

Animation::Animation(IOpenGlDevice* openGlDevice, std::string name, bool initialize) : openGlDevice_(openGlDevice), name_(std::move(name))
{
	duration_ = 0.0f;
	ticksPerSecond_ = 0.0f;
	animatedBoneNodes_ = std::map< std::string, AnimatedBoneNode >();
	runningTime_ = 0.0f;
	
	bufferId_ = 0;
	
	startFrame_ = 0;
	endFrame_ = 0;
	
	currentTransforms_ = std::vector< glm::mat4 >();
	
	isLocalDataLoaded_ = false;
	isVideoMemoryAllocated_ = false;
	isDirty_ = false;
	
	if (initialize)
	{
		loadLocalData();
		allocateVideoMemory();
		pushToVideoMemory();
	}
}

Animation::Animation(
		IOpenGlDevice* openGlDevice,
		std::string name, 
		glm::detail::float64 duration, 
		glm::detail::float64 ticksPerSecond, 
		std::map< std::string, AnimatedBoneNode > animatedBoneNodes,
		bool initialize
	) : openGlDevice_(openGlDevice), name_(std::move(name)), duration_(duration), ticksPerSecond_(ticksPerSecond), animatedBoneNodes_(std::move(animatedBoneNodes)), runningTime_(0.0f)
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
	
	isLocalDataLoaded_ = false;
	isVideoMemoryAllocated_ = false;
	isDirty_ = false;
	
	if (initialize)
	{
		loadLocalData();
		allocateVideoMemory();
		pushToVideoMemory();
	}
}

Animation::Animation(const Animation& other, bool initialize)
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
	
	isLocalDataLoaded_ = false;
	isVideoMemoryAllocated_ = false;
	isDirty_ = false;
	
	if (initialize)
	{
		loadLocalData();
		allocateVideoMemory();
		pushToVideoMemory();
	}
}

Animation::~Animation()
{
	openGlDevice_->releaseBufferObject( bufferId_ );
}

void Animation::bind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, bufferId_);
}

void Animation::pushToVideoMemory()
{
	pushToVideoMemory( currentTransforms_ );
}

void Animation::pullFromVideoMemory()
{
	// TODO: Implement
}

void Animation::loadLocalData()
{
	currentTransforms_ = std::vector< glm::mat4 >( Constants::MAX_NUMBER_OF_BONES_PER_MESH, glm::mat4(1.0f) );
	isLocalDataLoaded_ = true;
	isDirty_ = true;
}

void Animation::freeLocalData()
{
	currentTransforms_ = std::vector< glm::mat4 >( 0 );
	isLocalDataLoaded_ = false;
}

void Animation::freeVideoMemory()
{
	// TODO: Implement
}

void Animation::allocateVideoMemory()
{
	bufferId_ = openGlDevice_->createBufferObject(GL_UNIFORM_BUFFER, Constants::MAX_NUMBER_OF_BONES_PER_MESH * sizeof(glm::mat4), &currentTransforms_[0], GL_STREAM_DRAW);
	
	isVideoMemoryAllocated_ = true;
	
	OPENGL_CHECK_ERRORS(openGlDevice_)
	
	LOG_DEBUG( "Successfully loaded animation.  Buffer id: " << bufferId_ );
}

// TODO: Look at making this more efficient?
// Note: I couldn't just use glBufferSubData, as it didn't seem to synchronize when the previous buffer data was to be used for a draw call.
// Furthermore, it appears 'buffer orphaning' is not working either. That is, when I added 
// `glBufferData(GL_UNIFORM_BUFFER, currentTransforms_.size() * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);` before my call to `glBufferSubData`, 
// I get the same results.
// StackOverflow Question: http://stackoverflow.com/questions/19897461/glbuffersubdata-between-gldrawarrays-calls-mangling-data#19897905
void Animation::pushToVideoMemory(const std::vector< glm::mat4 >& transformations)
{
	assert( transformations.size() <= Constants::MAX_NUMBER_OF_BONES_PER_MESH );

	bind();

	if (transformations.size() > 0)
	{
		void* d = glMapBufferRange(GL_UNIFORM_BUFFER, 0, transformations.size() * sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
		
		if (d != nullptr)
		{
			memcpy( d, &transformations[0], transformations.size() * sizeof(glm::mat4) );
			GLboolean r = glUnmapBuffer(GL_UNIFORM_BUFFER);
			
			if (r == GL_FALSE)
			{
				GlError err = openGlDevice_->getGlError();
				
				std::string msg = std::string("Call to 'glUnmapBuffer' failed with error: ") + err.name;
				LOG_ERROR( msg );
				throw exception::GlException( msg );
				
			}
		}
		else
		{
			GlError err = openGlDevice_->getGlError();
			
			std::string msg = std::string("Call to 'glMapBufferRange' failed with error: ") + err.name;
			LOG_ERROR( msg );
			throw exception::GlException( msg );
		}
	}
	
	isDirty_ = false;
}

bool Animation::isVideoMemoryAllocated() const
{
	return isVideoMemoryAllocated_;
}

bool Animation::isLocalDataLoaded() const
{
	return isLocalDataLoaded_;
}

bool Animation::isDirty() const
{
	return isDirty_;
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

void Animation::setDuration(glm::detail::float64 duration)
{
	duration_ = duration;
}

void Animation::setTicksPerSecond(glm::detail::float64 ticksPerSecond)
{
	ticksPerSecond_ = ticksPerSecond;
}

void Animation::setAnimatedBoneNodes(std::map< std::string, AnimatedBoneNode > animatedBoneNodes)
{
	animatedBoneNodes_ = std::move(animatedBoneNodes);
}

const std::string& Animation::getName() const
{
	return name_;
}

glmd::uint32 Animation::findPosition(glmd::float32 animationTime, const AnimatedBoneNode& animatedBoneNode)
{
	assert(animatedBoneNode.positionTimes.size() > 0);
	assert( indexCache_[0] >= 0 && indexCache_[0] < animatedBoneNode.positionTimes.size() );
	
	// Search first from cache position
	for (glmd::uint32 i = indexCache_[0]; i < animatedBoneNode.positionTimes.size() - 1; i++)
	{
		if (animationTime < (glmd::float32)animatedBoneNode.positionTimes[i + 1])
		{
			indexCache_[0] = i;
			return i;
		}
	}
	
	// If not found, search from the beginning
	for (glmd::uint32 i = 0; i < animatedBoneNode.positionTimes.size() - 1; i++)
	{
		if (animationTime < (glmd::float32)animatedBoneNode.positionTimes[i + 1])
		{
			indexCache_[0] = i;
			return i;
		}
	}

	assert(0);

	return 0;
}

glmd::uint32 Animation::findRotation(glmd::float32 animationTime, const AnimatedBoneNode& animatedBoneNode)
{
	assert(animatedBoneNode.rotationTimes.size() > 0);
	assert( indexCache_[1] >= 0 && indexCache_[1] < animatedBoneNode.rotationTimes.size() );
	
	// Search first from cache position
	for (glmd::uint32 i = indexCache_[1]; i < animatedBoneNode.rotationTimes.size() - 1; i++)
	{
		if (animationTime < (glmd::float32)animatedBoneNode.rotationTimes[i + 1])
		{
			indexCache_[1] = i;
			return i;
		}
	}
	
	// If not found, search from the beginning
	for (glmd::uint32 i = 0; i < animatedBoneNode.rotationTimes.size() - 1; i++)
	{
		if (animationTime < (glmd::float32)animatedBoneNode.rotationTimes[i + 1])
		{
			indexCache_[1] = i;
			return i;
		}
	}
	
	assert(0);

	return 0;
}


glmd::uint32 Animation::findScaling(glmd::float32 animationTime, const AnimatedBoneNode& animatedBoneNode)
{
	assert(animatedBoneNode.scalingTimes.size() > 0);
	assert( indexCache_[2] >= 0 && indexCache_[2] < animatedBoneNode.scalingTimes.size() );
	
	// Search first from cache position
	for (glmd::uint32 i = indexCache_[2]; i < animatedBoneNode.scalingTimes.size() - 1; i++)
	{
		if (animationTime < (glmd::float32)animatedBoneNode.scalingTimes[i + 1])
		{
			indexCache_[2] = i;
			return i;
		}
	}
	
	// If not found, search from the beginning
	for (glmd::uint32 i = 0; i < animatedBoneNode.scalingTimes.size() - 1; i++)
	{
		if (animationTime < (glmd::float32)animatedBoneNode.scalingTimes[i + 1])
		{
			indexCache_[2] = i;
			return i;
		}
	}
	
	assert(0);

	return 0;
}

void Animation::calcInterpolatedPosition(glm::vec3& out, glmd::float32 animationTime, const AnimatedBoneNode& animatedBoneNode)
{
	if (animatedBoneNode.positionTimes.size() == 1)
	{
		out = animatedBoneNode.positions[0];
		return;
	}
			
	glmd::uint32 positionIndex = findPosition(animationTime, animatedBoneNode);
	glmd::uint32 nextPositionIndex = (positionIndex + 1);
	
	assert(nextPositionIndex < animatedBoneNode.positionTimes.size());
	
	glmd::float32 deltaTime = (glmd::float32)(animatedBoneNode.positionTimes[nextPositionIndex] - animatedBoneNode.positionTimes[positionIndex]);
	glmd::float32 factor = (animationTime - (glmd::float32)animatedBoneNode.positionTimes[positionIndex]) / deltaTime;
	
	assert(factor >= 0.0f && factor <= 1.0f);
	const glm::vec3& start = animatedBoneNode.positions[positionIndex];
	const glm::vec3& end = animatedBoneNode.positions[nextPositionIndex];
	glm::vec3 delta = end - start;
	out = start + factor * delta;
}


void Animation::calcInterpolatedRotation(glm::quat& out, glmd::float32 animationTime, const AnimatedBoneNode& animatedBoneNode)
{
	// we need at least two values to interpolate...
	if (animatedBoneNode.rotationTimes.size() == 1)
	{
		out = animatedBoneNode.rotations[0];
		return;
	}

	glmd::uint32 rotationIndex = findRotation(animationTime, animatedBoneNode);
	glmd::uint32 nextRotationIndex = (rotationIndex + 1);
	
	assert(nextRotationIndex < animatedBoneNode.rotationTimes.size());
	
	glmd::float32 deltaTime = (glmd::float32)(animatedBoneNode.rotationTimes[nextRotationIndex] - animatedBoneNode.rotationTimes[rotationIndex]);
	glmd::float32 factor = (animationTime - (glmd::float32)animatedBoneNode.rotationTimes[rotationIndex]) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const glm::quat& startRotationQuat = animatedBoneNode.rotations[rotationIndex];
	const glm::quat& endRotationQuat   = animatedBoneNode.rotations[nextRotationIndex];
	
	out = glm::slerp(startRotationQuat, endRotationQuat, factor);
	// TODO: I might not need to normalize the quaternion here...might already have been done in glm::mix??
	out = glm::normalize( out );
}


void Animation::calcInterpolatedScaling(glm::vec3& out, glmd::float32 animationTime, const AnimatedBoneNode& animatedBoneNode)
{
	if (animatedBoneNode.scalingTimes.size() == 1)
	{
		out = animatedBoneNode.scalings[0];
		return;
	}

	glmd::uint32 scalingIndex = findScaling(animationTime, animatedBoneNode);
	glmd::uint32 nextScalingIndex = (scalingIndex + 1);
	
	assert(nextScalingIndex < animatedBoneNode.scalingTimes.size());
	
	glmd::float32 deltaTime = (glmd::float32)(animatedBoneNode.scalingTimes[nextScalingIndex] - animatedBoneNode.scalingTimes[scalingIndex]);
	glmd::float32 factor = (animationTime - (glmd::float32)animatedBoneNode.scalingTimes[scalingIndex]) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const glm::vec3& start = animatedBoneNode.scalings[scalingIndex];
	const glm::vec3& end   = animatedBoneNode.scalings[nextScalingIndex];
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
			const AnimatedBoneNode& animatedBoneNode = it->second;
			
			// Clamp animation time between start and end frame
			if ( startFrame_ > 0 || endFrame_ > 0)
			{
				glmd::float32 st = (glmd::float32)animatedBoneNode.positionTimes[startFrame_];
				glmd::float32 et = (glmd::float32)animatedBoneNode.positionTimes[endFrame_];

				animationTime = fmod(animationTime, et) + st;
			}
			
			// Interpolate scaling and generate scaling transformation matrix
			glm::vec3 scaling;
			calcInterpolatedScaling(scaling, animationTime, animatedBoneNode);
			glm::mat4 scalingM = glm::scale( glm::mat4(1.0f), glm::vec3(scaling.x, scaling.y, scaling.z) );
			
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
	
	glmd::float32 timeInTicks = runningTime_ * ticksPerSecond_;
	glmd::float32 animationTime = fmod(timeInTicks, (glmd::float32)duration_);
	
	readNodeHeirarchy( transformations, animationTime, globalInverseTransformation, rootBoneNode, boneData, glm::mat4() );
}

void Animation::serialize(const std::string& filename)
{
	std::ofstream ofs(filename.c_str());
	serialize::TextOutArchive textOutArchive(ofs);
	serialize(textOutArchive);
}

void Animation::serialize(serialize::TextOutArchive& outArchive)
{
	outArchive << *this;
}

void Animation::deserialize(const std::string& filename)
{
	std::ifstream ifs(filename.c_str());
	serialize::TextInArchive textInArchive(ifs);
	deserialize(textInArchive);
}

void Animation::deserialize(serialize::TextInArchive& inArchive)
{
	inArchive >> *this;
	loadLocalData();
}

/*
template<class Archive> void Animation::serialize(Archive& ar, const unsigned int version)
{
	boost::serialization::void_cast_register<Animation, IAnimation>(
		static_cast<Animation*>(nullptr),
		static_cast<IAnimation*>(nullptr)
	);

	ar & name_;
	ar & duration_;
	ar & ticksPerSecond_;
	ar & animatedBoneNodes_;
}
*/

}
}

BOOST_CLASS_EXPORT(glr::glw::IAnimation)
BOOST_CLASS_EXPORT_GUID(glr::glw::Animation, "glr::glw::Animation")
