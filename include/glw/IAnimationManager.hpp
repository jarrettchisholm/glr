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
	 * **Note**: If initialize is true, this method is *not* thread safe (i.e. it should only be called from the OpenGL thread).  If it is
	 * false, then this method *is* thread safe (i.e. it is safe to call outside the OpenGL thread).
	 * 
	 * @param name The name to use for the new animation.
	 * @param initialize If true, will initialize all of the resources required for this animation.  Otherwise, it will
	 * just create the animation and return it (without initializing it).
	 * 
	 * @return An Animation object.
	 */
	virtual Animation* addAnimation(const std::string& name, bool initialize = true) = 0;
	
	/**
	 * Creates an animation with the given name and using the provided animation data.
	 * 
	 * If an animation already exists with the given name, it will return that animation.
	 * 
	 * **Note**: If initialize is true, this method is *not* thread safe (i.e. it should only be called from the OpenGL thread).  If it is
	 * false, then this method *is* thread safe (i.e. it is safe to call outside the OpenGL thread).
	 * 
	 * @param name The name to use for the new animation.
	 * @param duration The duration of the new animation.
	 * @param ticksPerSecond The number of ticks per second.
	 * @param animatedBoneNodes The animated bone nodes.
	 * @param initialize If true, will initialize all of the resources required for this animation.  Otherwise, it will
	 * just create the animation and return it (without initializing it).
	 * 
	 * @return An Animation object.
	 */
	virtual Animation* addAnimation(const std::string& name, glm::detail::float64 duration, glm::detail::float64 ticksPerSecond, std::map< std::string, AnimatedBoneNode > animatedBoneNodes, bool initialize = true) = 0;
};

}
}

#endif /* IANIMATIONMANAGER_H_ */
