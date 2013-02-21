/*
 * ShaderProgramManager.cpp
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#include <sstream>
#include <regex>

#include <boost/log/trivial.hpp>
#include <boost/filesystem/fstream.hpp>

#include "ShaderProgramManager.h"

#include "ShaderData.h"
//#include "ShaderManager.h"
//#include "ShaderParserJson.h"
#include "Constants.h"

namespace oglre {

namespace shaders {

ShaderProgramManager::ShaderProgramManager() {
	//loadStandardShaderPrograms();
}

ShaderProgramManager::~ShaderProgramManager() {
}

IShaderProgram* ShaderProgramManager::getShaderProgram(const std::string name) {
	return glslProgramMap_[ name ].get();
}

void ShaderProgramManager::loadShaderPrograms(const std::string directory) {
	load( directory );
}

void ShaderProgramManager::load(const std::string directory) {
	return load( fs::path(directory) );
}

void ShaderProgramManager::load(fs::path directory) {
	std::vector<std::string> filenames;
	
	// find all of the files
	fs::directory_iterator end_iter;
	if ( fs::exists(directory) && fs::is_directory(directory)) {
		for( fs::directory_iterator dir_iter(directory) ; dir_iter != end_iter ; ++dir_iter) {
			if ( fs::is_regular_file(dir_iter->status()) ) {
				std::stringstream temp;
				temp << dir_iter->path().filename();
				filenames.push_back( temp.str() );
			}
		}
	}
	
	return load( filenames );
}

void ShaderProgramManager::load(std::vector<std::string> filenames) {
	std::vector<fs::path> filePaths;
	
	for (int i=0; i < filenames.size(); i++) {
		if (fs::exists( filenames[i] ) && !fs::is_directory( filenames[i] ))
			filePaths.push_back( fs::path(filenames[i]) );
	}
	
	return load( filePaths);
}

void ShaderProgramManager::load(std::vector<fs::path> filePaths) {
	BOOST_LOG_TRIVIAL(debug) << "Reading shader programs from disk.";
	
	std::map<std::string, std::string> dataMap;
	
	// Load data from files and put into map
	for (int i=0; i < filePaths.size(); i++) {
		if (fs::exists( filePaths[i] )) {
			fs::ifstream file( filePaths[i] );
			std::string contents = "";
			
			std::string line;
			
			while ( getline(file, line) ) {
				contents += line;
			}
			
			file.close();
			
			std::stringstream temp;
			temp << filePaths[i].filename();
			dataMap[ temp.str() ] = contents;
		}
	}
	
	load( dataMap );
}

void ShaderProgramManager::load(std::map<std::string, std::string> dataMap) {
	BOOST_LOG_TRIVIAL(debug) << "Loading shader programs.";
	
	// Add all shaders and shader programs to the maps
	for (auto entry : dataMap) {
		if ( isShader(entry.second) ) {
			oglreShaderMap_[entry.first] = std::shared_ptr<OglreShader>( new OglreShader(entry.second) );
			// Add shader to glsl shader map if it doesn't have any preprocessor commands
			//if (!oglreShaderMap_[entry.first]->containsPreProcessorCommands()) {
			//	glslShaderMap_[entry.first] = std::shared_ptr<GlslShader>( new GlslShader(entry.second) );
			//}
		} else if ( isProgram(entry.second) ) {
			oglreProgramMap_[entry.first] = std::unique_ptr<OglreShaderProgram>( new OglreShaderProgram(entry.second) );
		} else {
			// Error
			BOOST_LOG_TRIVIAL(error) << "ERROR loading shaders and shader programs";
		}
	}
	
	// Compile all glsl shaders
	//for (auto entry : glslShaderMap_) {
	//	entry.second->compile();
	//}
	
	// Process each oglre shader program
	for (auto entry : oglreProgramMap_) {
		entry.second->process(oglreShaderMap_);
		
		glslProgramMap_[entry.first] = convertOglreProgramToGlslProgram( entry.second.get() );
	}
	
	// Compile each glsl shader program
	for (auto entry : glslProgramMap_) {
		entry.second->compile();
	}
}

std::unique_ptr<GlslShaderProgram> ShaderProgramManager::convertOglreProgramToGlslProgram( OglreShaderProgram* oglreProgram ) {
	auto oglreShaders = oglreProgram->getShaders();
	std::vector< std::shared_ptr<GlslShader> > glslShaders;
	
	for (auto s : oglreShaders) {
		if (glslShaderMap_.find( s->getName() ) == glslShaderMap_.end()) {
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
		} else {
			// Otherwise, use the already existing shader
			glslShaders.push_back( glslShaderMap_[s->getName()] );
		}
	}
	
	return std::unique_ptr<GlslShaderProgram>( new GlslShaderProgram(oglreProgram->getName(), glslShaders) );
}

bool ShaderProgramManager::isShader( std::string s ) {
	return !isProgram(s);
}

bool ShaderProgramManager::isProgram( std::string s ) {
	std::regex shaderRegex("\\#type(\\s+)program", std::regex_constants::icase);
	
	return ( std::regex_match(s, shaderRegex) );
}

}

}
