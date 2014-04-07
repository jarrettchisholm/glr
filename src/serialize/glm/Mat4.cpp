#include "serialize/glm/Mat4.hpp"

namespace boost
{
namespace serialization
{

template<class Archive> void save(Archive& ar, const glm::mat4& m, unsigned int version)
{
	for (glm::detail::uint32 i=0; i < 4u; i++)
		for (glm::detail::uint32 j=0; j < 4u; j++)
			ar & m[i][j];
}

template<class Archive> void load(Archive& ar, glm::mat4& m, unsigned int version)
{
	for (glm::detail::uint32 i=0; i < 4u; i++)
		for (glm::detail::uint32 j=0; j < 4u; j++)
			ar & m[i][j];
}

}
}
