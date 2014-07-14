#ifndef IMATERIAL_H_
#define IMATERIAL_H_

#include "IGraphicsObject.hpp"
#include "IMaterialBindListener.hpp"

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
	virtual void bind() = 0;
	
	/**
	 * Returns the OpenGL Buffer Id for this resource.
	 * 
	 * @return The OpenGL Buffer Id.
	 */
	virtual GLuint getBufferId() const = 0;
	
	virtual void setAmbient(const glm::vec4& ambient) = 0;
	virtual void setDiffuse(const glm::vec4& diffuse) = 0;
	virtual void setSpecular(const glm::vec4& specular) = 0;
	virtual void setEmission(const glm::vec4& emission) = 0;
	virtual void setShininess(glm::detail::float32 shininess) = 0;
	virtual void setStrength(glm::detail::float32 strength) = 0;
	
	/**
	 * Returns the name of this material.
	 * 
	 * @return The name of the material.
	 */
	virtual const std::string& getName() const = 0;
	
	/**
	 * Add a listener, which will be notified when this material gets bound.
	 * 
	 * @param bindListener A pointer to an object implementing the IMaterialBindListener interface.
	 */
	virtual void addBindListener(IMaterialBindListener* bindListener) = 0;
	
	/**
	 * Remove a bind listener.
	 * 
	 * @param bindListener A pointer to an IMaterialBindListener object that should be removed as a bind listener for this material.
	 */
	virtual void removeBindListener(IMaterialBindListener* bindListener) = 0;
};

}
}

#endif /* IMATERIAL_H_ */
