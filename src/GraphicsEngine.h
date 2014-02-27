#ifndef GRAPHICSENGINE_H_
#define GRAPHICSENGINE_H_

#include <memory>

#include "GlrProgram.h"

namespace glr
{	
class GraphicsEngine {
public:
	static std::unique_ptr<GlrProgram> createProgram();
	static std::unique_ptr<GlrProgram> createProgram(ProgramSettings settings);

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
