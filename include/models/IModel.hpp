#ifndef IMODEL_H_
#define IMODEL_H_

#include "IRenderable.hpp"
#include "glw/shaders/IShaderProgram.hpp"

#include "glw/IMesh.hpp"
#include "glw/ITexture.hpp"
#include "glw/IAnimation.hpp"

#include "Id.hpp"

#include "serialize/ITextSerializable.hpp"

namespace glr
{
namespace models
{

/**
 * 
 */
class IModel : public virtual IRenderable, public virtual serialize::ITextSerializable, public virtual glw::IGraphicsObject
{
public:
	virtual ~IModel()
	{
	}
	;
	
	/**
	 * Get the id of this model.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 * 
	 * @return The id of this model.
	 */
	virtual const Id& getId() const = 0;
	
	/**
	 * Get the name of this model.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 * 
	 * @return The name of this model.
	 */
	virtual const std::string& getName() const = 0;

	/**
	 * Plays the given animation for this model.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 * 
	 * @param animation
	 */
	virtual void playAnimation(glw::IAnimation* animation, glm::detail::float32 animationTime = 0.0f, bool loop = true) = 0;

	/**
	 * Plays the given animation for this model.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 * 
	 * @param animation
	 */
	virtual void playAnimation(glw::IAnimation* animation, glm::detail::uint32 startFrame, glm::detail::uint32 endFrame, glm::detail::float32 animationTime = 0.0f, bool loop = true) = 0;
	
	// TODO: Should this be here?
	/**
	 * Sets the animation time for animation currently playing.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 */
	virtual void setAnimationTime(glm::detail::float32 animationTime) = 0;
	
	/**
	 * Stop the currently playing animation for this model.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 */
	virtual void stopAnimation() = 0;

	/**
	 * Returns the current animation that affects this model.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 * 
	 * @return A pointer to the current animation that affects this model, or nullptr if there is no current animation.
	 */
	virtual glw::IAnimation* getPlayingAnimation() const = 0;

	/**
	 * Sets the current animation.  If nullptr is sent in as the animation, then no animation will be used when rendering
	 * this model.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 * 
	 * @param animation The animation to use as the current animation.
	 */
	virtual std::vector<glw::IAnimation*> getAnimations() const = 0;
};

}
}

#endif /* IMODEL_H_ */
