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

class ShaderProgramManager: public IShaderProgramManager {
public:
	ShaderProgramManager();
	virtual ~ShaderProgramManager();

	IShaderProgram* getShaderProgram(const std::string name);

	void load(const std::string directory);

private:	
	std::map< std::string, std::unique_ptr<OglreShaderProgram> >	oglreProgramMap_;
	std::map< std::string, std::shared_ptr<OglreShader> >			oglreShaderMap_;
	std::map< std::string, std::unique_ptr<GlslShaderProgram> >		glslProgramMap_;
	std::map< std::string, std::shared_ptr<GlslShader> >			glslShaderMap_;
};

}

}

#endif /* SHADERPROGRAMMANAGER_H_ */
