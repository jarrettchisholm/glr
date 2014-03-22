#ifndef IENVIRONMENTMANAGER_H_
#define IENVIRONMENTMANAGER_H_

#include <string>

#include "ISceneNode.hpp"
#include "environment/ISky.hpp"

namespace glr
{
namespace env
{

class IEnvironmentManager
{
public:
	virtual ~IEnvironmentManager()
	{
	}
	;
	
	virtual ISky* createSkyBox() = 0;
	virtual ISky* getSkyBox(Id id) = 0;
	virtual ISky* getSkyBox(const std::string& name) = 0;
	virtual void destroySkyBox(Id id) = 0;
	virtual void destroySkyBox(const std::string& name) = 0;
	virtual void destroySkyBox(ISky* skybox) = 0;
	
	virtual void render() = 0;
	
	virtual void setFollowTarget(ISceneNode* target) = 0;
};

}
}

#endif /* IENVIRONMENTMANAGER_H_ */
