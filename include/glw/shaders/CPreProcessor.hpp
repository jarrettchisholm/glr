#ifndef CPREPROCESSOR_H
#define CPREPROCESSOR_H

#if !defined(BOOST_WAVE_CUSTOM_DIRECTIVES_HOOKS_INCLUDED)
#define BOOST_WAVE_CUSTOM_DIRECTIVES_HOOKS_INCLUDED

#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>
#include <algorithm>
#include <map>
#include <utility>

#include <boost/algorithm/string.hpp>

#include <boost/assert.hpp>
#include <boost/config.hpp>

// Need to include this before other wave/phoenix includes
// @see https://groups.google.com/forum/#!msg/boost-list/3xZDWUyTJG0/IEF2wTy1EIsJ
#include <boost/phoenix/core/limits.hpp>

#include <boost/wave/token_ids.hpp>
#include <boost/wave/util/macro_helpers.hpp>
#include <boost/wave/preprocessing_hooks.hpp>
#include <boost/wave/cpp_iteration_context.hpp>

#include <iterator>
#include <fstream>
#if defined(BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS)
#include <sstream>
#endif

#include <boost/wave/wave_config.hpp>
#include <boost/wave/cpp_exceptions.hpp>
#include <boost/wave/language_support.hpp>
#include <boost/wave/util/file_position.hpp>

namespace wave = boost::wave;
namespace alg = boost::algorithm;

namespace glr
{
namespace shaders
{

/**
 * This class is a generic pre processor for C like code.  It will be used to parse our shader source files.  With it, we will be able to
 * use pre-processor directives in our shader sources files, such as #IFDEF, #DEFINE, and #include.  We will also be able to define and use
 * Macros.
 */
class CPreProcessor : public wave::context_policies::default_preprocessing_hooks
{
public:
	CPreProcessor(std::string source, std::string baseDirectory = std::string());
	virtual ~CPreProcessor();

	/**
	 * Simple structure to hold shader information.
	 */ 
	struct ShaderData {
		ShaderData(std::string name) : name(name)
		{
		}

		std::string name;
		std::map< std::string, std::string > defineMap;
	};

	/**
	 * Processess the shader source code.
	 * 
	 * @param defineMap this is an optional map that can contain any extra/special #defines that have been
	 * defined.
	 */
	void process(std::map< std::string, std::string > defineMap = std::map< std::string, std::string >());

	std::string getName();
	std::string getType();
	std::vector<ShaderData> getShaders();
	std::string getSource();
	std::string getProcessedSource();


	/* Below is a bunch of Boost Wave call-back functions */
	template <typename ContextT, typename ContainerT>
	bool found_unknown_directive(ContextT const& ctx, ContainerT const& line, ContainerT& pending);

	template <typename ContextT, typename ContainerT>
	bool emit_line_directive(ContextT const & ctx, ContainerT & pending, typename ContextT::token_type const & act_token);

	template <typename ContextT>
	bool locate_include_file(ContextT& ctx, std::string&file_path, bool is_system, char const*current_name, std::string&dir_path, std::string&native_name);

#if BOOST_WAVE_USE_DEPRECIATED_PREPROCESSING_HOOKS != 0
	void opened_include_file(std::string const&relname, std::string const&filename, std::size_t /*include_depth*/, bool is_system_include);
#else
	template <typename ContextT>
	void opened_include_file(ContextT const& ctx, std::string const& relname, std::string const& filename, bool is_system_include);
#endif


#if BOOST_WAVE_USE_DEPRECIATED_PREPROCESSING_HOOKS != 0
	void returning_from_include_file();
#else
	template <typename ContextT>
	void returning_from_include_file(ContextT const& ctx);
#endif


#if BOOST_WAVE_USE_DEPRECIATED_PREPROCESSING_HOOKS != 0
	// old signature
	void found_include_directive(std::string const& filename, bool include_next);
#else
	// new signature
	template <typename ContextT>
	bool found_include_directive(ContextT const& ctx, std::string const& filename, bool include_next);
#endif

	/**
	 * Inner class to do the loading of a shader file.  The shader source can come from either a string or from a file
	 * on the disk.
	 */
	template <typename IterContextT>
	class inner {
	public:
		template <typename PositionT>
		static void init_iterators(IterContextT& iter_ctx, PositionT const& act_pos, wave::language_support language)
		{
			static std::map<std::string, std::string> theFiles;

			typedef typename IterContextT::iterator_type iterator_type;

			//std::cout << "init_iterators: " << iter_ctx.filename << std::endl;

			//std::streambuf* buffer = nullptr;

			const char* cString = iter_ctx.filename.c_str();

			// Load data from string
			if ( CPreProcessor::files_.find(std::string(cString)) != CPreProcessor::files_.end())
			{
				std::stringstream ss(CPreProcessor::files_[std::string(cString)]);
				iter_ctx.instring.assign(
					std::istreambuf_iterator<char>(ss.rdbuf()),
					std::istreambuf_iterator<char>()
				);
			}
			// Load data from disk
			else
			{
				// read in the file
				std::ifstream instream(iter_ctx.filename.c_str());
				if ( !instream.is_open())
				{
					//BOOST_WAVE_THROW_CTX(iter_ctx.ctx, wave::preprocess_exception,
					//	wave::bad_include_file, iter_ctx.filename.c_str(), act_pos);
					//std::cout << "error: not open" << std::endl;
					return;
				}
				instream.unsetf(std::ios::skipws);

				iter_ctx.instring.assign(
					std::istreambuf_iterator<char>(instream.rdbuf()),
					std::istreambuf_iterator<char>()
				);
			}

			iter_ctx.first = iterator_type(
				iter_ctx.instring.begin(), 
				iter_ctx.instring.end(),
				PositionT(iter_ctx.filename), 
				language
			);
			iter_ctx.last = iterator_type();
		}

	private:
		std::string instring;
	};

private:
	std::string name_;
	std::string type_;
	std::vector<ShaderData> shaderData_;
	std::string source_;
	std::string processedSource_;

protected:
	static std::map<std::string, std::string> files_;
	std::string baseDirectory_;
};

}
}

#endif // !defined(BOOST_WAVE_ADVANCED_PREPROCESSING_HOOKS_INCLUDED)

#endif /* CPREPROCESSOR_H */
