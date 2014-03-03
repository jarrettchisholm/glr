#ifndef ASSIMPLOADER_H_
#define ASSIMPLOADER_H_

#include <string>

#include "IModel.hpp"

namespace glr
{
namespace models
{

class AssImpLoader
{
public:
	AssImpLoader()
	{
	}
	;

	virtual ~AssImpLoader()
	{
	}
	;

	virtual ModelData* loadModel(const std::string path) = 0;
};

}
}

#endif /* ASSIMPLOADER_H_ */
