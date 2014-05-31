#ifndef IMATERIAL_H_
#define IMATERIAL_H_

#include "IGraphicsObject.hpp"

#include "serialize/ITextSerializable.hpp"

namespace glr
{
namespace glw
{

class IMaterial : public virtual IGraphicsObject, public virtual serialize::ITextSerializable
{
public:
	virtual ~IMaterial()
	{
	}
	;

	/**
	 * Binds the data in OpenGL, making it ready to use.
	 * 
	 * **Not Thread Safe**: This method is *not* safe to call in a multi-threaded environment, and should only be called from the 
	 * OpenGL thread.
	 */
	virtual void bind() const = 0;
	
	/**
	 * Returns the OpenGL Buffer Id for this resource.
	 * 
	 * @return The OpenGL Buffer Id.
	 */
	virtual GLuint getBufferId() const = 0;
	
	/**
	 * Returns the name of this material.
	 * 
	 * @return The name of the material.
	 */
	virtual const std::string& getName() const = 0;
};

}
}

#endif /* IMATERIAL_H_ */
