#include "glw/shaders/CPreProcessor.hpp"

#include <vector>
#include <sstream>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

///////////////////////////////////////////////////////////////////////////////
//  Include Wave itself
#include <boost/wave.hpp>

///////////////////////////////////////////////////////////////////////////////
// Include the lexer stuff
#include <boost/wave/cpplexer/cpp_lex_token.hpp>    // token class
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp> // lexer class

#include "common/logger/Logger.hpp"

namespace glr
{
namespace shaders
{

std::map<std::string, std::string> CPreProcessor::files_ = std::map<std::string, std::string>();

CPreProcessor::CPreProcessor(std::string source, std::string baseDirectory) : source_(source), baseDirectory_(baseDirectory)
{
	name_ = "";
	type_ = "";
	shaderData_ = std::vector<ShaderData>();
	processedSource_ = std::string();
}

CPreProcessor::~CPreProcessor()
{
}

void CPreProcessor::process(std::map< std::string, std::string > defineMap)
{
	// current file position is saved for exception handling
	boost::wave::util::file_position_type current_position;

	//  The template boost::wave::cpplexer::lex_token<> is the token type to be
	//  used by the Wave library.
	typedef boost::wave::cpplexer::lex_token<> token_type;

	//  The template boost::wave::cpplexer::lex_iterator<> is the lexer type to
	//  be used by the Wave library.
	typedef boost::wave::cpplexer::lex_iterator<token_type> lex_iterator_type;

	//  This is the resulting context type to use. The first template parameter
	//  should match the iterator type to be used during construction of the
	//  corresponding context object (see below).
	typedef boost::wave::context<
			std::string::iterator,
			lex_iterator_type,
			CPreProcessor,
			CPreProcessor
			> context_type;

	//CPreProcessor hooks(files);

	// The preprocessor iterator shouldn't be constructed directly. It is
	// to be generated through a wave::context<> object. This wave:context<>
	// object additionally may be used to initialize and define different
	// parameters of the actual preprocessing (not done here).
	//
	// The preprocessing of the input stream is done on the fly behind the
	// scenes during iteration over the context_type::iterator_type stream.
	//context_type ctx (source.begin(), source.end(), "lex_infile", hooks);
	//std::cout << "processing: " << std::endl << source_ << std::endl;
	context_type ctx(source_.begin(), source_.end(), "lex_infile", *this);

	ctx.set_language(boost::wave::enable_long_long(ctx.get_language()));
	ctx.set_language(boost::wave::enable_preserve_comments(ctx.get_language()));
	ctx.set_language(boost::wave::enable_prefer_pp_numbers(ctx.get_language()));

	// analyze the input file, print out the preprocessed tokens
	context_type::iterator_type first = ctx.begin();
	context_type::iterator_type last = ctx.end();

	std::stringstream ss;

	try
	{
		while ( first != last )
		{
			current_position = (*first).get_position();
			ss << (*first).get_value();
			++first;
		}
	}
	catch ( boost::wave::cpp_exception const& e )
	{
		// some preprocessing error
		std::cerr
			<< e.file_name() << "(" << e.line_no() << "): "
			<< e.description() << std::endl;
		return;
	}
	catch ( std::exception const& e )
	{
		// use last recognized token to retrieve the error position
		std::cerr
			<< current_position.get_file()
			<< "(" << current_position.get_line() << "): "
			<< "exception caught: " << e.what()
			<< std::endl;
		return;
	}
	catch ( ... )
	{
		// use last recognized token to retrieve the error position
		std::cerr
			<< current_position.get_file()
			<< "(" << current_position.get_line() << "): "
			<< "unexpected exception caught." << std::endl;
		return;
	}

	processedSource_ = ss.str();
	name_ = ctx.get_hooks().getName();
	shaderData_ = ctx.get_hooks().getShaders();
	type_ = ctx.get_hooks().getType();
}

std::string CPreProcessor::getName()
{
	return name_;
}

std::string CPreProcessor::getType()
{
	return type_;
}

std::vector<CPreProcessor::ShaderData> CPreProcessor::getShaders()
{
	return shaderData_;
}

std::string CPreProcessor::getSource()
{
	return source_;
}

std::string CPreProcessor::getProcessedSource()
{
	return processedSource_;
}


template <typename ContextT, typename ContainerT>
bool CPreProcessor::found_unknown_directive(ContextT const& ctx, ContainerT const& line, ContainerT& pending)
{
	typedef typename ContainerT::const_iterator iterator_type;
	iterator_type it = line.begin();
	wave::token_id id = wave::util::impl::skip_whitespace(it, line.end());

	if ( id != wave::T_IDENTIFIER )
		return false;                                                                                                                                                                                                                                                         // nothing we could do

	if ((*it).get_value() == "version" || (*it).get_value() == "extension" )
	{
		// Handle #version and #extension directives
		std::copy(line.begin(), line.end(), std::back_inserter(pending));
		return true;
	}

	if ((*it).get_value() == "type" )
	{
		// Handle type directive
		typedef typename ContextT::token_type result_type;
		for ( result_type t : line )
		{
			const char* text = t.get_value().c_str();
			if ( strcmp("#", text) != 0 && strcmp("type", text) != 0 && strcmp("na", text) != 0 )
			{
				type_ += t.get_value().c_str();
			}
		}

		alg::trim(type_);
		//std::cout << type_ << std::endl;

		return true;
	}

	if ((*it).get_value() == "name")
	{
		// Once the name is set, we ignore #name directives
		if ( name_.size() == 0 )
		{
			// Handle name directive
			typedef typename ContextT::token_type result_type;
			for ( result_type t : line )
			{
				const char* text = t.get_value().c_str();
				if ( strcmp("#", text) != 0 && strcmp("name", text) != 0 )
				{
					name_ += t.get_value().c_str();
				}
			}
			
			alg::trim(name_);
		}
		
		//std::cout << name_ << std::endl;

		return true;
	}

	if ((*it).get_value() == "bind" )
	{
		// Handle bind directive

		//typedef typename ContextT::token_type result_type;
		//for ( result_type t : line) {
		//	std::cout << t.get_value() << std::endl;
		//}

		std::copy(line.begin(), line.end(), std::back_inserter(pending));
		return true;
	}

	// Unknown directive
	return false;
}


template <typename ContextT, typename ContainerT>
bool CPreProcessor::emit_line_directive(ContextT const& ctx, ContainerT&pending, typename ContextT::token_type const& act_token)
{
	//std::cout << "in emit_line_directive" << std::endl;

	// emit a #line directive showing the relative filename instead
	typename ContextT::position_type pos = act_token.get_position();
	unsigned int column = 1;

	typedef typename ContextT::token_type result_type;

	pos.set_column(column);

	std::string comments = "// From file: ";
	pending.push_back(result_type(wave::T_STRINGLIT, comments.c_str(), pos));
	pos.set_column(column += (unsigned int)comments.size());                                // account for comments

	std::string file("");
	boost::filesystem::path filename(wave::util::create_path(ctx.get_current_filename().c_str()));

	file += boost::wave::util::impl::escape_lit(wave::util::native_file_string(filename)) + "";

	pending.push_back(result_type(wave::T_STRINGLIT, file.c_str(), pos));
	pos.set_column(column += (unsigned int)file.size());                                // account for filename
	pending.push_back(result_type(wave::T_GENERATEDNEWLINE, "\n", pos));

	return true;
}

#if BOOST_WAVE_USE_DEPRECIATED_PREPROCESSING_HOOKS != 0
void CPreProcessor::opened_include_file(std::string const&relname, std::string const&filename, std::size_t /*include_depth*/, bool is_system_include)
#else
template <typename ContextT>
void CPreProcessor::opened_include_file(ContextT const& ctx, std::string const& relname, std::string const& filename, bool is_system_include)
#endif
{
	//std::cout << "opened_include_file: " << "relname: " << relname << " filename: " << filename << " is_system_include: " << is_system_include << std::endl;
}

#if BOOST_WAVE_USE_DEPRECIATED_PREPROCESSING_HOOKS != 0
void CPreProcessor::returning_from_include_file()
#else
template <typename ContextT>
void CPreProcessor::returning_from_include_file(ContextT const& ctx)
#endif
{
	//--include_depth;
}

#if BOOST_WAVE_USE_DEPRECIATED_PREPROCESSING_HOOKS != 0
// old signature
void CPreProcessor::found_include_directive(std::string const& filename, bool include_next)
{
}
#else
// new signature
template <typename ContextT>
bool CPreProcessor::found_include_directive(ContextT const& ctx, std::string const& filename, bool include_next)
{
	//std::cout << "found_include_directive:" << filename << std::endl;

	boost::regex systemIncludeRegex("<.*>");

	if ( !boost::regex_match(filename, systemIncludeRegex))
	{
		std::string editedFilename = filename;
		// remove any quotations around the filename
		boost::algorithm::erase_all(editedFilename, "\"");
		
		std::stringstream msg;
		//msg << "adding: " << editedFilename << " size: " << shaderData_.size();
		LOG_DEBUG( msg.str() );

		shaderData_.push_back(ShaderData(editedFilename));
	}

	return false;                                // ok to include this file
}
#endif


template <typename ContextT>
bool CPreProcessor::locate_include_file(ContextT& ctx, std::string& file_path, bool is_system, char const* current_name, std::string& dir_path, std::string& native_name)
{
	//if (is_system) {
	// Check if file is in the files map	
	if ( CPreProcessor::files_.find(file_path) != CPreProcessor::files_.end())
	{
		//std::cout << "locate_include_file 1: file_path:" << file_path << " dir_path:" << dir_path << " native_name:" << native_name << std::endl;
		native_name = file_path;

		return true;
	}
	else
	{
		namespace fs = boost::filesystem;
		//} else {
		// could not locate file
		
		fs::path baseDir = fs::path(baseDirectory_);
		if ( fs::exists(baseDir) && fs::is_directory(baseDir))
		{
			file_path = baseDir.string() + std::string("/") + file_path;
		}
		//std::cout << "baseDirectory_: " << baseDirectory_ << std::endl;
		//std::cout << "locate_include_file doesn't exist 2: file_path:" << file_path << " dir_path:" << dir_path << " native_name:" << native_name << std::endl;
		if ( !ctx.find_include_file(file_path, dir_path, is_system, current_name))
			return false;
		

		fs::path native_path(wave::util::create_path(file_path));
		if ( !fs::exists(native_path))
		{
			//BOOST_WAVE_THROW_CTX(ctx, preprocess_exception, bad_include_file,
			//    file_path.c_str(), ctx.get_main_pos());
			//std::cout << "error: doesn't exist" << std::endl;
			//std::cout << "locate_include_file doesn't exist 3: file_path:" << file_path << " dir_path:" << dir_path << " native_name:" << native_name << std::endl;
			return false;
		}

		// return the unique full file system path of the located file
		native_name = wave::util::native_file_string(native_path);

		return true;
	}

	// include file has not been located
	return false;
}

}
}
