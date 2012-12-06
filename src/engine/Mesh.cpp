/*
 * Mesh.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include "Mesh.h"

namespace icee {

namespace engine {

Mesh::Mesh(const aiMesh* mesh) {
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
		
		// go through all vertices in face
		for(uint32 i = 0; i < face->mNumIndices; i++) {
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
			
			vertices_[numIndices*t + i]		= glm::vec3( mesh->mVertices[vertexIndex].x, mesh->mVertices[vertexIndex].y, mesh->mVertices[vertexIndex].z );
			normals_[numIndices*t + i]		= glm::vec3( mesh->mNormals[vertexIndex].x, mesh->mNormals[vertexIndex].y, mesh->mNormals[vertexIndex].z );
			colors_[numIndices*t + i]		= glm::vec4(
													mesh->mColors[0][vertexIndex].a,
													mesh->mColors[0][vertexIndex].b,
													mesh->mColors[0][vertexIndex].g,
													mesh->mColors[0][vertexIndex].r
												);
			
		}		
	}
}

Mesh::~Mesh() {
}

void Mesh::render() {
	
}


}

}

