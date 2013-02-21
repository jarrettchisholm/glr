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

#include <boost/filesystem.hpp>

#include "IShaderProgramManager.h"

#include "GlslShaderProgram.h"
#include "OglreShaderProgram.h"
#include "GlslShader.h"
#include "OglreShader.h"


namespace oglre {

namespace shaders {

namespace fs = boost::filesystem;

class ShaderProgramManager: public IShaderProgramManager {
public:
	ShaderProgramManager();
	virtual ~ShaderProgramManager();

	virtual IShaderProgram* getShaderProgram(const std::string name);

	virtual void loadShaderPrograms(const std::string directory);
	
	void load(const std::string directory);	
	void load(fs::path directory);
	void load(std::vector<std::string> filenames);
	void load(std::vector<fs::path> filePaths);
	void load(std::map<std::string, std::string> dataMap);

private:	
	std::map< std::string, std::unique_ptr<OglreShaderProgram> >	oglreProgramMap_;
	std::map< std::string, std::shared_ptr<OglreShader> >			oglreShaderMap_;
	std::map< std::string, std::unique_ptr<GlslShaderProgram> >		glslProgramMap_;
	std::map< std::string, std::shared_ptr<GlslShader> >			glslShaderMap_;
	
	std::unique_ptr<GlslShaderProgram> convertOglreProgramToGlslProgram( OglreShaderProgram* oglreProgram );
};

}

}

#endif /* SHADERPROGRAMMANAGER_H_ */
