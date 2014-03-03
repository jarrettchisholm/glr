#include <sstream>
#include <algorithm>
#include <boost/regex.hpp>

#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>

#include "common/logger/Logger.hpp"

#include "exceptions/FormatException.hpp"

#include "glw/shaders/ShaderProgramManager.hpp"

#include "glw/shaders/ShaderData.hpp"
#include "glw/shaders/Constants.hpp"

namespace glr
{
namespace shaders
{

namespace alg = boost::algorithm;

ShaderProgramManager::ShaderProgramManager(glw::IOpenGlDevice* openGlDevice, bool autoLoad, std::vector<IShaderProgramBindListener*> defaultBindListeners) : openGlDevice_(openGlDevice)
{
	defaultBindListeners_ = defaultBindListeners;
	
	if ( autoLoad )
		loadStandardShaderPrograms();
}

ShaderProgramManager::~ShaderProgramManager()
{
}

void ShaderProgramManager::loadStandardShaderPrograms()
{
	load(SHADER_DATA);
}

void ShaderProgramManager::reloadShaders()
{
	glrProgramMap_.clear();
	glrShaderMap_.clear();
	glslProgramMap_.clear();
	glslShaderMap_.clear();
	
	loadStandardShaderPrograms();
	//loadShaderPrograms(constants::SHADER_DIRECTORY);
	for ( auto& dir : loadedShaderDirectories_ )
		load( dir );
}

IShaderProgram* ShaderProgramManager::getShaderProgram(const std::string& name)
{
	return glslProgramMap_[ name ].get();
}

void ShaderProgramManager::loadShaderPrograms(const std::string& directory)
{
	loadedShaderDirectories_.push_back( directory );
	load(directory);
}

void ShaderProgramManager::addDefaultBindListener(IShaderProgramBindListener* bindListener)
{
	defaultBindListeners_.push_back(bindListener);
	
	// Add the new bind listener to all glsl programs
	for ( auto entry : glslProgramMap_ )
	{
		entry.second->addBindListener( bindListener );
	}
}

void ShaderProgramManager::removeDefaultBindListener(IShaderProgramBindListener* bindListener)
{
	auto it = std::find(defaultBindListeners_.begin(), defaultBindListeners_.end(), bindListener);
	
	if (it != defaultBindListeners_.end())
		defaultBindListeners_.erase(it);
		
	// Remove the bind listener from all glsl programs
	for ( auto entry : glslProgramMap_ )
	{
		entry.second->removeBindListener( bindListener );
	}
}

void ShaderProgramManager::removeAllDefaultBindListeners()
{
	defaultBindListeners_.clear();
}

void ShaderProgramManager::load(const std::string& directory)
{
	return load( fs::path(directory), directory );
}

void ShaderProgramManager::load(fs::path directory, std::string baseDirectory)
{
	std::vector<std::string> filenames;

	// find all of the files
	fs::directory_iterator end_iter;

	if ( fs::exists(directory) && fs::is_directory(directory))
	{
		for ( fs::directory_iterator dir_iter(directory); dir_iter != end_iter; ++dir_iter )
		{
			if ( fs::is_regular_file(dir_iter->status()))
			{
				filenames.push_back( dir_iter->path().string() );
			}
		}
	}

	return load( filenames, baseDirectory );
}

void ShaderProgramManager::load(std::vector<std::string> filenames, std::string baseDirectory)
{
	std::vector<fs::path> filePaths;

	for ( glmd::uint32 i = 0; i < filenames.size(); i++ )
	{
		if ( fs::exists(filenames[i]) && !fs::is_directory(filenames[i]))
		{
			filePaths.push_back(fs::path(filenames[i]));
		}
	}

	return load( filePaths, baseDirectory );
}

void ShaderProgramManager::load(std::vector<fs::path> filePaths, std::string baseDirectory)
{
	LOG_DEBUG( "Reading shader programs from disk." );

	std::map<std::string, std::string> dataMap;

	// Load data from files and put into map
	for ( glmd::uint32 i = 0; i < filePaths.size(); i++ )
	{
		if ( fs::exists(filePaths[i]))
		{
			fs::ifstream file(filePaths[i]);
			std::string contents = "";

			std::string line = std::string();

			while ( getline(file, line))
			{
				contents += line + '\n';
			}

			file.close();

			// Strip out '"' characters in filename (for some reason fs::path includes '"' characters around the filename)
			std::stringstream temp;
			temp << filePaths[i].filename();
			std::string tempStr = temp.str();
			std::replace( tempStr.begin(), tempStr.end(), '"', ' ');
			alg::trim(tempStr);
			
			dataMap[ tempStr ] = contents;
		}
	}

	load( dataMap, baseDirectory );
}

void ShaderProgramManager::load(std::map<std::string, std::string> dataMap, std::string baseDirectory)
{
	LOG_DEBUG( "Loading shader programs." );

	// Add all shaders and shader programs to the maps
	for ( auto entry : dataMap )
	{
		bool isShader = this->isShader(entry.second);
		bool isMisc = this->isMisc(entry.second);
		if ( isShader || isMisc )
		{
			std::string type = std::string("Shader");
			if (isMisc)
				type = std::string("Shader include");

			LOG_DEBUG( type + " found: " + entry.first);
			std::shared_ptr<GlrShader> s = std::shared_ptr<GlrShader>(new GlrShader(entry.first, entry.second, baseDirectory));
			glrShaderMap_[entry.first] = s;
			// Add shader to glsl shader map if it doesn't have any preprocessor commands
			//if (!glrShaderMap_[entry.first]->containsPreProcessorCommands()) {
			//	glslShaderMap_[entry.first] = std::shared_ptr<GlslShader>( new GlslShader(entry.second) );
			//}
		}
		else if ( isProgram(entry.second) )
		{
			LOG_DEBUG( "Shader program found: " + entry.first );
			std::shared_ptr<GlrShaderProgram> sp = std::shared_ptr<GlrShaderProgram>(new GlrShaderProgram(entry.first, entry.second, baseDirectory));
			glrProgramMap_[entry.first] = sp;
		}
		else
		{
			// Error
			std::string msg = std::string("Unknown shader / shader program type for file: ") + entry.first;
			LOG_ERROR( msg );
			LOG_ERROR( entry.second );
			
			// Cleanup
			glrShaderMap_.clear();
			glrProgramMap_.clear();
			
			throw exception::FormatException( msg );
		}
	}

	// Compile all glsl shaders
	//for (auto entry : glslShaderMap_) {
	//	entry.second->compile();
	//}

	// Process each glr shader program
	for ( auto entry : glrProgramMap_ )
	{
		entry.second->process(glrShaderMap_);
		LOG_DEBUG( "entry.second->getName(): " + entry.second->getName() );
		glslProgramMap_[entry.second->getName()] = convertGlrProgramToGlslProgram(entry.second.get());
	}

	// Compile each glsl shader program and add the default set of bind listeners
	for ( auto entry : glslProgramMap_ )
	{
		entry.second->compile();

		for ( IShaderProgramBindListener* bindListener : defaultBindListeners_)
			entry.second->addBindListener( bindListener );
	}
	
	// Clear out the temporary Glr shader / shader program maps
	glrProgramMap_.clear();
	glrShaderMap_.clear();
}

std::unique_ptr<GlslShaderProgram> ShaderProgramManager::convertGlrProgramToGlslProgram(GlrShaderProgram* glrProgram)
{
	auto glrShaders = glrProgram->getShaders();

	std::vector< std::shared_ptr<GlslShader> > glslShaders;

	for ( auto s : glrShaders )
	{
		if ( glslShaderMap_.find(s->getName()) == glslShaderMap_.end())
		{
			// If GlslShader doesn't exist in the map, create it specifically for this shader program
			glslShaders.push_back(
				std::shared_ptr<GlslShader>(
					new GlslShader(
						s->getName(),
						s->getProcessedSource(),
						s->getType(),
						s->getBindings(),
						s->getLocationBindings()
						)
					)
				);
		}
		else
		{
			// Otherwise, use the already existing shader
			glslShaders.push_back( glslShaderMap_[s->getName()] );
		}
	}

	return std::unique_ptr<GlslShaderProgram>(new GlslShaderProgram(glrProgram->getName(), glslShaders, openGlDevice_));
}

//std::string ShaderProgramManager::prepend_ = std::string(".*(\\s+)\\#type(\\s+)");
//std::string ShaderProgramManager::append_ = std::string("(\\s*|\\s+\\n+.*)");
std::string ShaderProgramManager::prepend_ = std::string(".*\\#type(\\s+)");
std::string ShaderProgramManager::append_ = std::string("(\\s*|\\s*\\n+.*)");

bool ShaderProgramManager::isShader(std::string s)
{
	
	
	boost::regex vertexShaderRegex(ShaderProgramManager::prepend_ + "vertex" + ShaderProgramManager::append_, boost::regex_constants::icase);
	boost::regex fragmentShaderRegex(ShaderProgramManager::prepend_ + "fragment" + ShaderProgramManager::append_, boost::regex_constants::icase);
	boost::regex tessellationShaderRegex(ShaderProgramManager::prepend_ + "tessellation" + ShaderProgramManager::append_, boost::regex_constants::icase);
	boost::regex geometryShaderRegex(ShaderProgramManager::prepend_ + "geometry" + ShaderProgramManager::append_, boost::regex_constants::icase);

	bool matches = boost::regex_match(s, vertexShaderRegex) 
					|| boost::regex_match(s, fragmentShaderRegex) 
					|| boost::regex_match(s, tessellationShaderRegex) 
					|| boost::regex_match(s, geometryShaderRegex)
	;

	return matches;
	//return !isProgram(s);
}

bool ShaderProgramManager::isProgram(std::string s)
{	
	boost::regex shaderRegex(ShaderProgramManager::prepend_ + "program" + ShaderProgramManager::append_, boost::regex_constants::icase);

	return(boost::regex_match(s, shaderRegex));
}

bool ShaderProgramManager::isMisc(std::string s)
{	
	boost::regex includeRegex(ShaderProgramManager::prepend_ + "na" + ShaderProgramManager::append_, boost::regex_constants::icase);

	return(boost::regex_match(s, includeRegex));
}

}
}
