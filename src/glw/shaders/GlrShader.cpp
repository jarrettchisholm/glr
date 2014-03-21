#include <utility>

#include <boost/regex.hpp>

#include "glw/shaders/GlrShader.hpp"

#include "common/logger/Logger.hpp"

#include "glw/shaders/GlrPreProcessor.hpp"

namespace glr
{
namespace shaders
{

GlrShader::GlrShader(std::string source, std::string baseDirectory) : source_(std::move(source)), baseDirectory_(std::move(baseDirectory))
{
}

GlrShader::GlrShader(std::string name, std::string source, std::string baseDirectory) : name_(std::move(name)), source_(std::move(source)), baseDirectory_(std::move(baseDirectory))
{
}


GlrShader::~GlrShader()
{
}

void GlrShader::process(std::map< std::string, std::string > defineMap)
{
	LOG_DEBUG( "Processing shader '" + name_ + "'." );
	
	GlrPreProcessor pp(source_, baseDirectory_);
	pp.process(defineMap);
	
	processedSource_ = pp.getProcessedSource();
	type_ = IShader::parseType(pp.getType());

	GlrParser op(processedSource_);
	op.parse();
	bindBindings_ = op.getBindBindings();
	locationBindings_ = op.getLocationBindings();

	// Remove all '@bind' and '@location' lines
	{
		const boost::regex e("@bind(\\s+)(\\S+)\\n+");
		processedSource_ = boost::regex_replace(processedSource_, e, "\n");
	}
	{
		const boost::regex e("@location(\\s+)(\\S+)\\n+");
		processedSource_ = boost::regex_replace(processedSource_, e, "\n");
	}
}

const std::string& GlrShader::getName() const
{
	return name_;
}

IShader::Type GlrShader::getType() const
{
	return type_;
}

std::string GlrShader::getProcessedSource() const
{
	return processedSource_;
}

std::string GlrShader::getSource() const
{
	return source_;
}

std::vector< std::pair<std::string, std::string> > GlrShader::getBindings()
{
	return bindBindings_;
}

std::vector< std::pair<glmd::int32, std::string> > GlrShader::getLocationBindings()
{
	return locationBindings_;
}

bool GlrShader::containsPreProcessorCommands() const
{
	// TODO: implement
	return true;
}

}
}
