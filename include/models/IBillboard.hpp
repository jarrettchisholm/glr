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

	virtual const Id& getId() const = 0;
	virtual const std::string& getName() const = 0;
};

}
}

#endif /* IBILLBOARD_H_ */
