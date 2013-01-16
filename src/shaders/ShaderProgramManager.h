/*
 * ShaderProgramManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef SHADERPROGRAMMANAGER_H_
#define SHADERPROGRAMMANAGER_H_

#include <memory>
#include <map>
#include <string>

#include "ShaderProgram.h"

namespace oglre {

namespace shaders {

class ShaderProgramManager {
public:
	ShaderProgramManager();
	virtual ~ShaderProgramManager();
	ShaderProgramManager(ShaderProgramManager const&);
	ShaderProgramManager& operator=(ShaderProgramManager const&);
	
	static ShaderProgramManager* getInstance();

	ShaderProgram* getShaderProgram(const std::string filename);
	ShaderProgram* getShaderProgram(const std::string name, std::vector< std::pair <std::string, shaders::IShader::Type> > shaderInfo);
	
private:
	static ShaderProgramManager* shaderProgramManager_;

	std::map< std::string, std::unique_ptr<ShaderProgram> > shaderPrograms_;
	
};

}

}

#endif /* SHADERPROGRAMMANAGER_H_ */
