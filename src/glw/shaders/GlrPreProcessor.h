/*
 * GlrPreProcessor.h
 * 
 * Author: Jarrett Chisholm <j.chisholm@chisholmsoft.com>
 * Date: 2013
 *
 *
 */


#ifndef GLRPREPROCESSOR_H
#define GLRPREPROCESSOR_H

#include "CPreProcessor.h"

#include "../../exceptions/GlException.h"

namespace glr {
namespace shaders {

/**
 * Class that will extend the functionality of the CPreProcessor class for anything specific to processing
 * 'glr' shader source data.
 */
class GlrPreProcessor : public CPreProcessor {
public:
	GlrPreProcessor(std::string source, std::string baseDirectory = std::string());

private:
};

}
}
#endif /* GLRPREPROCESSOR_H */
