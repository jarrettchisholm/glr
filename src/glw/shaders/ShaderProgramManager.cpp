/*
 * ShaderProgramManager.cpp
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#include <sstream>
#include <algorithm>
#include <boost/regex.hpp>

#include <boost/log/trivial.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>

#include "ShaderProgramManager.h"

#include "ShaderData.h"
#include "Constants.h"

namespace alg = boost::algorithm;

namespace glr {
namespace shaders {
ShaderProgramManager::ShaderProgramManager(bool autoLoad, std::vector<IShaderProgramBindListener*> defaultBindListeners)
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
	
	//loadStandardShaderPrograms();
	loadShaderPrograms(constants::SHADER_DIRECTORY);
}

IShaderProgram* ShaderProgramManager::getShaderProgram(const std::string name)
{
	return glslProgramMap_[ name ].get();
}

void ShaderProgramManager::loadShaderPrograms(const std::string directory)
{
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

void ShaderProgramManager::load(const std::string directory)
{
	return load(fs::path(directory));
}

void ShaderProgramManager::load(fs::path directory)
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

	return load(filenames);
}

void ShaderProgramManager::load(std::vector<std::string> filenames)
{
	std::vector<fs::path> filePaths;

	for ( glmd::uint32 i = 0; i < filenames.size(); i++ )
	{
		if ( fs::exists(filenames[i]) && !fs::is_directory(filenames[i]))
		{
			filePaths.push_back(fs::path(filenames[i]));
		}
	}

	return load(filePaths);
}

void ShaderProgramManager::load(std::vector<fs::path> filePaths)
{
	BOOST_LOG_TRIVIAL(debug) << "Reading shader programs from disk.";

	std::map<std::string, std::string> dataMap;

	// Load data from files and put into map
	for ( glmd::uint32 i = 0; i < filePaths.size(); i++ )
	{
		if ( fs::exists(filePaths[i]))
		{
			fs::ifstream file(filePaths[i]);
			std::string contents = "";

			std::string line;

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

	load(dataMap);
}

void ShaderProgramManager::load(std::map<std::string, std::string> dataMap)
{
	BOOST_LOG_TRIVIAL(debug) << "Loading shader programs.";

	// Add all shaders and shader programs to the maps
	for ( auto entry : dataMap )
	{
		if ( isShader(entry.second))
		{
			BOOST_LOG_TRIVIAL(debug) << "Shader: " << entry.first;
			std::shared_ptr<GlrShader> s = std::shared_ptr<GlrShader>(new GlrShader(entry.second));
			glrShaderMap_[entry.first] = s;
			// Add shader to glsl shader map if it doesn't have any preprocessor commands
			//if (!glrShaderMap_[entry.first]->containsPreProcessorCommands()) {
			//	glslShaderMap_[entry.first] = std::shared_ptr<GlslShader>( new GlslShader(entry.second) );
			//}
		}
		else if ( isProgram(entry.second))
		{
			BOOST_LOG_TRIVIAL(debug) << "Shader Program: " << entry.first;
			std::shared_ptr<GlrShaderProgram> sp = std::shared_ptr<GlrShaderProgram>(new GlrShaderProgram(entry.second));
			glrProgramMap_[entry.first] = sp;
		}
		else
		{
			// Error
			BOOST_LOG_TRIVIAL(error) << "ERROR loading shaders and shader programs.";
			// TODO: throw exception
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
		BOOST_LOG_TRIVIAL(debug) << "entry.second->getName(): " << entry.second->getName();
		glslProgramMap_[entry.second->getName()] = convertGlrProgramToGlslProgram(entry.second.get());
	}

	// Compile each glsl shader program and add the default set of bind listeners
	for ( auto entry : glslProgramMap_ )
	{
		entry.second->compile();

		for ( IShaderProgramBindListener* bindListener : defaultBindListeners_)
			entry.second->addBindListener( bindListener );
	}
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
						s->getBindings()
						)
					)
				);
		}
		else
		{
			// Otherwise, use the already existing shader
			glslShaders.push_back(glslShaderMap_[s->getName()]);
		}
	}

	return std::unique_ptr<GlslShaderProgram>(new GlslShaderProgram(glrProgram->getName(), glslShaders));
}

bool ShaderProgramManager::isShader(std::string s)
{
	return !isProgram(s);
}

bool ShaderProgramManager::isProgram(std::string s)
{	
	boost::regex shaderRegex(".*(\\s+)\\#type(\\s+)program(\\s*|\\s+\\n+.*)", boost::regex_constants::icase);

	return(boost::regex_match(s, shaderRegex));
}
}
}
