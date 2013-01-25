/*
 * ShaderParserJson.cpp
 * 
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 * 
 * 
 */
#include <boost/log/trivial.hpp>

#include "jsoncpp/json/json.h"

#include "ShaderParserJson.h"

#include "Shader.h"
#include "../exceptions/FormatException.h"

namespace oglre {

namespace shaders {

ShaderParserJson::ShaderParserJson(const std::string data) {
	parseData(data);
}


ShaderParserJson::~ShaderParserJson() {	
}

void ShaderParserJson::parseData(const std::string data) {
	Json::Value root;
	Json::Reader reader;
	
	bool parsingSuccessful = reader.parse( data, root );
	
	if ( !parsingSuccessful ) {
		std::string error = "Failed to parse configuration: " + reader.getFormattedErrorMessages();
		BOOST_LOG_TRIVIAL(error) << error;
		throw exception::FormatException(error);
	}
	
	// Load shader data
	Json::Value shaders = root["shaders"];
	std::map<std::string, ShaderInfo> shaderMap;
	for ( unsigned int i = 0; i < shaders.size(); i++ ) {
		ShaderInfo si;
		si.name = shaders[i]["name"].asString();
		si.contents = shaders[i]["contents"].asString();
		si.type = Shader::parseType( shaders[i]["type"].asString() );
		shaderMap[si.name] = si;
	}
	
	// Load shader program data
	Json::Value programs = root["programs"];
	for ( unsigned int i = 0; i < programs.size(); i++ ) {
		ShaderProgramInfo spi;
		spi.name = programs[i]["name"].asString();
		
		shaders = programs[i]["shaders"];
		for ( unsigned int i = 0; i < shaders.size(); i++ ) {
			// If shader is found in the map, add it; otherwise, throw an exception
			if ( shaderMap.find(shaders[i]["name"].asString()) != shaderMap.end() ) {
				spi.shaderInfoList.push_back( shaderMap[shaders[i]["name"].asString()] );
			} else {
				std::string error = "Failed to find shader listed in shader program.";
				BOOST_LOG_TRIVIAL(error) << error;
				throw exception::FormatException(error);
			}
		}
		
		shaderProgramInfo_.push_back( spi );
	}
	
}

std::vector<ShaderProgramInfo> ShaderParserJson::getShaderProgramInfoList() {
	return shaderProgramInfo_;
}

}

}

