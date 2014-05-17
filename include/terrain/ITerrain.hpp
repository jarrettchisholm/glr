#ifndef ITERRAIN_H_
#define ITERRAIN_H_

#include "ISceneNode.hpp"
#include "TerrainSettings.hpp"

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
	
	/**
	 * Will render the terrain to scene.
	 * 
	 * Note: This method is NOT thread safe.  Only call it from the OpenGL thread.
	 */
	virtual void render() = 0;
	
	/**
	 * Tells us whether this terrain has changes that need to be passed to OpenGL.
	 */
	virtual bool isDirty() const = 0;
	virtual void setIsDirty(bool isDirty) = 0;
	
	/**
	 * Will do up to maxUpdates number of updates to/from the graphics card.
	 * 
	 * Note: This method is NOT thread safe.  Only call it from the OpenGL thread.
	 */
	virtual void update() = 0;
	
	virtual ? getLocalData() = 0;
	virtual void setLocalData() = 0;
	
	virtual void setLod(LevelOfDetail lod) = 0;
	virtual LevelOfDetail getLod() const = 0;
};

}
}

#endif /* ITERRAIN_H_ */
