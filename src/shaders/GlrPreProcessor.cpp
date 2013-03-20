/*
 * GlrPreProcessor.cpp
 * 
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 * 
 */


#include "GlrPreProcessor.h"

#include "ShaderData.h"

namespace glr {

namespace shaders {

GlrPreProcessor::GlrPreProcessor(std::string source) : CPreProcessor(source) {
	CPreProcessor::files_ = SHADER_DATA;
}

}

}

