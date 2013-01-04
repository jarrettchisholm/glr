/*
 * ShaderProgramManager.cpp
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#include <boost/log/trivial.hpp>

#include "ShaderProgramManager.h"

#include "Constants.h"

namespace oglre {

namespace shaders {

ShaderProgramManager::ShaderProgramManager() {
}

ShaderProgramManager::~ShaderProgramManager() {
}

ShaderProgramManager::ShaderProgramManager(ShaderProgramManager const&) {
}

ShaderProgramManager* ShaderProgramManager::shaderProgramManager_ = 0;
ShaderProgramManager* ShaderProgramManager::getInstance() {
	if (ShaderProgramManager::shaderProgramManager_ == 0)
		ShaderProgramManager::shaderProgramManager_ = new ShaderProgramManager();

	return ShaderProgramManager::shaderProgramManager_;
}

ShaderProgram* ShaderProgramManager::getShaderProgram(const std::string filename) {
	BOOST_LOG_TRIVIAL(debug) << "Loading shaderProgram...";
	
	if (shaderPrograms_[filename] != 0) {
		BOOST_LOG_TRIVIAL(debug) << "ShaderProgram found.";
		return shaderPrograms_[filename].get();
	}
	
	std::string basepath = Constants::SHADER_DIRECTORY;
	
	BOOST_LOG_TRIVIAL(debug) << "Loading shaderProgram image.";
	
    
    //if (image == 0) {
	//	BOOST_LOG_TRIVIAL(debug) << "Unable to load shaderProgram.";
	//	return 0;
	//}
    
    
	
	BOOST_LOG_TRIVIAL(debug) << "Creating shaderProgram.";
	shaderPrograms_[filename] = std::unique_ptr<ShaderProgram>( new ShaderProgram() );
	
	//delete image;
	
	return shaderPrograms_[filename].get();
}

}

}
