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
#include "GlrShaderProgram.h"
#include "GlslShader.h"
#include "GlrShader.h"


namespace glr {
namespace shaders {
namespace fs = boost::filesystem;

class ShaderProgramManager : public IShaderProgramManager {
public:
	ShaderProgramManager(bool autoLoad = true);
	virtual ~ShaderProgramManager();

	virtual IShaderProgram* getShaderProgram(const std::string name);

	virtual void loadShaderPrograms(const std::string directory);

	virtual void addDefaultBindListener(IShaderProgramBindListener* bindListener);
	virtual void removeDefaultBindListener(IShaderProgramBindListener* bindListener);
	void removeAllDefaultBindListeners();

	void loadStandardShaderPrograms();

	void load(const std::string directory);
	void load(fs::path directory);
	void load(std::vector<std::string> filenames);
	void load(std::vector<fs::path> filePaths);
	void load(std::map<std::string, std::string> dataMap);

private:
	std::map< std::string, std::shared_ptr<GlrShaderProgram> >      glrProgramMap_;
	std::map< std::string, std::shared_ptr<GlrShader> >                     glrShaderMap_;
	std::map< std::string, std::shared_ptr<GlslShaderProgram> >             glslProgramMap_;
	std::map< std::string, std::shared_ptr<GlslShader> >                    glslShaderMap_;

	std::vector<IShaderProgramBindListener> defaultBindListeners_;

	std::unique_ptr<GlslShaderProgram> convertGlrProgramToGlslProgram(GlrShaderProgram* glrProgram);

	bool isShader(std::string s);
	bool isProgram(std::string s);
};
}
}
#endif /* SHADERPROGRAMMANAGER_H_ */
