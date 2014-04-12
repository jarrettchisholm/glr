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
	 */
	virtual void render() = 0;
	
	/**
	 * Returns a reference to the bone data.
	 */
	virtual BoneData& getBoneData() = 0;
	
	virtual const std::string& getName() const = 0;
};

}
}

#endif /* IMESH_H_ */
