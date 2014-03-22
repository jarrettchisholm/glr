#ifndef ISKY_H_
#define ISKY_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "ISceneNode.hpp"

namespace glr
{
namespace env
{

class ISky : public virtual ISceneNode
{
public:
	virtual ~ISky()
	{
	}
	;	
};

}
}

#endif /* ISKY_H_ */
