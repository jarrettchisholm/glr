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
	ShaderProgram* getShaderProgram(ShaderProgramInfo shaderProgramInfo);
	
	void loadShaderPrograms(const std::string filename);
	void loadShaderProgramsFromData(const std::string data);
	
private:
	static ShaderProgramManager* shaderProgramManager_;

	std::map< std::string, std::unique_ptr<ShaderProgram> > shaderPrograms_;
	
	/**
	 * Loads the standard set of shader programs into OpenGL.
	 * 
	 * @exception GlException if an error occurs while compiling and linking shaders into shader programs.
	 */ 
	void loadStandardShaderPrograms();
	
};

}

}

#endif /* SHADERPROGRAMMANAGER_H_ */
