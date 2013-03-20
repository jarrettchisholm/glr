/*
 * IModelManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef IMODELMANAGER_H_
#define IMODELMANAGER_H_

#include <string>

#include "IModel.h"

namespace glr {

namespace models {

class IModelManager {
public:
	IModelManager() {
	}
	;
	
	virtual ~IModelManager() {
	}
	;

	virtual IModel* loadModel(const std::string path) = 0;
};

}

}

#endif /* IMODELMANAGER_H_ */
