

#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"

// A position or scale key-frame
struct PositionKeyFrame
{
	glm::detail::double time;
	glm::vec3 v;
};

// A rotation key-frame
struct RotationKeyFrame
{
	glm::detail::double time;
	glm::quat q; // Unit Quaternion
};

// An animated joint in the armature - may or may not be weighted to a bone
struct AnimationNode {
	AnimationNode();
	
	std::vector<PositionKeyFrame> positionKeyFrame;
	std::vector<PositionKeyFrame> scaleKeyFrames;
	std::vector<RotationKeyFrame> rotationKeyFrame;
	glm::detail::int32 numChildren;
	glm::detail::int32 boneIndex;
	glm::detail::bool hasBone;
	std::vector<AnimationNode*> children;
};

struct AnimationNodeNameMap {
	AnimationNodeNameMap();
	
	aiString assImpName;
	AnimationNode* animationNodePtr;
};
