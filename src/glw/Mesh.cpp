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
		const std::string name,
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		std::vector<VertexBoneData > bones,
		BoneData boneData)
	: openGlDevice_(openGlDevice), name_(name), vertices_(vertices), normals_(normals), textureCoordinates_(textureCoordinates), colors_(colors), bones_(bones), boneData_(boneData)
{
	load();
}

Mesh::Mesh(IOpenGlDevice* openGlDevice,
		const std::string name,
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors
	)
	: openGlDevice_(openGlDevice), name_(name), vertices_(vertices), normals_(normals), textureCoordinates_(textureCoordinates), colors_(colors)
{
	bones_ = std::vector< VertexBoneData >();
	boneData_ = BoneData();
	
	load();
}

void Mesh::load()
{
	BOOST_LOG_TRIVIAL(debug) << "loading mesh into video memory...";

	// create our vao
	glGenVertexArrays(1, &vaoId_);
	glBindVertexArray(vaoId_);

	// create our vbos
	glGenBuffers(5, &vboIds_[0]);

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
	
	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[3]);
	glBufferData(GL_ARRAY_BUFFER, colors_.size() * sizeof(glm::vec4), &colors_[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
	
	//std::cout << "SIZE: " << vertices_.size() << " " << sizeof(glm::ivec4) << " " << bones_.size() << " " << sizeof(VertexBoneData) << std::endl;
	
	// TODO: We might want to not load any bone data (if there is none) and use a different shader?  Not sure best way to handle this.....?
	// Deal with not having any bones
	if (bones_.size() == 0)
	{
		bones_.resize( vertices_.size() );
		for (auto& b : bones_)
		{
			b.weights = glm::vec4(0.25f);
		}
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[4]);
	glBufferData(GL_ARRAY_BUFFER, bones_.size() * sizeof(VertexBoneData), &bones_[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribIPointer(4, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)(sizeof(glm::ivec4)));
	
	/*
	std::vector< glm::ivec4 > boneIds = std::vector< glm::ivec4 >();
	std::vector< glm::vec4 > weights = std::vector< glm::vec4 >();
	
	for ( VertexBoneData& d : bones_ )
	{
		boneIds.push_back( d.boneIds );
		weights.push_back( d.weights );
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[3]);
	glBufferData(GL_ARRAY_BUFFER, boneIds.size() * sizeof(glm::ivec4), &boneIds[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[4]);
	glBufferData(GL_ARRAY_BUFFER, weights.size() * sizeof(glm::vec4), &weights[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, 0);
	*/
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

BoneData& Mesh::getBoneData()
{
	return boneData_;
}
}
}

