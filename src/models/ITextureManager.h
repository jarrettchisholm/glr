/*
 * ITextureManager.h
 *
 *  Created on: 2013-04-13
 *      Author: jarrett
 */

#ifndef ITEXTUREMANAGER_H_
#define ITEXTUREMANAGER_H_

#include <string>

#include "Texture.h"

namespace glr {
namespace models {
class ITextureManager {
public:
	virtual ~ITextureManager()
	{
	}
	;

	virtual Texture* getTexture(const std::string filename) = 0;
};
}
}
#endif /* ITEXTUREMANAGER_H_ */
