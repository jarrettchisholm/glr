#include <sstream>

#include "glw/Mesh.hpp"

#include "common/logger/Logger.hpp"

#include "exceptions/GlException.hpp"

namespace glr
{
namespace glw
{

Mesh::Mesh(IOpenGlDevice* openGlDevice, const std::string& name) : openGlDevice_(openGlDevice), name_(name)
{
	vertexBoneData_ = std::vector< VertexBoneData >();
	boneData_ = BoneData();
	
	vaoId_ = 0;
}

Mesh::Mesh(IOpenGlDevice* openGlDevice,
		const std::string& name,
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		std::vector<VertexBoneData > vertexBoneData,
		BoneData boneData)
	: openGlDevice_(openGlDevice), name_(name), vertices_(vertices), normals_(normals), textureCoordinates_(textureCoordinates), colors_(colors), vertexBoneData_(vertexBoneData), boneData_(boneData)
{
	vaoId_ = 0;
	
	allocateVideoMemory();
	pushToVideoMemory();
}

Mesh::Mesh(IOpenGlDevice* openGlDevice,
		const std::string& name,
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors
	)
	: openGlDevice_(openGlDevice), name_(name), vertices_(vertices), normals_(normals), textureCoordinates_(textureCoordinates), colors_(colors)
{
	vertexBoneData_ = std::vector< VertexBoneData >();
	boneData_ = BoneData();
	vaoId_ = 0;
	
	allocateVideoMemory();
	pushToVideoMemory();
}

Mesh::~Mesh()
{
	freeVideoMemory();
}

void Mesh::pushToVideoMemory()
{
	LOG_DEBUG( "loading mesh '" + name_ +"' into video memory." );

	if (vaoId_ == 0)
	{
		std::string msg = std::string( "Mesh has not been allocated in video memory yet." );
		LOG_ERROR( msg );
		throw exception::GlException( msg );
	}
	
	glBindVertexArray(vaoId_);

	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_.size() * sizeof(glm::vec3), &vertices_[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, textureCoordinates_.size() * sizeof(glm::vec2), &textureCoordinates_[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[2]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, normals_.size() * sizeof(glm::vec3), &normals_[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[3]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, colors_.size() * sizeof(glm::vec4), &colors_[0]);
	
	//std::cout << "SIZE: " << vertices_.size() << " " << sizeof(glm::ivec4) << " " << vertexBoneData_.size() << " " << sizeof(VertexBoneData) << std::endl;
	
	// TODO: We might want to not load any bone data (if there is none) and use a different shader?  Not sure best way to handle this.....?
	// Deal with not having any bones
	if (vertexBoneData_.size() == 0)
	{
		vertexBoneData_.resize( vertices_.size() );
		for (auto& b : vertexBoneData_)
		{
			b.weights = glm::vec4(0.25f);
		}
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[4]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBoneData_.size() * sizeof(VertexBoneData), &vertexBoneData_[0]);
	
	glBindVertexArray(0);
	
	GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{		
		std::string msg = std::string( "Error while push data for mesh '" + name_ + "' to video memory in OpenGL: " + err.name);
		LOG_ERROR( msg );
		throw exception::GlException( msg );
	}
	else
	{
		LOG_DEBUG( "Successfully pushed data for mesh '" + name_ + "' to video memory." );
	}
}

void Mesh::pullFromVideoMemory()
{
	// TODO: Implement
}

void Mesh::freeLocalData()
{
	vertices_ = std::vector< glm::vec3 >();
	normals_ = std::vector< glm::vec3 >();
	textureCoordinates_ = std::vector< glm::vec2 >();
	colors_ = std::vector< glm::vec4 >();
	vertexBoneData_ = std::vector< VertexBoneData >();
}

void Mesh::freeVideoMemory()
{
	if (vaoId_ == 0)
		return;

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &vaoId_);
	glDeleteBuffers(5, &vboIds_[0]);
	
	vaoId_ = 0;
}

void Mesh::allocateVideoMemory()
{
	if (vaoId_ > 0)
	{
		std::string msg = std::string( "Mesh already has a vertex array object generated." );
		LOG_ERROR( msg );
		throw exception::GlException( msg );
	}
	
	// create our vao
	glGenVertexArrays(1, &vaoId_);
	
	glBindVertexArray(vaoId_);

	// create our vbos
	glGenBuffers(5, &vboIds_[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[1]);
	glBufferData(GL_ARRAY_BUFFER, textureCoordinates_.size() * sizeof(glm::vec2), nullptr, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[2]);
	glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[3]);
	glBufferData(GL_ARRAY_BUFFER, colors_.size() * sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
	
	//std::cout << "SIZE: " << vertices_.size() << " " << sizeof(glm::ivec4) << " " << vertexBoneData_.size() << " " << sizeof(VertexBoneData) << std::endl;
	
	// TODO: We might want to not load any bone data (if there is none) and use a different shader?  Not sure best way to handle this.....?
	// Deal with not having any bones
	if (vertexBoneData_.size() == 0)
	{
		vertexBoneData_.resize( vertices_.size() );
		for (auto& b : vertexBoneData_)
		{
			b.weights = glm::vec4(0.25f);
		}
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, vboIds_[4]);
	glBufferData(GL_ARRAY_BUFFER, vertexBoneData_.size() * sizeof(VertexBoneData), nullptr, GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribIPointer(4, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)(sizeof(glm::ivec4)));

	glBindVertexArray(0);
	
	GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{
		// Cleanup
		freeVideoMemory();
		
		std::string msg = std::string( "Error while allocating video memory for mesh '" + name_ + "' in OpenGL: " + err.name);
		LOG_ERROR( msg );
		throw exception::GlException( msg );
	}
	else
	{
		LOG_DEBUG( "Successfully allocated memory for mesh '" + name_ + "'." );
	}
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

const std::string& Mesh::getName() const
{
	return name_;
}

void Mesh::setName(const std::string& name)
{
	name_ = name;
}

void Mesh::setVertices(std::vector< glm::vec3 > vertices)
{
	vertices_ = vertices;
}

void Mesh::setNormals(std::vector< glm::vec3 > normals)
{
	normals_ = normals;
}

void Mesh::setTextureCoordinates(std::vector< glm::vec2 > textureCoordinates)
{
	textureCoordinates_ = textureCoordinates;
}

void Mesh::setColors(std::vector< glm::vec4 > colors)
{
	colors_ = colors;
}

void Mesh::setVertexBoneData(std::vector< VertexBoneData > vertexBoneData)
{
	vertexBoneData_ = vertexBoneData;
}

std::vector< glm::vec3 >& Mesh::getVertices()
{
	return vertices_;
}

std::vector< glm::vec3 >& Mesh::getNormals()
{
	return normals_;
}

std::vector< glm::vec2 >& Mesh::getTextureCoordinates()
{
	return textureCoordinates_;
}

std::vector< glm::vec4 >& Mesh::getColors()
{
	return colors_;
}

std::vector< VertexBoneData >& Mesh::getVertexBoneData()
{
	return vertexBoneData_;
}

}
}
