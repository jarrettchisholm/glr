/*
 * GraphicsEngine.cpp
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#include "GraphicsEngine.h"

#include "Window.h"

namespace glr {
/**
 *
 */
std::unique_ptr<GlrProgram> GraphicsEngine::createProgram()
{
	return GraphicsEngine::createProgram( ProgramSettings() );
}

std::unique_ptr<GlrProgram> GraphicsEngine::createProgram(ProgramSettings settings)
{
	std::unique_ptr<GlrProgram> program(new GlrProgram( settings ));

	return program;
}
}
