/*
 * ShaderManager.cpp
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#include <boost/log/trivial.hpp>

#include "ShaderManager.h"

#include "Constants.h"

namespace oglre {

namespace shaders {

ShaderManager::ShaderManager() {
}

ShaderManager::~ShaderManager() {
}

ShaderManager::ShaderManager(ShaderManager const&) {
}

ShaderManager* ShaderManager::shaderManager_ = 0;
ShaderManager* ShaderManager::getInstance() {
	if (ShaderManager::shaderManager_ == 0)
		ShaderManager::shaderManager_ = new ShaderManager();

	return ShaderManager::shaderManager_;
}

Shader* ShaderManager::getShader(const std::string filename) {
	BOOST_LOG_TRIVIAL(debug) << "Loading shader...";
	
	if (shaders_[filename] != 0) {
		BOOST_LOG_TRIVIAL(debug) << "Shader found.";
		return shaders_[filename].get();
	}
	
	std::string basepath = Constants::SHADER_DIRECTORY;
	
	BOOST_LOG_TRIVIAL(debug) << "Loading shader.";
	
    
    //if (image == 0) {
	//	BOOST_LOG_TRIVIAL(debug) << "Unable to load shaderProgram.";
	//	return 0;
	//}
    
    
	
	BOOST_LOG_TRIVIAL(debug) << "Creating shader.";
	shaders_[filename] = std::unique_ptr<Shader>( new Shader() );
	
	//delete image;
	
	return shaders_[filename].get();
}

}

}
