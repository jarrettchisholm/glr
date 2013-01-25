/*
 * ShaderParserJson.h
 * 
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 * 
 * 
 */

#include "IShaderProgram.h"

#ifndef SHADERPARSERJSON_H
#define SHADERPARSERJSON_H

namespace oglre {

namespace shaders {

class ShaderParserJson {
public:
	ShaderParserJson(const std::string data);
	virtual ~ShaderParserJson();
	
	std::vector<ShaderProgramInfo> getShaderProgramInfoList();
	
private:
	std::vector<ShaderProgramInfo> shaderProgramInfo_;

	void parseData(const std::string data);
};

}

}
#endif /* SHADERPARSERJSON_H */ 
