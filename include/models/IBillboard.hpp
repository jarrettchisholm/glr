#ifndef IBILLBOARD_H_
#define IBILLBOARD_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Id.hpp"

#include "IRenderable.hpp"
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
class IBillboard : public virtual IRenderable
{
public:
	virtual ~IBillboard()
	{
	}
	;

	/**
	 * Get the id of this model.
	 * 
	 * @return The id of this model.
	 */
	virtual const Id& getId() const = 0;
	
	/**
	 * Get the name of this model.
	 * 
	 * @return The name of this model.
	 */
	virtual const std::string& getName() const = 0;
};

}
}

#endif /* IBILLBOARD_H_ */
