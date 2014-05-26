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
	 * Will free the local data used by this graphics object.
	 * 
	 * **Note**: This method is *not* thread safe - it should only be called from the OpenGL thread.
	 */
	virtual void allocateVideoMemory() = 0;

	/**
	 * Will stream the latest graphics data into opengl memory.
	 * 
	 * **Note**: This method is *not* thread safe - it should only be called from the OpenGL thread.
	 */
	virtual void pushToVideoMemory() = 0;
	
	/**
	 * Will stream the graphics data from opengl memory to local memory.
	 * 
	 * **Note**: This method is *not* thread safe - it should only be called from the OpenGL thread.
	 */
	virtual void pullFromVideoMemory() = 0;
	
	/**
	 * Will free the local data used by this graphics object.
	 * 
	 * **Note**: This method is *not* thread safe - it should only be called from the OpenGL thread.
	 */
	virtual void freeVideoMemory() = 0;
	
	/**
	 * Tells us whether the video memory has been allocated.
	 * 
	 * @return true if the video memory has been allocated, and false otherwise.
	 */
	virtual bool isVideoMemoryAllocated() const = 0;
	
	/**
	 * Will load any resources required for this graphics object into local memory.
	 * 
	 * **Note**: This method *is* thread safe - it is safe to call outside the OpenGL thread.
	 */
	virtual void loadLocalData() = 0;
	
	/**
	 * Will free the local data used by this graphics object.
	 * 
	 * **Note**: This method *is* thread safe - it is safe to call outside the OpenGL thread.
	 */
	virtual void freeLocalData() = 0;
	
	/**
	 * Tells us whether the local data has been loaded into memory.
	 * 
	 * @return true if the local data has been loaded into memory, and false otherwise.
	 */
	virtual bool isLocalDataLoaded() const = 0;
	
	/**
	 * Tells us whether the local data has changed and the data in OpenGL needs updating.
	 * 
	 * @return true if the local data has changed since it was last loaded into OpenGL, and false otherwise.
	 */
	virtual bool isDirty() const = 0;
};

}
}

#endif /* IGRAPHICSOBJECT_H_ */
