/*
 * Mesh.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include <boost/log/trivial.hpp>

#include "../common/utilities/AssImpUtilities.h"

#include "Mesh.h"

namespace glr {
namespace glw {
Mesh::Mesh(IOpenGlDevice* openGlDevice,
		const std::string path, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		std::vector< glm::vec2 > bones)
	: openGlDevice_(openGlDevice), vertices_(vertices), normals_(normals), textureCoordinates_(textureCoordinates), colors_(colors), bones_(bones)
{
	BOOST_LOG_TRIVIAL(debug) << "loading mesh into video memory...";

	// create our vao
	glGenVertexArrays(1, &vaoId_);
	glBindVertexArray(vaoId_);

	// create our vbos
	glGenBuffers(4, &vboIds_[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec3), &vertices_[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[1]);
	glBufferData(GL_ARRAY_BUFFER, textureCoordinates_.size() * sizeof(glm::vec2), &textureCoordinates_[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[2]);
	glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(glm::vec3), &normals_[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// TODO: deal with not having any bones?
	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[3]);
	glBufferData(GL_ARRAY_BUFFER, bones_.size() * sizeof(glm::vec2), &bones_[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);


	// Disable our Vertex Array Object
	//glEnableVertexAttribArray(0);
	// Disable our Vertex Buffer Object
	glBindVertexArray(0);
	
	GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{
		// TODO: throw error
		BOOST_LOG_TRIVIAL(error) << "Error loading mesh in opengl";
		BOOST_LOG_TRIVIAL(error) << "OpenGL error: " << err.name;
	}
	else
	{
		BOOST_LOG_TRIVIAL(debug) << "Successfully loaded mesh.";
	}
}

Mesh::~Mesh()
{
}

void Mesh::render()
{
	glBindVertexArray(vaoId_);

	glDrawArrays(GL_TRIANGLES, 0, vertices_.size());

	glBindVertexArray(0);
}
}
}

