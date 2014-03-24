#ifndef IFIELDFUNCTION_H_
#define IFIELDFUNCTION_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace glr
{
namespace terrain
{

class IFieldFunction
{
public:
	virtual ~IFieldFunction()
	{
	}
	;

	virtual glm::detail::float32 getNoise(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z) = 0;
};

}
}

#endif /* IFIELDFUNCTION_H_ */
