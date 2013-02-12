/*
 * ShaderProgramManager.cpp
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>

#include "ShaderProgramManager.h"

#include "ShaderData.h"
#include "ShaderManager.h"
#include "ShaderParserJson.h"
#include "Constants.h"

namespace fs = boost::filesystem;

namespace oglre {

namespace shaders {

ShaderProgramManager::ShaderProgramManager() {
	//loadStandardShaderPrograms();
}

ShaderProgramManager::~ShaderProgramManager() {
}

IShaderProgram* ShaderProgramManager::getShaderProgram(const std::string name) {
	return glslProgramMap_[ name ];
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
			if (fs::is_regular_file(dir_iter->status()) ) {
				filenames.push_back( dir_iter->file_string() );
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
			
			while ( getline(file, line) ) {
				contents += line;
			}
			
			file.close();
			
			dataMap[ filePaths[i].file_string() ] = contents;
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
			if (!oglreShaderMap_[entry.first]->containsPreProcessorCommands()) {
				glslShaderMap_[entry.first] = std::shared_ptr<GlslShader>( new GlslShader(entry.second) );
			}
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
		entry.second->process();
		
		glslProgramMap_[entry.first] = convertOglreProgramToGlslProgram( entry.second->get() );
	}
	
	// Compile each glsl shader program
	for (auto entry : glslProgramMap_) {
		entry.second->compile();
	}
}

std::unique_ptr<GlslShaderProgram> convertOglreProgramToGlslProgram( OglreShaderProgram* oglreProgram ) {
	auto oglreShaders = oglreProgram->getShaders();
	std::vector< std::shared_ptr<GlslShader> > glslShaders;
	
	for (auto entry : oglreShaders) {
		if (glslShaderMap_.find( entry.first ) == glslShaderMap_.end()) {
			// If GlslShader doesn't exist in the map, create it specifically for this shader program
			glslShaders.push_back( 
				std::shared_ptr<GlslShader>( 
					new GlslShader( 
						entry.second->getName(),
						entry.second->getProcessedSource(),
						entry.second->getType()
					) 
				)
			);
		} else {
			// Otherwise, use the already existing shader
			glslShaders.push_back( glslShaderMap_[entry.first] );
		}
	}
	
	return std::unique_ptr<GlslShaderProgram>( new GlslShaderProgram(oglreProgram->getName(), glslShaders) );
}

}

}
