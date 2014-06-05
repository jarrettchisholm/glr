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
	 * **Not Thread Safe**: This method is *not* safe to call in a multi-threaded environment, and should only be called from the 
	 * OpenGL thread.
	 */
	virtual void allocateVideoMemory() = 0;

	/**
	 * Will stream the latest graphics data into opengl memory.
	 * 
	 * **Not Thread Safe**: This method is *not* safe to call in a multi-threaded environment, and should only be called from the 
	 * OpenGL thread.
	 */
	virtual void pushToVideoMemory() = 0;
	
	/**
	 * Will stream the graphics data from opengl memory to local memory.
	 * 
	 * **Not Thread Safe**: This method is *not* safe to call in a multi-threaded environment, and should only be called from the 
	 * OpenGL thread.
	 */
	virtual void pullFromVideoMemory() = 0;
	
	/**
	 * Will free the local data used by this graphics object.
	 * 
	 * **Not Thread Safe**: This method is *not* safe to call in a multi-threaded environment, and should only be called from the 
	 * OpenGL thread.
	 */
	virtual void freeVideoMemory() = 0;
	
	/**
	 * Tells us whether the video memory has been allocated.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 * 
	 * @return true if the video memory has been allocated, and false otherwise.
	 */
	virtual bool isVideoMemoryAllocated() const = 0;
	
	/**
	 * Will load any resources required for this graphics object into local memory.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 */
	virtual void loadLocalData() = 0;
	
	/**
	 * Will free local data used by this graphics object.
	 * 
	 * Note that some local data may be retained, at the discretion of the implementer.  Also, calling this
	 * method does not guarantee that 'isLocalDataLoaded()' will return false (in which case, the necessary local
	 * data for this graphics object is still loaded).
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 */
	virtual void freeLocalData() = 0;
	
	/**
	 * Tells us whether the local data has been loaded into memory.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 * 
	 * @return true if the local data has been loaded into memory, and false otherwise.
	 */
	virtual bool isLocalDataLoaded() const = 0;
	
	/**
	 * Tells us whether the local data has changed and the data in OpenGL needs updating.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 * 
	 * @return true if the local data has changed since it was last loaded into OpenGL, and false otherwise.
	 */
	virtual bool isDirty() const = 0;
};

}
}

#endif /* IGRAPHICSOBJECT_H_ */
