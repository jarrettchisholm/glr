/*
 * OglrePreProcessor.cpp
 * 
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 * 
 */


#include "OglrePreProcessor.h"

#include "ShaderData.h"

namespace oglre {

namespace shaders {

OglrePreProcessor::OglrePreProcessor(std::string source) : CPreProcessor(source) {
	CPreProcessor::files_ = SHADER_DATA;
}

}

}

