/*
 * Logger.cpp
 *
 *  Created on: 2011-01-17
 *      Author: jarrett
 */

#include "Logger.h"

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

namespace cs_logger {
	
Logger::Logger(std::string fileName)
{
	initialize(fileName);
}

Logger::Logger(Logger const&)
{
}

Logger::~Logger()
{

}

Logger* Logger::logger_ = nullptr;
Logger* Logger::getInstance(std::string logFile)
{
	if ( Logger::logger_ == nullptr )
		Logger::logger_ = new Logger(logFile);

	return Logger::logger_;
}

void Logger::initialize(std::string fileName)
{	
	logging::add_file_log( 
		fileName, 
		keywords::auto_flush = true,
		keywords::format = "[%TimeStamp%]: %Message%"
	);

    //logging::core::get()->set_filter
    //(
	//	logging::trivial::severity >= logging::trivial::info
    //);
	
	logging::add_common_attributes();
}

void Logger::logInfo(std::string message)
{
	BOOST_LOG_SEV(log_, info) << message;
}

void Logger::logDebug(std::string message)
{
	BOOST_LOG_SEV(log_, debug) << message;
}

void Logger::logWarn(std::string message)
{
	BOOST_LOG_SEV(log_, warning) << message;
}

void Logger::logError(std::string message)
{
	BOOST_LOG_SEV(log_, error) << message;
}

void Logger::logFatal(std::string message)
{
	BOOST_LOG_SEV(log_, fatal) << message;
}

}
