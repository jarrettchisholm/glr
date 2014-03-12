#ifndef SHADERPROGRAMMANAGER_H_
#define SHADERPROGRAMMANAGER_H_

#include <memory>
#include <map>
#include <string>

#include <boost/filesystem.hpp>

#include "IShaderProgramManager.hpp"

#include "GlslShaderProgram.hpp"
#include "GlrShaderProgram.hpp"
#include "GlslShader.hpp"
#include "GlrShader.hpp"


namespace glr
{
namespace shaders
{
namespace fs = boost::filesystem;

class ShaderProgramManager : public IShaderProgramManager
{
public:
	ShaderProgramManager(glw::IOpenGlDevice* openGlDevice, bool autoLoad = true, std::vector<IShaderProgramBindListener*> defaultBindListeners = std::vector<IShaderProgramBindListener*>());
	virtual ~ShaderProgramManager();

	virtual IShaderProgram* getShaderProgram(const std::string& name) const;

	virtual void loadShaderPrograms(const std::string& directory);
	
	virtual void addDefaultBindListener(IShaderProgramBindListener* bindListener);
	virtual void removeDefaultBindListener(IShaderProgramBindListener* bindListener);
	void removeAllDefaultBindListeners();

	void loadStandardShaderPrograms();
	
	/**
	 * Unloads and then reloads all of the shaders that have previously been loaded.
	 * 
	 * Note that this method is destructive - if you have any pointers to shader programs, their contents
	 * will become undefined after calling this method.
	 */
	virtual void reloadShaders();

	void load(const std::string& directory);
	void load(fs::path directory, const std::string& baseDirectory = std::string());
	void load(std::vector<std::string> filenames, const std::string& baseDirectory = std::string());
	void load(std::vector<fs::path> filePaths, const std::string& baseDirectory = std::string());
	void load(std::map<std::string, std::string> dataMap, const std::string& baseDirectory = std::string());

private:
	std::map< std::string, std::unique_ptr<GlrShaderProgram> >				glrProgramMap_;
	std::map< std::string, std::unique_ptr<GlrShader> >                     glrShaderMap_;
	std::map< std::string, std::unique_ptr<GlslShaderProgram> >             glslProgramMap_;
	
	glw::IOpenGlDevice* openGlDevice_;
	
	std::vector<IShaderProgramBindListener*> defaultBindListeners_;

	std::unique_ptr<GlslShaderProgram> convertGlrProgramToGlslProgram(GlrShaderProgram* glrProgram) const;

	// A list of all of the directories that have had their shaders loaded
	std::vector< std::string > loadedShaderDirectories_;

	bool isShader(const std::string& source) const;
	bool isProgram(const std::string& source) const;
	bool isMisc(const std::string& source) const;
	
	// Used for searching for the 'type' of a shader / shader program file
	static std::string prepend_;
	static std::string append_;
};

}
}

#endif /* SHADERPROGRAMMANAGER_H_ */
