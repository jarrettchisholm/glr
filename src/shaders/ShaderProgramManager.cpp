/*
 * ShaderProgramManager.cpp
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#include <boost/log/trivial.hpp>

#include "ShaderProgramManager.h"

#include "ShaderManager.h"
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

ShaderProgram* ShaderProgramManager::getShaderProgram(const std::string name) {
	BOOST_LOG_TRIVIAL(debug) << "Loading shaderProgram: " << name;
	
	if (shaderPrograms_[name] != nullptr) {
		BOOST_LOG_TRIVIAL(debug) << "ShaderProgram found.";
		return shaderPrograms_[name].get();
	}
	else {
		BOOST_LOG_TRIVIAL(debug) << "Unable to find ShaderProgram: " << name;
		return nullptr;
	}
}

ShaderProgram* ShaderProgramManager::getShaderProgram(const std::string name, std::vector< std::pair <std::string, shaders::IShader::Type> > shaderInfo) {
	BOOST_LOG_TRIVIAL(debug) << "Loading ShaderProgram...";
	
	if (shaderPrograms_[name] != nullptr) {
		BOOST_LOG_TRIVIAL(debug) << "ShaderProgram found.";
		return shaderPrograms_[name].get();
	}
	
	std::vector<Shader*> shaders;
	
	for (int i=0; i < shaderInfo.size(); i++) {		
		Shader* shader = ShaderManager::getInstance()->getShader(shaderInfo[i].first, shaderInfo[i].second);
		
		if (shader != nullptr)
			shaders.push_back(shader);
	}
	
	
	BOOST_LOG_TRIVIAL(debug) << "Loading ShaderProgram.";
	
	shaderPrograms_[name] = std::unique_ptr<ShaderProgram>( new ShaderProgram(shaders) );
	
	// error checking
	if (shaderPrograms_[name]->initialize() < 0) {
		BOOST_LOG_TRIVIAL(debug) << "Unable to load ShaderProgram.";
		shaderPrograms_.erase(name);
		return nullptr;
	}
	
	return shaderPrograms_[name].get();
}

}

}
