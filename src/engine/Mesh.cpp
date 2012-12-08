/*
 * Mesh.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include <boost/log/trivial.hpp>

#include "../common/utilities/AssImpUtilities.h"

#include "Mesh.h"

namespace icee {

namespace engine {

Mesh::Mesh(const aiMesh* mesh) {
	BOOST_LOG_TRIVIAL(debug) << "loading mesh...";
	for (uint32 t = 0; t < mesh->mNumFaces; ++t) {
		const aiFace* face = &mesh->mFaces[t];
		GLenum face_mode;
	
		switch(face->mNumIndices) {
			case 1: face_mode = GL_POINTS; break;
			case 2: face_mode = GL_LINES; break;
			case 3: face_mode = GL_TRIANGLES; break;
			default: face_mode = GL_POLYGON; break;
		}
		
		uint32 numIndices = face->mNumIndices;
		
		vertices_.resize( vertices_.size() + numIndices );
		normals_.resize( normals_.size() + numIndices );
		colors_.resize( colors_.size() + numIndices );
		BOOST_LOG_TRIVIAL(debug) << "size: " << (vertices_.size() + numIndices);
		//BOOST_LOG_TRIVIAL(debug) << "loading face: " << face->mNumIndices;
		// go through all vertices in face
		for(uint32 i = 0; i < numIndices; i++) {
			// get group index for current index
			int vertexIndex = face->mIndices[i];
			/*
			if(mesh->mColors[0] != NULL)
				Color4f(&mesh->mColors[0][vertexIndex]);
				
			if(mesh->mNormals != NULL) {
				glNormal3fv(&mesh->mNormals[vertexIndex].x);
				glVertex3fv(&mesh->mVertices[vertexIndex].x);
			}
			*/
				
			if (mesh->mNormals != 0) {
				vertices_[vertices_.size() + i]		= glm::vec3( mesh->mVertices[vertexIndex].x, mesh->mVertices[vertexIndex].y, mesh->mVertices[vertexIndex].z );
				normals_[normals_.size() + i]		= glm::vec3( mesh->mNormals[vertexIndex].x, mesh->mNormals[vertexIndex].y, mesh->mNormals[vertexIndex].z );
			}
			
			//utilities::AssImpUtilities::color4_to_vec4(&mesh->mColors[0][vertexIndex], colors_[colors_.size() + i]);
			if (mesh->mColors[0] != 0) {
				colors_[colors_.size() + i]			= glm::vec4(
														(float)mesh->mColors[0][vertexIndex].a,
														(float)mesh->mColors[0][vertexIndex].b,
														(float)mesh->mColors[0][vertexIndex].g,
														(float)mesh->mColors[0][vertexIndex].r
													);
			}
			
			BOOST_LOG_TRIVIAL(debug) << "rendering mesh: " << vertices_.size() + i;
			
		}	
	}
	BOOST_LOG_TRIVIAL(debug) << "rendering mesh: " << vertices_.size();
	BOOST_LOG_TRIVIAL(debug) << "done loading mesh...";
}

Mesh::~Mesh() {
}

void Mesh::render() {
	glBegin(GL_TRIANGLES);
	
	//BOOST_LOG_TRIVIAL(debug) << "rendering mesh: " << vertices_.size();
	
	for (uint32 i = 0; i < vertices_.size(); i++) {
		glNormal3fv( &normals_[i].x );
		glVertex3fv( &vertices_[i].x );
	}
	
	glEnd();
}


}

}

