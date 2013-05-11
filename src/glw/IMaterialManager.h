/*
 * IMaterialManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef IMATERIALMANAGER_H_
#define IMATERIALMANAGER_H_

#include <string>

#include "Material.h"

namespace glr {
namespace glw {

class IMaterialManager {
public:
	virtual ~IMaterialManager()
	{
	}
	;

	virtual Material* getMaterial(const std::string path) = 0;
};

}
}
#endif /* IMATERIALMANAGER_H_ */
