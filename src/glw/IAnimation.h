#ifndef IANIMATION_H_
#define IANIMATION_H_

#include <string>

#include "../common/utilities/Macros.hpp"

namespace glr
{
namespace glw
{

struct BoneNode
{
	std::string name;
	glm::mat4 transformation;
	std::vector< BoneNode > children;
};

class IAnimation
{
public:
	virtual ~IAnimation()
	{
	}
	;

	virtual const std::string& getName() = 0;
	virtual void setAnimationTime(glm::detail::float32 runningTime) = 0;
};

}
}

#endif /* IANIMATION_H_ */
