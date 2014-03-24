#include <utility>

#include "terrain/TerrainMesh.hpp"

#include "exceptions/GlException.hpp"

namespace glr
{
namespace terrain
{

TerrainMesh::TerrainMesh(glw::IOpenGlDevice* openGlDevice, std::string name) : Mesh(openGlDevice, std::move(name))
{
	vertexBoneData_ = std::vector< glw::VertexBoneData >();
	boneData_ = glw::BoneData();
	texBlendVboId_ = 0;
}

TerrainMesh::TerrainMesh(glw::IOpenGlDevice* openGlDevice,
		std::string name,
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		std::vector< glm::vec4 > texBlendingData,
		GLint shaderVariableLocation
	)
	: Mesh(openGlDevice, std::move(name)), texBlendingData_(std::move(texBlendingData)), shaderVariableLocation_(shaderVariableLocation)
{
	vertices_ = std::move(vertices);
	normals_ = std::move(normals);
	textureCoordinates_ = std::move(textureCoordinates);
	colors_ = std::move(colors);
	vertexBoneData_ = std::vector< glw::VertexBoneData >();
	boneData_ = glw::BoneData();
	
	texBlendVboId_ = 0;
	
	allocateVideoMemory();
	pushToVideoMemory();
}

TerrainMesh::~TerrainMesh()
{
	freeVideoMemory();
}

void TerrainMesh::pushToVideoMemory()
{
	Mesh::pushToVideoMemory();
	
	LOG_DEBUG( "Loading texture blending data into video memory for mesh '" + name_ +"'." );

	glBindVertexArray(vaoId_);

	glBindBuffer(GL_ARRAY_BUFFER, texBlendVboId_);
	glBufferSubData(GL_ARRAY_BUFFER, 0, texBlendingData_.size() * sizeof(glm::vec4), &texBlendingData_[0]);

	glBindVertexArray(0);
	
	glw::GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{		
		std::string msg = std::string( "Error while pushing texture blending data to video memory for mesh '" + name_ + "' in OpenGL: " + err.name);
		LOG_ERROR( msg );
		throw exception::GlException( msg );
	}
	else
	{
		LOG_DEBUG( "Successfully pushed texture blending data to video memory for mesh '" + name_ + "'." );
	}
}

void TerrainMesh::pullFromVideoMemory()
{
	Mesh::pullFromVideoMemory();
	// TODO: Implement
}

void TerrainMesh::freeLocalData()
{
	Mesh::freeLocalData();
	
	texBlendingData_ = std::vector< glm::vec4 >();
}

void TerrainMesh::freeVideoMemory()
{	
	Mesh::freeVideoMemory();
	
	if (texBlendVboId_ == 0)
		return;

	glDeleteBuffers(1, &texBlendVboId_);
	
	texBlendVboId_ = 0;
}

void TerrainMesh::allocateVideoMemory()
{
	Mesh::allocateVideoMemory();
	
	if (texBlendVboId_ > 0)
	{
		std::string msg = std::string( "Mesh already has a texture blend vertex buffer object generated." );
		LOG_ERROR( msg );
		throw exception::GlException( msg );
	}
	
	glBindVertexArray(vaoId_);

	// create our vbo
	glGenBuffers(1, &texBlendVboId_);
	
	glBindBuffer(GL_ARRAY_BUFFER, texBlendVboId_);
	glBufferData(GL_ARRAY_BUFFER, texBlendingData_.size() * sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);
	glEnableVertexAttribArray(shaderVariableLocation_);
	glVertexAttribPointer(shaderVariableLocation_, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	
	glw::GlError err = openGlDevice_->getGlError();
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
		LOG_DEBUG( "Successfully allocated texture blending data in video memory for mesh '" + name_ + "'." );
	}
}

void TerrainMesh::setTextureBlendingData(std::vector< glm::vec4 > texBlendingData)
{
	texBlendingData_ = std::move(texBlendingData);
}

void TerrainMesh::setShaderVariableLocation(GLint shaderVariableLocation)
{
	shaderVariableLocation_ = shaderVariableLocation;
}

std::vector< glm::vec4 >& TerrainMesh::getTextureBlendingData()
{
	return texBlendingData_;
}

GLint TerrainMesh::getShaderVariableLocation() const
{
	return shaderVariableLocation_;
}

}
}

