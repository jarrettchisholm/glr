/*
 * IModel.h
 *
 *  Created on: 2012-10-20
 *      Author: jarrett
 */

#ifndef IMODEL_H_
#define IMODEL_H_

#include "../IMatrixData.h"
#include "../shaders/IShaderProgram.h"

namespace glr {
namespace models {
class IModel {
public:
	IModel()
	{
	}
	;

	virtual ~IModel()
	{
	}
	;

	virtual void render(IMatrixData* matrixData, shaders::IShaderProgram* shader) = 0;
};
}
}
#endif /* IMODEL_H_ */
