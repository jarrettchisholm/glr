/*
 * Mesh.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include <boost/log/trivial.hpp>

#include "../common/utilities/AssImpUtilities.h"

#include "Mesh.h"

namespace oglre {

namespace engine {

Mesh::Mesh(const aiMesh* mesh) {
	BOOST_LOG_TRIVIAL(debug) << "loading mesh...";
	
	glm::detail::uint32 currentIndex = 0;
	
	for (glm::detail::uint32 t = 0; t < mesh->mNumFaces; ++t) {
		const aiFace* face = &mesh->mFaces[t];
		GLenum face_mode;
	
		switch(face->mNumIndices) {
			case 1: face_mode = GL_POINTS; break;
			case 2: face_mode = GL_LINES; break;
			case 3: face_mode = GL_TRIANGLES; break;
			default: face_mode = GL_POLYGON; break;
		}
		
		glm::detail::uint32 numIndices = face->mNumIndices;
		
		vertices_.resize( currentIndex + numIndices );
		normals_.resize( currentIndex + numIndices );
		textureCoordinates_.resize( currentIndex + numIndices );
		colors_.resize( currentIndex + numIndices );
		
		//BOOST_LOG_TRIVIAL(debug) << "loading face: " << face->mNumIndices;
		// go through all vertices in face
		for(glm::detail::uint32 i = 0; i < numIndices; i++) {
			// get group index for current index
			int vertexIndex = face->mIndices[i];
			
			if (mesh->mNormals != 0) {
				vertices_[currentIndex + i]		= glm::vec3( mesh->mVertices[vertexIndex].x, mesh->mVertices[vertexIndex].y, mesh->mVertices[vertexIndex].z );
				normals_[currentIndex + i]		= glm::vec3( mesh->mNormals[vertexIndex].x, mesh->mNormals[vertexIndex].y, mesh->mNormals[vertexIndex].z );
			}
			
			if (mesh->HasTextureCoords(0)) {
				textureCoordinates_[currentIndex + i] = glm::vec2( mesh->mTextureCoords[0][vertexIndex].x, mesh->mTextureCoords[0][vertexIndex].y );
			}
			
			//utilities::AssImpUtilities::color4_to_vec4(&mesh->mColors[0][vertexIndex], colors_[colors_.size() + i]);
			if (mesh->mColors[0] != 0) {
				colors_[currentIndex + i]			= glm::vec4(
														(float)mesh->mColors[0][vertexIndex].a,
														(float)mesh->mColors[0][vertexIndex].b,
														(float)mesh->mColors[0][vertexIndex].g,
														(float)mesh->mColors[0][vertexIndex].r
													);
			}			
		}
		
		currentIndex += 3;
	}
	BOOST_LOG_TRIVIAL(debug) << "done loading mesh...";
}

Mesh::~Mesh() {
}

void Mesh::render() {
	glBegin( GL_TRIANGLES );
	
	for (glm::detail::uint32 i = 0; i < vertices_.size(); i++) {
		//BOOST_LOG_TRIVIAL(debug) << "texCoords " << i << ": (" << textureCoordinates_[i].x << ", " << textureCoordinates_[i].y << ")";
		glTexCoord2fv( &textureCoordinates_[i].x );
		glNormal3fv( &normals_[i].x );
		glVertex3fv( &vertices_[i].x );
		//Color4f(&mesh->mColors[0][vertexIndex]);
	}
	
	glEnd();
}


}

}

