/*
 * ShaderProgramManager.cpp
 *
 *  Created on: 2013-01-04
 *      Author: jarrett
 */

#include <boost/log/trivial.hpp>

#include "ShaderProgramManager.h"

#include "ShaderData.h"
#include "ShaderManager.h"
#include "ShaderParserJson.h"
#include "Constants.h"

namespace oglre {

namespace shaders {

ShaderProgramManager::ShaderProgramManager() {
	loadStandardShaderPrograms();
}

ShaderProgramManager::~ShaderProgramManager() {
}

ShaderProgramManager::ShaderProgramManager(ShaderProgramManager const&) {
}

ShaderProgramManager* ShaderProgramManager::shaderProgramManager_ = 0;
ShaderProgramManager* ShaderProgramManager::getInstance() {
	if (ShaderProgramManager::shaderProgramManager_ == nullptr)
		ShaderProgramManager::shaderProgramManager_ = new ShaderProgramManager();

	return ShaderProgramManager::shaderProgramManager_;
}

void ShaderProgramManager::loadStandardShaderPrograms() {
	loadShaderProgramsFromData( SHADER_DATA );
}

void ShaderProgramManager::loadShaderProgramsFromData(const std::string data) {
	BOOST_LOG_TRIVIAL(debug) << "Loading shader programs from data: " << data;
	
	ShaderParserJson parser(data);
	
	//std::vector<ShaderInfo> shaders = parser.getShaderInfoList();
	std::vector<ShaderProgramInfo> shaderPrograms = parser.getShaderProgramInfoList();
	
	BOOST_LOG_TRIVIAL(debug) << "Found " << shaderPrograms.size() << " shader programs.";
	
	// Load all of the shaders
	//for (ShaderInfo si : shaders)
	//	ShaderManager->getInstance()->getShaderFromData(si);
		
	// Load all of the shader programs
	for (ShaderProgramInfo spi : shaderPrograms)
		this->getShaderProgram(spi);
	
}

void ShaderProgramManager::loadShaderPrograms(const std::string filename) {
	BOOST_LOG_TRIVIAL(debug) << "Loading shader programs from file: " << filename;
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

ShaderProgram* ShaderProgramManager::getShaderProgram(ShaderProgramInfo shaderProgramInfo) {
	BOOST_LOG_TRIVIAL(debug) << "Loading ShaderProgram...";
	
	if (shaderProgramInfo.shaderInfoList.size() == 0) {
		BOOST_LOG_TRIVIAL(debug) << "Unable to load ShaderProgram - shader program has no shaders listed.";
		return nullptr;
	}
	
	std::vector<Shader*> shaders;
	
	std::vector<ShaderInfo> shaderInfoList = shaderProgramInfo.shaderInfoList;
	
	for (int i=0; i < shaderInfoList.size(); i++) {		
		Shader* shader = ShaderManager::getInstance()->getShader(shaderInfoList[i]);
		
		if (shader != nullptr)
			shaders.push_back(shader);
	}
	
	BOOST_LOG_TRIVIAL(debug) << "Loading ShaderProgram.";
	
	shaderPrograms_[shaderProgramInfo.name] = std::unique_ptr<ShaderProgram>( new ShaderProgram(shaders) );
	
	// error checking
	if (shaderPrograms_[shaderProgramInfo.name]->initialize() < 0) {
		BOOST_LOG_TRIVIAL(debug) << "Unable to load ShaderProgram.";
		shaderPrograms_.erase(shaderProgramInfo.name);
		return nullptr;
	}
	
	return shaderPrograms_[shaderProgramInfo.name].get();
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
