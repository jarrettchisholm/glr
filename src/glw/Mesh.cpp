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
Mesh::Mesh(const aiMesh* mesh)
{
	BOOST_LOG_TRIVIAL(debug) << "loading mesh...";

	glm::detail::uint32 currentIndex = 0;

	for ( glm::detail::uint32 t = 0; t < mesh->mNumFaces; ++t )
	{
		const aiFace* face = &mesh->mFaces[t];
		GLenum face_mode;

		switch ( face->mNumIndices )
		{
			case 1: 
				face_mode = GL_POINTS; 
				break;
	
			case 2: 
				face_mode = GL_LINES; 
				break;
	
			case 3: 
				face_mode = GL_TRIANGLES; 
				break;
	
			default:
				face_mode = GL_POLYGON; 
				break;
		}

		glm::detail::uint32 numIndices = face->mNumIndices;

		vertices_.resize(currentIndex + numIndices);
		normals_.resize(currentIndex + numIndices);
		textureCoordinates_.resize(currentIndex + numIndices);
		colors_.resize(currentIndex + numIndices);

		//BOOST_LOG_TRIVIAL(debug) << "loading face: " << face->mNumIndices;
		// go through all vertices in face
		for ( glm::detail::uint32 i = 0; i < numIndices; i++ )
		{
			// get group index for current index i
			int vertexIndex = face->mIndices[i];

			if ( mesh->mNormals != 0 )
			{
				vertices_[currentIndex + i] = glm::vec3(mesh->mVertices[vertexIndex].x, mesh->mVertices[vertexIndex].y, mesh->mVertices[vertexIndex].z);
				normals_[currentIndex + i] = glm::vec3(mesh->mNormals[vertexIndex].x, mesh->mNormals[vertexIndex].y, mesh->mNormals[vertexIndex].z);
			}

			if ( mesh->HasTextureCoords(0))
			{
				textureCoordinates_[currentIndex + i] = glm::vec2(mesh->mTextureCoords[0][vertexIndex].x, mesh->mTextureCoords[0][vertexIndex].y);
			}

			//utilities::AssImpUtilities::color4_to_vec4(&mesh->mColors[0][vertexIndex], colors_[colors_.size() + i]);
			if ( mesh->mColors[0] != 0 )
			{
				colors_[currentIndex + i] = glm::vec4(
					(float)mesh->mColors[0][vertexIndex].a,
					(float)mesh->mColors[0][vertexIndex].b,
					(float)mesh->mColors[0][vertexIndex].g,
					(float)mesh->mColors[0][vertexIndex].r
					);
			}
		}

		currentIndex += 3;
	}

	BOOST_LOG_TRIVIAL(debug) << "loading mesh into video memory...";


	// create our vao
	glGenVertexArrays(1, &vaoId_);
	glBindVertexArray(vaoId_);


	// create our vbos
	glGenBuffers(3, &vboIds_[0]);                             // Using only '1' for now (otherwise causes seg fault)

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


	// Disable our Vertex Array Object
	//glEnableVertexAttribArray(0);
	// Disable our Vertex Buffer Object
	glBindVertexArray(0);

	BOOST_LOG_TRIVIAL(debug) << "done loading mesh...";
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

