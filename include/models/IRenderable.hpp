#ifndef IRENDERABLE_H_
#define IRENDERABLE_H_

#include "glw/shaders/IShaderProgram.hpp"

namespace glr
{
namespace models
{

class IRenderable
{
public:
	virtual ~IRenderable()
	{
	}
	;

	/**
	 * Render this object to the scene using the provided shader.
	 * 
	 * @param shader The shader to use to render this object to the scene.
	 */
	virtual void render(shaders::IShaderProgram* shader) = 0;
};

}
}

#endif /* IRENDERABLE_H_ */
