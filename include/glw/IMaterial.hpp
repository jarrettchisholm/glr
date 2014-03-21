#ifndef IMATERIAL_H_
#define IMATERIAL_H_

#include "IGraphicsObject.hpp"

namespace glr
{
namespace glw
{

class IMaterial : public virtual IGraphicsObject
{
public:
	virtual ~IMaterial()
	{
	}
	;

	virtual void bind() const = 0;
	
	virtual GLuint getBufferId() const = 0;
};

}
}

#endif /* IMATERIAL_H_ */
