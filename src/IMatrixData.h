/*
 * IMatrixData.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#ifndef IMATRIXDATA_H_
#define IMATRIXDATA_H_

#include "glm/glm.hpp"

namespace glr {

class IMatrixData {
public:
	virtual ~IMatrixData()
	{
	}
	;

	virtual const glm::mat4& getViewMatrix() = 0;
	virtual const glm::mat4& getProjectionMatrix() = 0;
	virtual const glm::mat4& getModelMatrix() = 0;
};
}
#endif /* IMATRIXDATA_H_ */
