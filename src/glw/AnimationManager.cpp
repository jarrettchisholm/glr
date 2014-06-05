#include <utility>

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
	std::lock_guard<std::mutex> lock(accessMutex_);
	
	auto it = animations_.find(name);
	if ( it != animations_.end() )
	{
		LOG_DEBUG( "Animation found." );
		return it->second.get();
	}

	LOG_DEBUG( "Animation not found." );
	
	return nullptr;
}

Animation* AnimationManager::addAnimation(const std::string& name, bool initialize)
{
	std::lock_guard<std::mutex> lock(accessMutex_);
	
	LOG_DEBUG( "Loading animation..." );

	auto it = animations_.find(name);
	if ( it != animations_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Animation already exists." );
		return it->second.get();
	}

	LOG_DEBUG( "Creating Animation." );
	auto animation = std::unique_ptr<Animation>(new Animation(openGlDevice_, name, initialize));
	auto animationPointer = animation.get();
	
	animations_[name] = std::move(animation);

	return animationPointer;
}

Animation* AnimationManager::addAnimation(const std::string& name, glm::detail::float64 duration, glm::detail::float64 ticksPerSecond, std::map< std::string, AnimatedBoneNode > animatedBoneNodes, bool initialize)
{
	std::lock_guard<std::mutex> lock(accessMutex_);
	
	LOG_DEBUG( "Loading animation..." );
	
	auto it = animations_.find(name);
	if ( it != animations_.end() && it->second.get() != nullptr )
	{
		LOG_DEBUG( "Animation already exists." );
		return it->second.get();
	}

	LOG_DEBUG( "Creating Animation." );
	auto animation = std::unique_ptr<Animation>(new Animation(openGlDevice_, name, duration, ticksPerSecond, animatedBoneNodes, initialize));
	auto animationPointer = animation.get();
	
	animations_[name] = std::move(animation);

	return animationPointer;
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
	boost::serialization::split_member(ar, *this, version);
}

template<class Archive> void AnimationManager::save(Archive& ar, const unsigned int version) const
{
	boost::serialization::void_cast_register<AnimationManager, IAnimationManager>(
		static_cast<AnimationManager*>(nullptr),
		static_cast<IAnimationManager*>(nullptr)
	);
	
	auto size = animations_.size();
	ar & size;
	for (auto& it : animations_)
	{
		ar & it.first & *(it.second.get());
	}
}

template<class Archive> void AnimationManager::load(Archive& ar, const unsigned int version)
{
	boost::serialization::void_cast_register<AnimationManager, IAnimationManager>(
		static_cast<AnimationManager*>(nullptr),
		static_cast<IAnimationManager*>(nullptr)
	);

	std::map< std::string, std::unique_ptr<Animation> >::size_type numAnimations = 0;
	ar & numAnimations;

	animations_ = std::map< std::string, std::unique_ptr<Animation> >();

	for (glmd::uint32 i=0; i < numAnimations; i++)
	{
		auto s = std::string();
		ar & s;
		
		auto animation = std::unique_ptr<Animation>( new Animation(openGlDevice_, s) );
		ar & *(animation.get());
		
		animations_[s] = std::move(animation);
	}
}

}
}

BOOST_CLASS_EXPORT(glr::glw::IAnimationManager)
BOOST_CLASS_EXPORT_GUID(glr::glw::AnimationManager, "glr::glw::AnimationManager")
