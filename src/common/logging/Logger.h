/*
 * Logger.h
 *
 *  Created on: 2011-01-17
 *      Author: jarrett
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

using namespace logging::trivial;

#ifndef DEBUG
#define LOG_INFO(x)
#define LOG_DEBUG(x)
#define LOG_WARN(x)
#define LOG_ERROR(x)
#define LOG_FATAL(x)
#else
#define LOG_INFO(x) cs_logger::Logger::getInstance()->logInfo( x );
#define LOG_DEBUG(x) cs_logger::Logger::getInstance()->logDebug( x );
#define LOG_WARN(x) cs_logger::Logger::getInstance()->logWarn( x );
#define LOG_ERROR(x) cs_logger::Logger::getInstance()->logError( x );
#define LOG_FATAL(x) cs_logger::Logger::getInstance()->logFatal( x );
#endif

namespace cs_logger {

class Logger
{
	
public:
	static Logger* getInstance(std::string logFile = "default.log");
	
	void logInfo(std::string message);
	void logDebug(std::string message);
	void logWarn(std::string message);
	void logError(std::string message);
	void logFatal(std::string message);
	

private:
	Logger(std::string fileName);
	Logger(Logger const&);
	Logger& operator=(Logger const&);
	virtual ~Logger();

	void initialize(std::string fileName);

	src::severity_logger< severity_level > log_;

	static Logger* logger_; // singleton instance
};

}
#endif /* LOGGER_H_ */
