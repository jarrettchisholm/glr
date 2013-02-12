/*
 * Light.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>

#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <boost/log/trivial.hpp>

#include "Light.h"

#include "common/math/Math.h"

namespace oglre {


Light::Light() {
	setLookAt( 1, 1, 1 );
	setPosition( 0, 0, 0 );
	setScale(1, 1, 1);
	
	active_ = true;

	initialize();
}

Light::Light(glm::vec3 position, glm::vec3 lookAt, bool active) {
	setPosition(position);
	setLookAt(lookAt);
	active_ = active;

	initialize();
}

Light::~Light() {
}

void Light::initialize() {
	BOOST_LOG_TRIVIAL(debug) << "Light initialized.";
}

void Light::render() {
	if (isActive()) {
		
	}
}

/**
* Does nothing in the Light.
*/ 
void Light::attach(models::IModel* model) {
	
}

/**
* Does nothing in the Light.
*/ 
void Light::setVisible(bool isVisible) {
	
}

}
