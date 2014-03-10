#include "Configure.hpp"

#ifdef OS_WINDOWS
#include <windows.h>
#endif

#include "glw/AnimationManager.hpp"


namespace glr
{
namespace glw
{
	
AnimationManager::AnimationManager(IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
	// Create and add GLR_IDENTITY_BONES animation
	
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

}
}
