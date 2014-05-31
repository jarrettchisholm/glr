#ifndef IMESH_H_
#define IMESH_H_

#include <map>
#include <iostream>
#include <sstream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "VertexBoneData.hpp"
#include "BoneData.hpp"
#include "Bone.hpp"

#include "IGraphicsObject.hpp"

#include "common/logger/Logger.hpp"

#include "serialize/ITextSerializable.hpp"

namespace glr
{
namespace glw
{

/**
 * Represents a Mesh which can be rendered in the scene.
 */
class IMesh : public virtual IGraphicsObject, public virtual serialize::ITextSerializable
{
public:
	virtual ~IMesh()
	{
	}
	;

	/**
	 * Will render this mesh in the scene.
	 * 
	 * **Not Thread Safe**: This method is *not* safe to call in a multi-threaded environment, and should only be called from the 
	 * OpenGL thread.
	 */
	virtual void render() = 0;
	
	/**
	 * Returns a reference to the bone data.
	 */
	virtual BoneData& getBoneData() = 0;
	
	/**
	 * Returns the name of this mesh.
	 * 
	 * @return The name of the mesh.
	 */
	virtual const std::string& getName() const = 0;
};

}
}

#endif /* IMESH_H_ */
