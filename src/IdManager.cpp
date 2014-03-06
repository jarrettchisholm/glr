#include "IdManager.hpp"

namespace glr
{

IdManager::IdManager() : currentId_(1)
{
}

IdManager::~IdManager()
{
}

Id IdManager::createId()
{
	return Id(++currentId_);
}

}
