#ifndef ITERRAIN_H_
#define ITERRAIN_H_

#include "ISceneNode.hpp"

namespace glr
{
namespace terrain
{

class ITerrain : public virtual ISceneNode
{
public:
	virtual ~ITerrain()
	{
	}
	;

	virtual bool isDirty() const = 0;
};

}
}

#endif /* ITERRAIN_H_ */
