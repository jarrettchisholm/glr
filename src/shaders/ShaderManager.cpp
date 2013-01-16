/*
 * ShaderManager.cpp
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#include <boost/log/trivial.hpp>

#include "ShaderManager.h"

#include "../common/io/File.h"

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

Shader* ShaderManager::getShader(const std::string filename, const IShader::Type shaderType) {
	BOOST_LOG_TRIVIAL(debug) << "Loading shader...";
	
	if (shaders_[filename] != nullptr) {
		BOOST_LOG_TRIVIAL(debug) << "Shader found.";
		return shaders_[filename].get();
	}
	
	std::string filepath = constants::SHADER_DIRECTORY;
	filepath.append(filename);
	
	BOOST_LOG_TRIVIAL(debug) << "shader source filename: " << filename;
	BOOST_LOG_TRIVIAL(debug) << "shader source filepath: " << filepath;
	
	BOOST_LOG_TRIVIAL(debug) << "Loading shader.";
	
	std::string shaderContents = io::File::getFileContents(filepath);    
    
	
	BOOST_LOG_TRIVIAL(debug) << "Creating shader.";
	
	shaders_[filename] = std::unique_ptr<Shader>( new Shader(shaderContents, shaderType) );
	
	// error checking
	if (shaders_[filename]->initialize() < 0) {
		BOOST_LOG_TRIVIAL(debug) << "Unable to load shader.";
		shaders_.erase(filename);
		return nullptr;
	}
	
	return shaders_[filename].get();
}

}

}
