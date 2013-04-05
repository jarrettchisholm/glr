/*
 * GraphicsEngine.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#ifndef GRAPHICSENGINE_H_
#define GRAPHICSENGINE_H_

#include <memory>

#include "GlrProgram.h"

// we require a C++ compiler.
#if !defined(__cplusplus)
#error C++ compiler required.
#endif

namespace glr {
class GraphicsEngine {
public:
	static std::unique_ptr<GlrProgram> createProgram();

private:
	/**
	 * Private.
	 */
	GraphicsEngine()
	{
	}
	;

	/**
	 * Private.
	 */
	GraphicsEngine(const GraphicsEngine& iceGE)
	{
	}
	;

	/**
	 * Private.
	 */
	virtual ~GraphicsEngine()
	{
	}
	;
};
}
#endif /* GRAPHICSENGINE_H_ */
