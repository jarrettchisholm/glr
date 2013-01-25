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

Shader* ShaderManager::getShader(ShaderInfo shaderInfo) {
	BOOST_LOG_TRIVIAL(debug) << "Loading shader...";
	
	if (shaderInfo.contents.length() == 0) {
		BOOST_LOG_TRIVIAL(error) << "Unable to load shader - Shader has no contents.";
		return nullptr;
	}
	
	BOOST_LOG_TRIVIAL(debug) << "Creating shader.";
	
	shaders_[shaderInfo.name] = std::unique_ptr<Shader>( new Shader(shaderInfo.contents, shaderInfo.type) );
	
	// error checking
	if (shaders_[shaderInfo.name]->initialize() < 0) {
		BOOST_LOG_TRIVIAL(debug) << "Unable to load shader.";
		shaders_.erase(shaderInfo.name);
		return nullptr;
	}
	
	return shaders_[shaderInfo.name].get();
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
