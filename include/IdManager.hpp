#ifndef IDMANAGER_H_
#define IDMANAGER_H_

#include "Id.hpp"

namespace glr
{

class IdManager
{
public:
	IdManager();
	virtual ~IdManager();
	
	Id createId();

private:
	glm::detail::uint32 currentId_;
};

}

#endif /* IDMANAGER_H_ */
