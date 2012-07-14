/*
 * Test.h
 *
 *  Created on: 2011-05-05
 *      Author: jarrett
 */

#ifndef TEST_H_
#define TEST_H_

//#include <SDL/SDL.h>

// Light Weight Input System (LWIS)
#include "LWISCommon.h"

using namespace compatibility;

class Test: public lwis::engine::IKeyboardListener, public lwis::engine::IMouseListener {
public:
	Test();
	virtual ~Test();

	//static char getChar(SDLKey sym, SDLMod mod);

	virtual void receiveKeyboardEvent(lwis::engine::KEYBOARD_EVENT evt, lwis::engine::KEY key, uint32 mod, char8 character);
	virtual void receiveMouseEvent(lwis::engine::MouseEvent evt);

	static float32 rotationX;
	static float32 rotationY;
	static sint32 mousePosX;
	static sint32 mousePosY;

	static bool test;
};

#endif /* TEST_H_ */
