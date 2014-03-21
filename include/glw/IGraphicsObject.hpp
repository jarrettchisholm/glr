#ifndef IGRAPHICSOBJECT_H_
#define IGRAPHICSOBJECT_H_

namespace glr
{
namespace glw
{

class IGraphicsObject
{
public:
	virtual ~IGraphicsObject()
	{
	}
	;

	/**
	 * Will stream the latest graphics data into opengl memory.
	 */
	virtual void pushToVideoMemory() = 0;
	virtual void pullFromVideoMemory() = 0;
	virtual void freeLocalData() = 0;
	virtual void freeVideoMemory() = 0;
	virtual void allocateVideoMemory() = 0;
};

}
}

#endif /* IGRAPHICSOBJECT_H_ */
