/*
 * GlrShader.cpp
 *
 * Copyright 2013 Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 *
 */

#include <boost/log/trivial.hpp>
#include <boost/regex.hpp>

#include "GlrShader.h"

#include "GlrPreProcessor.h"

namespace glr {
namespace shaders {

GlrShader::GlrShader(std::string source) : source_(source)
{
}

GlrShader::GlrShader(std::string name, std::string source) : name_(name), source_(source)
{
}


GlrShader::~GlrShader()
{
}

void GlrShader::process(std::map< std::string, std::string > defineMap)
{
	BOOST_LOG_TRIVIAL(debug) << "Processing shader '" << name_ << "'.";
	GlrPreProcessor pp(source_);
	pp.process(defineMap);
	processedSource_ = pp.getProcessedSource();
	type_ = IShader::parseType(pp.getType());

	GlrParser op(processedSource_);
	op.parse();
	bindings_ = op.getBindings();

	// Remove all '@bind' lines
	const boost::regex e("@bind(\\s+)(\\S+)\\n+");
	processedSource_ = boost::regex_replace(processedSource_, e, "\n");
	//std::cout << "processedSource_: " << processedSource_ << std::endl;
}

std::string GlrShader::getName()
{
	return name_;
}

IShader::Type GlrShader::getType()
{
	return type_;
}

std::string GlrShader::getProcessedSource()
{
	return processedSource_;
}

std::string GlrShader::getSource()
{
	return source_;
}

GlrParser::StringBindingsMap GlrShader::getBindings()
{
	return bindings_;
}

bool GlrShader::containsPreProcessorCommands()
{
	// TODO: implement
	return true;
}
}
}
