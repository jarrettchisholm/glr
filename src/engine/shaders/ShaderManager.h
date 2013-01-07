/*
 * ShaderManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef SHADERMANAGER_H_
#define SHADERMANAGER_H_

#include <memory>
#include <map>
#include <string>

#include "Shader.h"

namespace oglre {

namespace shaders {

class ShaderManager {
public:
	ShaderManager();
	virtual ~ShaderManager();
	ShaderManager(ShaderManager const&);
	ShaderManager& operator=(ShaderManager const&);
	
	static ShaderManager* getInstance();

	Shader* getShader(const std::string filename);
	
private:
	static ShaderManager* shaderManager_;

	std::map< std::string, std::unique_ptr<Shader> > shaders_;
	
};

}

}

#endif /* SHADERMANAGER_H_ */
