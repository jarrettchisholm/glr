#ifndef GLRPREPROCESSOR_H
#define GLRPREPROCESSOR_H

#include "CPreProcessor.hpp"

#include "../../exceptions/GlException.hpp"

namespace glr
{
namespace shaders
{

/**
 * Class that will extend the functionality of the CPreProcessor class for anything specific to processing
 * 'glr' shader source data.
 */
class GlrPreProcessor : public CPreProcessor
{
public:
	GlrPreProcessor(std::string source, std::string baseDirectory = std::string());

private:
};

}
}

#endif /* GLRPREPROCESSOR_H */
