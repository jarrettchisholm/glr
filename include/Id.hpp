#ifndef ID_H_
#define ID_H_

#include <iostream>

#include <glm/glm.hpp>

namespace glr
{

class Id
{
public:
	Id();
	explicit Id(glm::detail::uint32 id);
	// We make the destructor non-virtual so that we can't extend this class easily
	~Id();
	
	glm::detail::uint32 getId() const;
	
	bool operator == (const Id &other) const;
	bool operator != (const Id &other) const;
	bool operator < (const Id &other) const;
	bool operator > (const Id &other) const;
	bool operator <= (const Id &other) const;
	bool operator >= (const Id &other) const;
	bool operator << (const Id &other) const;
	
	friend std::ostream& operator << (std::ostream& os, const Id& id);
	
	static const Id INVALID;

private:
	glm::detail::uint32 id_;
};

}

#endif /* ID_H_ */
