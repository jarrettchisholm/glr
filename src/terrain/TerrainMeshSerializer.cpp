#include "terrain/TerrainMeshSerializer.hpp"

namespace glr
{
namespace terrain
{

/**
 * 
 */
void serialize(const std::string& filename, TerrainMesh& terrainMesh)
{
	std::ofstream stream(filename, std::ios::out | std::ios::binary);
	serialize(stream, terrainMesh);
}

void deserialize(const std::string& filename, TerrainMesh& terrainMesh)
{
	std::ifstream stream(filename, std::ios::in | std::ios::binary);
	deserialize(stream, terrainMesh);
}

void serialize(std::ofstream& stream, TerrainMesh& terrainMesh)
{
	auto vertices = terrainMesh.getVertices();
	
	glmd::int32 size = vertices.size();
	stream.write((char*)&size, sizeof(glmd::int32));
	
	for ( auto& v : vertices)
		stream.write((char*)&v, sizeof(glm::vec3));
}

void deserialize(std::ifstream& stream, TerrainMesh& terrainMesh)
{
	glmd::int32 size = 0;
	stream.read((char*)&size, sizeof(glmd::int32));
	
	auto vertices = std::vector<glm::vec3>( size );
	
	for ( auto& v : vertices)
		stream.read((char*)&v, sizeof(glm::vec3));
	
	terrainMesh.setVertices( vertices );
}

/*
void TerrainMesh::serialize(const std::string& filename)
{
	std::ofstream stream(filename, std::ios::out | std::ios::binary);
	serialize(stream);
}

void TerrainMesh::deserialize(const std::string& filename)
{
	std::ifstream stream(filename, std::ios::in | std::ios::binary);
	deserialize(stream);
}

void TerrainMesh::serialize(std::ofstream& stream)
{
	glmd::int32 size = vertices_.size();
	stream.write((char*)&size, sizeof(glmd::int32));
	
	for ( auto& v : vertices_)
		stream.write((char*)&v, sizeof(glm::vec3));
}

void TerrainMesh::deserialize(std::ifstream& stream)
{
	glmd::int32 size = 0;
	stream.read((char*)&size, sizeof(glmd::int32));
	
	vertices_ = std::vector<glm::vec3>( size );
	
	for ( auto& v : vertices_)
		stream.read((char*)&v, sizeof(glm::vec3));
}
*/

}
}
