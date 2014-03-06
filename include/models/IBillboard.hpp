#ifndef IBILLBOARD_H_
#define IBILLBOARD_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Id.hpp"

#include "glw/shaders/IShaderProgram.hpp"

#include "glw/IMesh.hpp"
#include "glw/ITexture.hpp"
#include "glw/IMaterial.hpp"

namespace glr
{
namespace models
{

/**
 * 
 */
class IBillboard
{
public:
	virtual ~IBillboard()
	{
	}
	;

	virtual const Id& getId() const = 0;
	virtual const std::string& getName() const = 0;

	/**
	 * Render the billboard to the scene, using the provided shader.
	 * 
	 * TODO: Should we do it like this, with the shader being passed in?
	 * 
	 * @param shader The shader to use to render this billboard to the scene.
	 */
	virtual void render(shaders::IShaderProgram* shader) = 0;
};

}
}

#endif /* IBILLBOARD_H_ */
