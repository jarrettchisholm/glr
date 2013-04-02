/*
 * GraphicsEngine.cpp
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#include <boost/log/trivial.hpp>

#include "GraphicsEngine.h"

#include "Window.h"

namespace glr {



/**
 *
 */
std::unique_ptr<GlrProgram> GraphicsEngine::createProgram() {
	std::unique_ptr<GlrProgram> program(new GlrProgram());
	
	return program;
}

}
