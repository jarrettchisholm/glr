#ifndef IANIMATIONMANAGER_H_
#define IANIMATIONMANAGER_H_

#include <string>

#include "Animation.hpp"

#include "serialize/ITextSerializable.hpp"

namespace glr
{
namespace glw
{

class Animation;

class IAnimationManager : public virtual serialize::ITextSerializable
{
public:
	virtual ~IAnimationManager()
	{
	}
	;

	/**
	 * Returns the animation with the given name.  If an animation with the given name doesn't exist, it returns
	 * nullptr.
	 * 
	 * @param name The name of the animation to retrieve.
	 * 
	 * @return The Animation object with name 'name', or nullptr if no animation exists with that name.
	 */
	virtual Animation* getAnimation(const std::string& name) const = 0;
	
	/**
	 * Creates an animation with the given name.
	 * 
	 * If an animation already exists with the given name, it will return that animation.
	 * 
	 * @param name The name to use for the new animation.
	 * 
	 * @return An Animation object.
	 */
	virtual Animation* addAnimation(const std::string& name) = 0;
	
	/**
	 * Creates an animation with the given name and using the provided animation data.
	 * 
	 * If an animation already exists with the given name, it will return that animation.
	 * 
	 * @param name The name to use for the new animation.
	 * @param duration The duration of the new animation.
	 * @param ticksPerSecond The number of ticks per second.
	 * @param animatedBoneNodes The animated bone nodes.
	 * 
	 * @return An Animation object.
	 */
	virtual Animation* addAnimation(const std::string& name, glm::detail::float64 duration, glm::detail::float64 ticksPerSecond, std::map< std::string, AnimatedBoneNode > animatedBoneNodes) = 0;
};

}
}

#endif /* IANIMATIONMANAGER_H_ */
