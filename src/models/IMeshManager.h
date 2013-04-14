/*
 * IMeshManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef IMESHMANAGER_H_
#define IMESHMANAGER_H_

#include <string>

#include "Mesh.h"

namespace glr {
namespace models {
class IMeshManager {
public:
	virtual ~IMeshManager()
	{
	}
	;

	virtual Mesh* getMesh(const std::string path) = 0;
};
}
}
#endif /* IMESHMANAGER_H_ */
