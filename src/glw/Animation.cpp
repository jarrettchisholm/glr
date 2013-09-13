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
	// TODO: implement
	glBindBuffer(GL_UNIFORM_BUFFER, bufferId_);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, currentTransforms_.size() * sizeof(glm::mat4), &currentTransforms_[0]);
	
	//GLuint bindPoint = openGlDevice_->bindBuffer( ubo );
	
	//shader->bindVariableByBindingName(shaders::IShader::BIND_TYPE_LIGHT, bindPoint);
}

/**
 * Will stream the latest transformation matrices into opengl memory, and will then bind the data to a bind point.
 */
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
 * Will set the animation time to runningTime.
 */
void Animation::setAnimationTime(glmd::float32 runningTime)
{
	runningTime_ = runningTime;
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
    
    ////BOOST_LOG_TRIVIAL(debug) << "calc pos: " << PositionIndex << " " << NextPositionIndex << " " << DeltaTime << " " << Factor;
    
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
        //BOOST_LOG_TRIVIAL(debug) << "HERE - rotationTimes_.size() == 1";
        return;
    }
    
    //BOOST_LOG_TRIVIAL(debug) << "HERE - calcInterpolatedRotation";
    glmd::uint32 RotationIndex = findRotation(animationTime, animatedBoneNode);
    glmd::uint32 NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < animatedBoneNode->rotationTimes_.size());
    float DeltaTime = (float)(animatedBoneNode->rotationTimes_[NextRotationIndex] - animatedBoneNode->rotationTimes_[RotationIndex]);
    float Factor = (animationTime - (float)animatedBoneNode->rotationTimes_[RotationIndex]) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::quat& StartRotationQ = animatedBoneNode->rotations_[RotationIndex];
    const glm::quat& EndRotationQ   = animatedBoneNode->rotations_[NextRotationIndex];    
    //BOOST_LOG_TRIVIAL(debug) << "HERE - " << RotationIndex << " StartRotationQ: " << StartRotationQ.x << ", " << StartRotationQ.y << ", " << StartRotationQ.z << " Factor: " << Factor;
    //BOOST_LOG_TRIVIAL(debug) << "HERE - " << NextRotationIndex << " EndRotationQ: " << EndRotationQ.x << ", " << EndRotationQ.y << ", " << EndRotationQ.z;
    
    Out = glm::slerp(StartRotationQ, EndRotationQ, Factor);
    // TODO: I might not need to normalize the quaternion here...might already have been done in glm::mix??
	Out = glm::normalize( Out );
	//BOOST_LOG_TRIVIAL(debug) << "HERE - Out: " << Out.x << ", " << Out.y << ", " << Out.z << " Factor: " << Factor;
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
	//BOOST_LOG_TRIVIAL(debug) << "HERE - parentTransform: " << glm::to_string(parentTransform) << " time: " << animationTime;
	
	glm::mat4 nodeTransformation = glm::mat4( rootBoneNode.transformation );
	//BOOST_LOG_TRIVIAL(debug) << "HERE - nodeTransformation1: " << glm::to_string(nodeTransformation) << " time: " << animationTime;
	
	AnimatedBoneNode* animatedBoneNode = nullptr;
	
	if ( animatedBoneNodes_.find( rootBoneNode.name ) != animatedBoneNodes_.end() )
	{
		////BOOST_LOG_TRIVIAL(debug) << "Found animated bone node with name: " << rootBoneNode.name;
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
		//BOOST_LOG_TRIVIAL(debug) << "HERE - Scaling: " << glm::to_string(Scaling) << " time: " << animationTime;
		
		// Interpolate rotation and generate rotation transformation matrix
		glm::quat RotationQ;
		calcInterpolatedRotation(RotationQ, animationTime, animatedBoneNode);
		glm::mat4 RotationM = glm::mat4_cast(RotationQ);
		//BOOST_LOG_TRIVIAL(debug) << "HERE - RotationQ: " << RotationQ.x << ", " << RotationQ.y << ", " << RotationQ.z << " time: " << animationTime;
		//BOOST_LOG_TRIVIAL(debug) << "HERE - RotationM: " << glm::to_string(RotationM) << " time: " << animationTime;

		// Interpolate translation and generate translation transformation matrix
		glm::vec3 Translation;
		calcInterpolatedPosition(Translation, animationTime, animatedBoneNode);
		//BOOST_LOG_TRIVIAL(debug) << "HERE - Translation: " << glm::to_string(Translation) << " time: " << animationTime;
		glm::mat4 TranslationM = glm::translate( glm::mat4(1.0f), glm::vec3(Translation.x, Translation.y, Translation.z) );
		
		// Combine the above transformations
		nodeTransformation = TranslationM * RotationM * ScalingM;
		//BOOST_LOG_TRIVIAL(debug) << "HERE - nodeTransformation2: " << glm::to_string(nodeTransformation) << " time: " << animationTime;
	}
	   
	glm::mat4 globalTransformation = parentTransform * nodeTransformation;
	//BOOST_LOG_TRIVIAL(debug) << "HERE - globalTransformation: " << glm::to_string(globalTransformation) << " time: " << animationTime;
	
	if (boneData.boneIndexMap.find( rootBoneNode.name ) != boneData.boneIndexMap.end()) {
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
void Animation::generateBoneTransforms(glm::mat4& globalInverseTransformation, BoneNode& rootBoneNode, BoneData& boneData)
{
	
	glm::mat4 identity = glm::mat4();
	
	//BOOST_LOG_TRIVIAL(debug) << runningTime_ << " " << duration_ << " " << name_;
	//duration_= 2.9f;
	
	glmd::float32 timeInTicks_ = runningTime_ * ticksPerSecond_;
	glmd::float32 animationTime = fmod(timeInTicks_, (glmd::float32)duration_);

	//BOOST_LOG_TRIVIAL(debug) << "animationTime: " << animationTime;
	readNodeHeirarchy(animationTime, globalInverseTransformation, rootBoneNode, boneData, identity);
	
	currentTransforms_ = std::vector< glm::mat4 >( boneData.boneTransform.size() );
	//currentTransforms_ = std::vector< glm::mat4 >( 100, identity );
	
	for (glmd::uint32 i = 0; i < boneData.boneTransform.size(); i++) {
		currentTransforms_[i] = boneData.boneTransform[i].finalTransformation;
		
		//if (name_.compare("Walk") != 0)
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

