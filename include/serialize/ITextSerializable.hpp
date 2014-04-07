#ifndef ITEXTSERIALIZABLE_H_
#define ITEXTSERIALIZABLE_H_

#include <string>

#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

#include "serialize/glm/Vec3.hpp"
#include "serialize/glm/Mat4.hpp"
#include "serialize/glm/Quat.hpp"

#include "TextOutArchive.hpp"
#include "TextInArchive.hpp"

namespace glr
{
namespace serialize
{

/**
 * 
 */
class ITextSerializable
{
public:
	virtual ~ITextSerializable()
	{
	}
	;

	virtual void serialize(const std::string& filename) = 0;
	virtual void serialize(TextOutArchive& outArchive) = 0;

	virtual void deserialize(const std::string& filename) = 0;
	virtual void deserialize(TextInArchive& inArchive) = 0;
};

}
}

#endif /* ITEXTSERIALIZABLE_H_ */
