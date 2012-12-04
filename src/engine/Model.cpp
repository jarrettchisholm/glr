/*
 * Model.cpp
 *
 *  Created on: 2012-09-20
 *      Author: jarrett
 */

#include "Model.h"

namespace icee {

namespace engine {


Model::Model() {
	
}

Model::Model( std::shared_ptr<aiScene> scene ) {
	loadMeshes(scene);
	loadTextures(scene);
	loadMaterials(scene);
	loadAnimations(scene);
}

Model::~Model() {	
	//destroyAILogger();
}

void Model::render() {
	for (uint32 i=0; i < meshes_.size(); i++) {
		meshes_[i]->render();
	}
	
	/*
	float tmp;

	// scale the whole asset to fit into our view frustum 
	tmp = scene_max.x-scene_min.x;
	tmp = aisgl_max(scene_max.y - scene_min.y,tmp);
	tmp = aisgl_max(scene_max.z - scene_min.z,tmp);
	tmp = 1.f / tmp;
	//glScalef(tmp, tmp, tmp);

        // center the model
	glTranslatef( -scene_center.x, -scene_center.y, -scene_center.z );

        // if the display list has not been made yet, create a new one and
        // fill it with scene contents
	if(scene_list == 0) {
	    scene_list = glGenLists(1);
	    
	    setLighting();
	    
	    glNewList(scene_list, GL_COMPILE);
            // now begin at the root node of the imported data and traverse
            // the scenegraph by multiplying subsequent local transforms
            // together on GL's matrix stack.
	    recursive_render(scene_, scene_->mRootNode);
	    glEndList();
	}

	glCallList(scene_list);

	//do_motion();
	*/
}

void Model::recursive_render(const aiScene* scene, const aiNode* nd) {
	/*
	//testDrawTest1();
	
	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = nd->mTransformation;

	// update transform
	m.Transpose();
	glPushMatrix();
	glMultMatrixf((float*)&m);
	

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		apply_material(scene->mMaterials[mesh->mMaterialIndex]);

		if(mesh->mNormals == NULL) {
			glDisable(GL_LIGHTING);
		} else {
			glEnable(GL_LIGHTING);
		}
		
		if(mesh->mColors[0] != NULL) {
			glEnable(GL_COLOR_MATERIAL);
		}
		else {
			glDisable(GL_COLOR_MATERIAL);
		}

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glEnable( GL_TEXTURE_2D );
			//glBindTexture(GL_TEXTURE_2D, textureid_);

			glBegin(face_mode);

			// go through all vertices in face
			for(i = 0; i < face->mNumIndices; i++) {
				// get group index for current index
				int vertexIndex = face->mIndices[i];
				
				if(mesh->mColors[0] != NULL)
					Color4f(&mesh->mColors[0][vertexIndex]);
					
				if(mesh->mNormals != NULL) {

					// HasTextureCoords(texture_coordinates_set)
					if(mesh->HasTextureCoords(0)) {
						glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, 1 - mesh->mTextureCoords[0][vertexIndex].y); //mTextureCoords[channel][vertex]
					}

					glNormal3fv(&mesh->mNormals[vertexIndex].x);
					glVertex3fv(&mesh->mVertices[vertexIndex].x);
				}
			}

			glEnd();
			
			//glDisable(GL_TEXTURE_2D);
			//glDisable(GL_BLEND);
		}

	}
	

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(scene, nd->mChildren[n]);
	}

	glPopMatrix();
	*/
}

/*
void Model::apply_material(const aiMaterial *mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;	// changed: to unsigned

	int texIndex = 0;
	aiString texPath;	//contains filename of texture

	if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
	{
		//bind texture
		unsigned int texId = *textureIdMap[texPath.data];
		glBindTexture(GL_TEXTURE_2D, texId);
	}

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	max = 1;
	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
	if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}
*/

void ModelManager::loadMeshes(std::shared_ptr<aiScene> scene) {
	
	loadMeshesRecursive(scene_, scene_->mRootNode);
}

void ModelManager::loadMeshesRecursive(std::shared_ptr<aiScene> scene, const aiNode* node) {
	// get all meshes assigned to this node
	for (uint32 n = 0; n < node->mNumMeshes; n++) {
		// create new mesh
		meshes_[n] = new Mesh( scene->mMeshes[node->mMeshes[n]] );
	}
	

	// draw all children
	for (uint32 n = 0; n < node->mNumChildren; n++) {
		recursive_render(scene, node->mChildren[n]);
	}
}

void ModelManager::loadTextures(std::shared_ptr<aiScene> scene) {
	if (scene->HasTextures()) {
		BOOST_LOG_TRIVIAL(debug) << "Support for meshes with embedded textures is not implemented";
		exit 1;
	}

	/* getTexture Filenames and Numb of Textures */
	for (uint32 m = 0; m < scene->mNumMaterials; m++) {
		int texIndex = 0;
		aiReturn texFound = AI_SUCCESS;

		aiString path;	// filename

		while (texFound == AI_SUCCESS) {
			texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			if ( textureMap_[path.data] == 0 ) {
				//fill map with textures, pointers still NULL yet
				textureMap_[path.data] = 0;
				texIndex++;
			}
		}
	}
	
	int numTextures = textureMap_.size();
	
	/* create and fill array with GL texture ids */
	//textureIds_ = new GLuint[numTextures];
	/* Texture name generation */
	//glGenTextures(numTextures, textureIds_);
	
	std::map<std::string, GLuint*>::iterator itr = textureMap_.begin();
	
	for (sint32 i = 0; i < numTextures; i++) {
		//save IL image ID
		std::string filename = (*itr).first;  // get filename
		
		Texture* texture = TextureManager::getInstance()->getTexture(filename);
		
		if (texture == 0) {
			BOOST_LOG_TRIVIAL(debug) << "Not able to load texture.";
		}
		
		// save texture for filename in map
		(*itr).second =  texture;
		
		itr++;
	}
	
}

void ModelManager::loadMaterials(std::shared_ptr<aiScene> scene) {
	// TODO: implement	
}

void ModelManager::loadAnimations(std::shared_ptr<aiScene> scene) {
	// TODO: implement	
}

// temporary method
void Model::setLighting() {
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	glEnable(GL_LIGHT0);
}

}

}


