#include "Configure.hpp"

#ifdef OS_WINDOWS
#include <windows.h>
#endif

#include "glw/AnimationManager.hpp"
#include "glw/Constants.hpp"


namespace glr
{
namespace glw
{

AnimationManager::AnimationManager()
{
	openGlDevice_ = nullptr;
	
	// Create and add GLR_IDENTITY_BONES animation
	addAnimation(glw::Constants::GLR_IDENTITY_BONES);
}
	
AnimationManager::AnimationManager(IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
	// Create and add GLR_IDENTITY_BONES animation
	addAnimation(glw::Constants::GLR_IDENTITY_BONES);
}

AnimationManager::~AnimationManager()
{
}

Animation* AnimationManager::getAnimation(const std::string& name) const
{
	auto it = animations_.find(name);
	if ( it != animations_.end() )
	{
		LOG_DEBUG( "Animation found." );
		return it->second.get();
	}

	LOG_DEBUG( "Animation not found." );
	
	return nullptr;
}

Animation* AnimationManager::addAnimation(const std::string& name)
{
	LOG_DEBUG( "Loading animation..." );

	auto it = animations_.find(name);
	if ( it != animations_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Animation already exists." );
		return it->second.get();
	}

	LOG_DEBUG( "Creating Animation." );
	animations_[name] = std::unique_ptr<Animation>(new Animation(openGlDevice_, name));

	return animations_[name].get();
}

Animation* AnimationManager::addAnimation(const std::string& name, glm::detail::float64 duration, glm::detail::float64 ticksPerSecond, std::map< std::string, AnimatedBoneNode > animatedBoneNodes)
{
	LOG_DEBUG( "Loading animation..." );

	auto it = animations_.find(name);
	if ( it != animations_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Animation already exists." );
		return it->second.get();
	}

	LOG_DEBUG( "Creating Animation." );
	animations_[name] = std::unique_ptr<Animation>(new Animation(openGlDevice_, name, duration, ticksPerSecond, animatedBoneNodes));

	return animations_[name].get();
}

void AnimationManager::serialize(const std::string& filename)
{
	std::ofstream ofs(filename.c_str());
	serialize::TextOutArchive textOutArchive(ofs);
	serialize(textOutArchive);
}

void AnimationManager::serialize(serialize::TextOutArchive& outArchive)
{
	outArchive << *this;
}

void AnimationManager::deserialize(const std::string& filename)
{
	std::ifstream ifs(filename.c_str());
	serialize::TextInArchive textInArchive(ifs);
	deserialize(textInArchive);
}

void AnimationManager::deserialize(serialize::TextInArchive& inArchive)
{
	inArchive >> *this;
}

template<class Archive> void AnimationManager::serialize(Archive& ar, const unsigned int version)
{
	boost::serialization::void_cast_register<AnimationManager, IAnimationManager>(
		static_cast<AnimationManager*>(nullptr),
		static_cast<IAnimationManager*>(nullptr)
	);
	/*
	ar & name_;
	ar & vertices_;
	ar & normals_;
	ar & textureCoordinates_;
	ar & colors_;
	ar & vertexBoneData_;
	ar & boneData_;
	*/
}

}
}

BOOST_CLASS_EXPORT(glr::glw::IAnimationManager)
BOOST_CLASS_EXPORT_GUID(glr::glw::AnimationManager, "glr::glw::AnimationManager")
