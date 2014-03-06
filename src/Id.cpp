#include "Id.hpp"

namespace glr
{

Id::Id() : id_(0)
{
}

Id::Id(glm::detail::uint32 id) : id_(id)
{
}


Id::~Id()
{
}
	
glm::detail::uint32 Id::getId() const
{
	return id_;
}
	
bool Id::operator == (const Id &other) const
{
	return id_ == other.id_;
}

bool Id::operator != (const Id &other) const
{
	return id_ != other.id_;
}

bool Id::operator < (const Id &other) const
{
	return id_ < other.id_;
}

bool Id::operator > (const Id &other) const
{
	return other.id_ < id_;
}

bool Id::operator <= (const Id &other) const
{
	return !(id_ > other.id_);
}

bool Id::operator >= (const Id &other) const
{
	return !(id_ < other.id_);
}

std::ostream& operator << (std::ostream& os, const Id& id)
{
	os << id.id_;
	return os;
}

}
