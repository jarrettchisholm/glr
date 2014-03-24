#ifndef TERRAINMESHSERIALIZER_H_
#define TERRAINMESHSERIALIZER_H_

#include <fstream>
#include <string>

#include "TerrainMesh.hpp"

namespace glr
{
namespace terrain
{

/**
 * 
 */
void serialize(const std::string& filename, TerrainMesh& terrainMesh);
void deserialize(const std::string& filename, TerrainMesh& terrainMesh);

void serialize(std::ofstream& stream, TerrainMesh& terrainMesh);
void deserialize(std::ifstream& stream, TerrainMesh& terrainMesh);

}
}

#endif /* TERRAINMESHSERIALIZER_H_ */
