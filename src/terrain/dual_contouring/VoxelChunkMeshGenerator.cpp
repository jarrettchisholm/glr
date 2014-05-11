#include <iostream>
#include <sstream>
#include <fstream>
#include <mutex>

#include "terrain/dual_contouring/VoxelChunkMeshGenerator.hpp"

#include "terrain/dual_contouring/Qef.hpp"
#include "terrain/Interpolation.hpp"

#include "terrain/Constants.hpp"

namespace glr
{
namespace terrain
{
namespace dual_contouring
{

VoxelChunkMeshGenerator::VoxelChunkMeshGenerator()
{
}

VoxelChunkMeshGenerator::VoxelChunkMeshGenerator(IFieldFunction* fieldFunction) : fieldFunction_(fieldFunction)
{
}

VoxelChunkMeshGenerator::~VoxelChunkMeshGenerator()
{
}

glmd::int32 edgeTable_[256] = {
	0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
	0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
	0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
	0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
	0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
	0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
	0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
	0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
	0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
	0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
	0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
	0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
	0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
	0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
	0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
	0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
	0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
	0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
	0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
	0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
	0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
	0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
	0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
	0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
	0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
	0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
	0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
	0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
	0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
	0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
	0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
	0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0
};

/**
 * Determines whether the points provided define a a fully solid space or a totally empty space.
 * 
 * @return True if space is totally empty or solid; false otherwise.
 */
bool VoxelChunkMeshGenerator::isEmptyOrSolid(Points& points) const
{
	bool isEmpty = true;
	bool isSolid = true;
	
	for (glmd::int32 x=0; x < glr::terrain::constants::SIZE+1; x++)
	{
		for (glmd::int32 y=0; y < glr::terrain::constants::SIZE+1; y++)
		{
			for (glmd::int32 z=0; z < glr::terrain::constants::SIZE+1; z++)
			{
				if (isEmpty)
				{
					if (points[x][y][z] <= 0.0f)
						isEmpty = false;
				}
				
				if (isSolid)
				{
					if (points[x][y][z] > 0.0f)
						isSolid = false;
				}
				
				if (!isEmpty && !isSolid)
					return false;
			}
		}
	}
	
	if (isEmpty && isSolid)
		assert(0);
	
	return true;
}

/**
 * Get noise at the point x, y, z on the grid with the given coordinates.
 */
glmd::float32 VoxelChunkMeshGenerator::getInterpolatedNoise(Points& points, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ, glmd::float32 x, glmd::float32 y, glmd::float32 z) const
{
#define DENSITY_AT(i,j,k) points[iXStart+i][iYStart+j][iZStart+k]
#define POSITION_X_AT(i) ((glmd::float32)(i*glr::terrain::constants::RESOLUTION) + (glmd::float32)(glr::terrain::constants::CHUNK_SIZE*gridX))
#define POSITION_Y_AT(i) ((glmd::float32)(i*glr::terrain::constants::RESOLUTION) + (glmd::float32)(glr::terrain::constants::CHUNK_SIZE*gridY))
#define POSITION_Z_AT(i) ((glmd::float32)(i*glr::terrain::constants::RESOLUTION) + (glmd::float32)(glr::terrain::constants::CHUNK_SIZE*gridZ))
	
	// If true, just uses the noise function
	bool useNoiseFunction = true;
	if (useNoiseFunction)
		return fieldFunction_->getNoise(x, y, z);
	
	if (gridX < 0)
		gridX += glr::terrain::constants::CHUNK_SIZE;
	if (gridY < 0)
		gridY += glr::terrain::constants::CHUNK_SIZE;
	if (gridZ < 0)
		gridZ += glr::terrain::constants::CHUNK_SIZE;
	
	gridX %= glr::terrain::constants::CHUNK_SIZE;
	gridY %= glr::terrain::constants::CHUNK_SIZE;
	gridZ %= glr::terrain::constants::CHUNK_SIZE;
	
	// Get x0 point
	glmd::int32 iXStart = 0;
	glmd::int32 iXEnd = 1;
	while (POSITION_X_AT(iXEnd) < x)
	{
		iXStart++;
		iXEnd++;
	}
	
	if (iXStart > glr::terrain::constants::CHUNK_SIZE+1)
	{
		std::cout << "UH-OH: We have a wierd error here..." << iXStart << std::endl;
		iXStart = glr::terrain::constants::CHUNK_SIZE+1;
	}
	
	// Get y0 point
	glmd::int32 iYStart = 0;
	glmd::int32 iYEnd = 1;
	while (POSITION_Y_AT(iYEnd) < y)
	{
		iYStart++;
		iYEnd++;
	}
	
	if (iYStart > glr::terrain::constants::CHUNK_SIZE+1)
	{
		std::cout << "UH-OH: We have a wierd error here..." << iYStart << std::endl;
		iYStart = glr::terrain::constants::CHUNK_SIZE+1;
	}
	
	// Get z0 point
	glmd::int32 iZStart = 0;
	glmd::int32 iZEnd = 1;
	while (POSITION_Z_AT(iZEnd) < z)
	{
		iZStart++;
		iZEnd++;
	}
	
	if (iZStart > glr::terrain::constants::CHUNK_SIZE+1)
	{
		std::cout << "UH-OH: We have a wierd error here..." << iZStart << std::endl;
		iZStart = glr::terrain::constants::CHUNK_SIZE+1;
	}

	// Determine all 8 points
	glmd::float64 dat[2][2][2];
	
	dat[0][0][0] = DENSITY_AT(0,0,0);
	dat[0][0][1] = DENSITY_AT(0,1,0);
	dat[1][0][1] = DENSITY_AT(1,1,0);
	dat[1][0][0] = DENSITY_AT(1,0,0);
	
	dat[0][1][0] = DENSITY_AT(0,0,1);
	dat[0][1][1] = DENSITY_AT(0,1,1);
	dat[1][1][1] = DENSITY_AT(1,1,1);
	dat[1][1][0] = DENSITY_AT(1,0,1);
	
	// Get (x, y, z) position relative to (x0, y0, z0)
	glmd::float32 xPosRel = x - POSITION_X_AT(iXStart);
	glmd::float32 yPosRel = y - POSITION_Y_AT(iYStart);
	glmd::float32 zPosRel = z - POSITION_Z_AT(iZStart);

	// Do interpolation	
	glmd::float64 retVal = trilinear_interp(xPosRel, yPosRel, zPosRel, (const glmd::float64*)&dat, 2, 2, 2, 0.0f, glr::terrain::constants::RESOLUTION, 0.0f, glr::terrain::constants::RESOLUTION, 0.0f, glr::terrain::constants::RESOLUTION, 0, 0.0f);
	glmd::float32 retValf = (glmd::float32) retVal;
	
	return retValf;
	
}

/**
 * Calculate the normal for the provided point.
 */
glm::vec3 VoxelChunkMeshGenerator::calculateNormal(const glm::vec3& point, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ, Points& densityValues) const
{
	glmd::float32 d = getInterpolatedNoise(densityValues, gridX, gridY, gridZ, point.x, point.y, point.z);
	glmd::float32 nx = getInterpolatedNoise(densityValues, gridX, gridY, gridZ, point.x + glr::terrain::constants::EPSILON_VALUE, point.y, point.z) - d;
	glmd::float32 ny = getInterpolatedNoise(densityValues, gridX, gridY, gridZ, point.x, point.y + glr::terrain::constants::EPSILON_VALUE, point.z) - d;
	glmd::float32 nz = getInterpolatedNoise(densityValues, gridX, gridY, gridZ, point.x, point.y, point.z + glr::terrain::constants::EPSILON_VALUE) - d;
	
	glm::vec3 normal = glm::vec3(nx, ny, nz);

	if (normal == glm::vec3(0.0f, 0.0f, 0.0f))
		return glm::vec3(0.0f, 1.0f, 0.0f);

	return glm::normalize( normal );
}

/**
 * Find the intersection along the x-axis where the line segment between p0 and p1 would intersect with the isosurface.
 */
void VoxelChunkMeshGenerator::intersectXAxis(Point& p0, Point& p1, Point& out, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ, Points& densityValues) const
{
	glmd::float32 fa, fb;
	glmd::float32 xa, xb;
	// p0 < 0  ,  p1 > 0
	if (std::signbit(p0.density))
	{
		fa = p0.density;
		fb = p1.density;
		xa = p0.pos.x;
		xb = p1.pos.x;
	}
	// p1 < 0  ,  p0 > 0
	else
	{
		fa = p1.density;
		fb = p0.density;
		xa = p1.pos.x;
		xb = p0.pos.x;
	}
	
	glmd::float32 y = p0.pos.y;
	glmd::float32 z = p0.pos.z;
	glmd::float32 xm;
	glmd::float32 density = 0.0f;
	glmd::float32 previousDensity = density;

	while (true)
	{
		xm = xb - (fb * (xb - xa) / (fb - fa));

		density = getInterpolatedNoise(densityValues, gridX, gridY, gridZ, xm, y, z);
		density += glr::terrain::constants::EPSILON_DENSITY;
		
		// Needed to add this check to the previous density.
		// Without it, this method was causing an infinite loop.
		// Not entirely sure WHY it was causing an inifinite loop, however.
		if (previousDensity == density)
			break;
		previousDensity = density;
		
		if (fabs(density) < glr::terrain::constants::TOLERANCE_DENSITY)
			break;
		if (fabs(xa - xb) < glr::terrain::constants::TOLERANCE_COORD)
			break;
		
		// pm is negative
		if (std::signbit(density))
		{
			xa = xm;
			fa = density;
		}
		// pm is positive
		else
		{
			xb = xm;
			fb = density;
		}

		xm = (xa + xb) * 0.5f;

		density = getInterpolatedNoise(densityValues, gridX, gridY, gridZ, xm, y, z);
		density += glr::terrain::constants::EPSILON_DENSITY;
		
		// pm < 0
		if (std::signbit(density))
		{
			xa = xm;
			fa = density;
		}
		// pm > 0
		else
		{
			xb = xm;
			fb = density;
		}
	}

	out.density = density;
	out.pos = glm::vec3(xm, y, z);
}


/**
 * Find the intersection along the y-axis where the line segment between p0 and p1 would intersect with the isosurface.
 */
void VoxelChunkMeshGenerator::intersectYAxis(Point& p0, Point& p1, Point& out, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ, Points& densityValues) const
{
	glmd::float32 fa, fb;
	glmd::float32 ya, yb;
	
	// p0 < 0  ,  p1 > 0
	if (std::signbit(p0.density))
	{
		fa = p0.density;
		fb = p1.density;
		ya = p0.pos.y;
		yb = p1.pos.y;
	}
	// p1 < 0  ,  p0 > 0
	else
	{
		fa = p1.density;
		fb = p0.density;
		ya = p1.pos.y;
		yb = p0.pos.y;
	}
	glmd::float32 x = p0.pos.x;
	glmd::float32 z = p0.pos.z;
	glmd::float32 ym;
	glmd::float32 density = 0.0f;
	glmd::float32 previousDensity = density;
	
	while (true)
	{
		ym = yb - (fb * (yb - ya) / (fb - fa));

		density = getInterpolatedNoise(densityValues, gridX, gridY, gridZ, x, ym, z);
		density += glr::terrain::constants::EPSILON_DENSITY;
		
		// Needed to add this check to the previous density.
		// Without it, this method was causing an infinite loop.
		// Not entirely sure WHY it was causing an inifinite loop, however.
		if (previousDensity == density)
			break;
		previousDensity = density;
		
		if (fabs(density) < glr::terrain::constants::TOLERANCE_DENSITY)
			break;
		if (fabs(ya - yb) < glr::terrain::constants::TOLERANCE_COORD)
			break;
		
		// pm is negative
		if (std::signbit(density))
		{
			ya = ym;
			fa = density;
		}
		// pm is positive
		else
		{
			yb = ym;
			fb = density;
		}

		ym = (ya + yb) * 0.5f;

		density = getInterpolatedNoise(densityValues, gridX, gridY, gridZ, x, ym, z);
		density += glr::terrain::constants::EPSILON_DENSITY;
		
		// pm < 0
		if (std::signbit(density))
		{
			ya = ym;
			fa = density;
		}
		// pm > 0
		else
		{
			yb = ym;
			fb = density;
		}
	}

	out.density = density;
	out.pos = glm::vec3(x, ym, z);
}


/**
 * Find the intersection along the z-axis where the line segment between p0 and p1 would intersect with the isosurface.
 */
void VoxelChunkMeshGenerator::intersectZAxis(Point& p0, Point& p1, Point& out, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ, Points& densityValues) const
{
	glmd::float32 fa, fb;
	glmd::float32 za, zb;
	
	// p0 < 0  ,  p1 > 0
	if (std::signbit(p0.density))
	{
		fa = p0.density;
		fb = p1.density;
		za = p0.pos.z;
		zb = p1.pos.z;
	}
	// p1 < 0  ,  p0 > 0
	else
	{
		fa = p1.density;
		fb = p0.density;
		za = p1.pos.z;
		zb = p0.pos.z;
	}
	glmd::float32 x = p0.pos.x;
	glmd::float32 y = p0.pos.y;
	glmd::float32 zm;
	glmd::float32 density = 0.0f;
	glmd::float32 previousDensity = density;

	while (true)
	{
		zm = zb - (fb * (zb - za) / (fb - fa));

		density = getInterpolatedNoise(densityValues, gridX, gridY, gridZ, x, y, zm);
		density += glr::terrain::constants::EPSILON_DENSITY;
		
		// Needed to add this check to the previous density.
		// Without it, this method was causing an infinite loop.
		// Not entirely sure WHY it was causing an inifinite loop, however.
		if (previousDensity == density)
			break;
		previousDensity = density;
		
		if (fabs(density) < glr::terrain::constants::TOLERANCE_DENSITY)
			break;
		if (fabs(za - zb) < glr::terrain::constants::TOLERANCE_COORD)
			break;
		
		// pm is negative
		if (std::signbit(density))
		{
			za = zm;
			fa = density;
		}
		// pm is positive
		else
		{
			zb = zm;
			fb = density;
		}

		zm = (za + zb) * 0.5f;

		density = getInterpolatedNoise(densityValues, gridX, gridY, gridZ, x, y, zm);
		density += glr::terrain::constants::EPSILON_DENSITY;
		
		// pm < 0
		if (std::signbit(density))
		{
			za = zm;
			fa = density;
		}
		// pm > 0
		else
		{
			zb = zm;
			fb = density;
		}
	}

	out.density = density;
	out.pos = glm::vec3(x, y, zm);
}

/**
 * Generate the vertex for the given block at position x, y, z.
 */
void VoxelChunkMeshGenerator::generateVertex(Blocks& blocks, glmd::int32 x, glmd::int32 y, glmd::int32 z, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ, Points& densityValues) const
{
	//
	// Part 1: Compute intersection points and their normals.
	//
	static glmd::int32 intersections[12][2][3] = {
		{ {0,0,0}, {1,0,0} }, { {1,0,0}, {1,1,0} }, { {1,1,0}, {0,1,0} }, { {0,1,0}, {0,0,0} }, { {0,0,1}, {1,0,1} }, { {1,0,1}, {1,1,1} },
		{ {1,1,1}, {0,1,1} }, { {0,1,1}, {0,0,1} }, { {0,0,0}, {0,0,1} }, { {1,0,0}, {1,0,1} }, { {1,1,0}, {1,1,1} }, { {0,1,0}, {0,1,1} }
	};

/*
	static glmd::int32 intersections[12][2] = {
		{ 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 }, { 4, 5 }, { 5, 6 },
		{ 6, 7 }, { 7, 4 }, { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 }
	};
*/
	glmd::int32 edgeInfo = edgeTable_[blocks[x][y][z].index];
	
	Point points[12];
	glm::vec3 normals[12];
	glm::vec3 massPoint;
	glmd::int32 numIntersections = 0;

// if (edgeInfo & (1 << n)) --> If the contour intersects the edge, process it
#define CALCULATE_NORMALS_AND_MASS_POINT(n)\
	if (edgeInfo & (1 << n))\
	{\
		glmd::int32 n1[3];\
		n1[0] = intersections[n][0][0];\
		n1[1] = intersections[n][0][1];\
		n1[2] = intersections[n][0][2];\
		glmd::int32 n2[3];\
		n2[0] = intersections[n][1][0];\
		n2[1] = intersections[n][1][1];\
		n2[2] = intersections[n][1][2];\
\
		if (fabs(blocks[x][y][z].points[n1[0]][n1[1]][n1[2]].density) < glr::terrain::constants::EPSILON_VALUE)\
			points[n] = blocks[x][y][z].points[n1[0]][n1[1]][n1[2]];\
		else if (fabs(blocks[x][y][z].points[n2[0]][n2[1]][n2[2]].density) < glr::terrain::constants::EPSILON_VALUE)\
			points[n] = blocks[x][y][z].points[n2[0]][n2[1]][n2[2]];\
		else if (fabs(blocks[x][y][z].points[n1[0]][n1[1]][n1[2]].density - blocks[x][y][z].points[n2[0]][n2[1]][n2[2]].density) < glr::terrain::constants::EPSILON_VALUE)\
			points[n] = blocks[x][y][z].points[n1[0]][n1[1]][n1[2]];\
		else\
		{\
			glm::vec3 vDiff = blocks[x][y][z].points[n1[0]][n1[1]][n1[2]].pos - blocks[x][y][z].points[n2[0]][n2[1]][n2[2]].pos;\
			vDiff = glm::vec3( fabs(vDiff.x), fabs(vDiff.y), fabs(vDiff.z) );\
			if (vDiff.x > glr::terrain::constants::EPSILON_VALUE)\
				intersectXAxis(blocks[x][y][z].points[n1[0]][n1[1]][n1[2]], blocks[x][y][z].points[n2[0]][n2[1]][n2[2]], points[n], gridX, gridY, gridZ, densityValues);\
			else if (vDiff.y > glr::terrain::constants::EPSILON_VALUE)\
				intersectYAxis(blocks[x][y][z].points[n1[0]][n1[1]][n1[2]], blocks[x][y][z].points[n2[0]][n2[1]][n2[2]], points[n], gridX, gridY, gridZ, densityValues);\
			else if (vDiff.z > glr::terrain::constants::EPSILON_VALUE)\
				intersectZAxis(blocks[x][y][z].points[n1[0]][n1[1]][n1[2]], blocks[x][y][z].points[n2[0]][n2[1]][n2[2]], points[n], gridX, gridY, gridZ, densityValues);\
		}\
\
		normals[n] = calculateNormal(points[n].pos, gridX, gridY, gridZ, densityValues);\
\
		massPoint += points[n].pos;\
		numIntersections++;\
	}

/*
CORNER(0,  0,0,0);
CORNER(1,  1,0,0);
CORNER(2,  1,1,0);
CORNER(3,  0,1,0);
CORNER(4,  0,0,1);
CORNER(5,  1,0,1);
CORNER(6,  1,1,1);
CORNER(7,  0,1,1);
 */
	CALCULATE_NORMALS_AND_MASS_POINT(0)
	CALCULATE_NORMALS_AND_MASS_POINT(1)
	CALCULATE_NORMALS_AND_MASS_POINT(2)
	CALCULATE_NORMALS_AND_MASS_POINT(3)
	CALCULATE_NORMALS_AND_MASS_POINT(4)
	CALCULATE_NORMALS_AND_MASS_POINT(5)
	CALCULATE_NORMALS_AND_MASS_POINT(6)
	CALCULATE_NORMALS_AND_MASS_POINT(7)
	CALCULATE_NORMALS_AND_MASS_POINT(8)
	CALCULATE_NORMALS_AND_MASS_POINT(9)
	CALCULATE_NORMALS_AND_MASS_POINT(10)
	CALCULATE_NORMALS_AND_MASS_POINT(11)
	
	massPoint /= (glmd::float32)numIntersections;

	//
	// Part 2: Compute the QEF-minimizing point
	//
	
	glm::vec3 newPointNormal = glm::vec3(0.0f, 0.0f, 0.0f);
	
	glmd::float64 matrix[12][3];
	glmd::float64 vector[12];
	glmd::int32 rows = 0;

	for (glmd::int32 i = 0; i < 12; i++)
	{
		// If the contour does not intersect the edge, skip it
		if (! (edgeInfo & (1 << i)))
			continue;

		const glm::vec3& normal = normals[i];

		matrix[rows][0] = normal.x;
		matrix[rows][1] = normal.y;
		matrix[rows][2] = normal.z;

		glm::vec3 p = points[i].pos - massPoint;
		vector[rows] = (glmd::float64)glm::dot(normal, p);

		rows++;

		newPointNormal += normal;
	}
	
	glm::vec3 newPointV = evaluate(matrix, vector, rows);
	
	newPointV += massPoint;

	blocks[x][y][z].meshPoint = newPointV;
	if (newPointNormal == glm::vec3(0.0f, 0.0f, 0.0f))
		newPointNormal = glm::vec3(0.0f, 1.0f, 0.0f);
	blocks[x][y][z].meshPointNormal = glm::normalize( newPointNormal );
}

/**
 * Determine if the cubes along the xz plane at point y have an intersection.  If a cube does, generate the vertex for that block.
 */
void VoxelChunkMeshGenerator::computeCubes(Blocks& blocks, glmd::int32 y, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ, Points& densityValues) const
{	
	for (glmd::int32 x = 0; x < glr::terrain::constants::SIZE+1; x++)
	{
		for (glmd::int32 z = 0; z < glr::terrain::constants::SIZE+1; z++)
		{
			glmd::int32 index = 0;

#define IF_DENSITY(n,i,j,k) if (! std::signbit(blocks[x][y][z].points[i][j][k].density)) index |= (1 << n)
			
			// We find all of the vertices of the block that lie outside of the contour
			IF_DENSITY(0, 0,0,0);
			IF_DENSITY(1, 1,0,0);
			IF_DENSITY(2, 1,1,0);
			IF_DENSITY(3, 0,1,0);
			IF_DENSITY(4, 0,0,1);
			IF_DENSITY(5, 1,0,1);
			IF_DENSITY(6, 1,1,1);
			IF_DENSITY(7, 0,1,1);
#undef IF_DENSITY
			
			// What I think this is doing:
			// If the configuration of vertices that lie outside the contour does not match any of the
			// special cases in the edge table, we don't need to create a vertex for this block
			if (edgeTable_[index] == 0)
			{
				blocks[x][y][z].index = 0;
				continue;
			}
			
			blocks[x][y][z].index = index;
			generateVertex(blocks, x, y, z, gridX, gridY, gridZ, densityValues);
		}

	}
}

/**
 * Generate the 3 points for a triangle along the y coordinate (will move along the xz plane at point y).  Will generate
 * up to 3 points per block.
 * 
 */
void VoxelChunkMeshGenerator::generateTriangles(Blocks& blocks, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, glmd::int32 y) const
{
	Block blocksTemp[4];

	for (glmd::int32 x = 0; x < glr::terrain::constants::SIZE; x++)
	{
		for (glmd::int32 z = 0; z < glr::terrain::constants::SIZE; z++)
		{
			blocksTemp[0] = blocks[x][y][z];
			glmd::int32 cube0_edgeInfo = edgeTable_[blocksTemp[0].index];
			glmd::int32 flip_if_nonzero = 0;

			for (glmd::int32 i = 0; i < 3; i++)
			{
				if (i == 0 && cube0_edgeInfo & (1 << 10))
				{
					blocksTemp[1] = blocks[x+1][y][z];
					blocksTemp[2] = blocks[x+1][y+1][z];
					blocksTemp[3] = blocks[x][y+1][z];
					flip_if_nonzero = (blocksTemp[0].index & (1 << 6));
				} else if (i == 1 && cube0_edgeInfo & (1 << 6))
				{
					blocksTemp[1] = blocks[x][y][z+1];
					blocksTemp[2] = blocks[x][y+1][z+1];
					blocksTemp[3] = blocks[x][y+1][z];
					flip_if_nonzero = (blocksTemp[0].index & (1 << 7));
				} else if (i == 2 && cube0_edgeInfo & (1 << 5))
				{
					blocksTemp[1] = blocks[x+1][y][z];
					blocksTemp[2] = blocks[x+1][y][z+1];
					blocksTemp[3] = blocks[x][y][z+1];
					flip_if_nonzero = (blocksTemp[0].index & (1 << 5));
				} else
					continue;

				// create triangles (cube0,cube2,cube1)
				//			  and (cube0,cube3,cube2)
				// flipping last two vertices if necessary

				auto p0 = blocksTemp[0].meshPoint;
				auto n0 = blocksTemp[0].meshPointNormal;
				//const Isosurface::Material &mat0 = blocksTemp[0]->mat;

				for (glmd::int32 j = 1; j < 3; j++)
				{
					glmd::int32 ja = 0;
					glmd::int32 jb = 0;
					if (flip_if_nonzero)
					{
						ja = j + 0;
						jb = j + 1;
					} else
					{
						ja = j + 1;
						jb = j + 0;
					}

					auto p1 = blocksTemp[ja].meshPoint;
					auto p2 = blocksTemp[jb].meshPoint;
					auto n1 = blocksTemp[ja].meshPointNormal;
					auto n2 = blocksTemp[jb].meshPointNormal;

					vertices.push_back( p2 );
					vertices.push_back( p1 );
					vertices.push_back( p0 );
					
					normals.push_back( n2 );
					normals.push_back( n1 );
					normals.push_back( n0 );					
				}
			}
		}
	}
}

/**
 * Set the densities and positions for the blocks, using the provided points (density values) and the grid coordinates.
 */
void VoxelChunkMeshGenerator::setDensitiesAndPositions(Blocks& blocks, Points& points, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ) const
{
	// Set the starting x, y, z
	glmd::float32 fx = (glmd::float32)(gridX * glr::terrain::constants::SIZE * glr::terrain::constants::RESOLUTION);
	glmd::float32 fy = (glmd::float32)(gridY * glr::terrain::constants::SIZE * glr::terrain::constants::RESOLUTION);
	glmd::float32 fz = (glmd::float32)(gridZ * glr::terrain::constants::SIZE * glr::terrain::constants::RESOLUTION);

#define SET_DENSITY(x, y, z, i, j, k) blocks[x][y][z].points[i][j][k].density = points[x+i][y+j][z+k];
#define SET_POSITION(x, y, z, i, j, k)\
	blocks[x][y][z].points[i][j][k].pos.x = fx + (glmd::float32)(i * glr::terrain::constants::RESOLUTION);\
	blocks[x][y][z].points[i][j][k].pos.y = fy + (glmd::float32)(j * glr::terrain::constants::RESOLUTION);\
	blocks[x][y][z].points[i][j][k].pos.z = fz + (glmd::float32)(k * glr::terrain::constants::RESOLUTION);
	
	// Set the densities and positions
	for (glmd::int32 x=0; x < glr::terrain::constants::SIZE+1; x++)
	{
		for (glmd::int32 y=0; y < glr::terrain::constants::SIZE+1; y++)
		{
			for (glmd::int32 z=0; z < glr::terrain::constants::SIZE+1; z++)
			{
				SET_DENSITY(x, y, z, 0,0,0)
				SET_DENSITY(x, y, z, 1,0,0)
				SET_DENSITY(x, y, z, 0,1,0)
				SET_DENSITY(x, y, z, 0,0,1)
				SET_DENSITY(x, y, z, 1,0,1)
				SET_DENSITY(x, y, z, 1,1,0)
				SET_DENSITY(x, y, z, 0,1,1)
				SET_DENSITY(x, y, z, 1,1,1)
				
				SET_POSITION(x, y, z, 0,0,0)
				SET_POSITION(x, y, z, 1,0,0)
				SET_POSITION(x, y, z, 0,1,0)
				SET_POSITION(x, y, z, 0,0,1)
				SET_POSITION(x, y, z, 1,0,1)
				SET_POSITION(x, y, z, 1,1,0)
				SET_POSITION(x, y, z, 0,1,1)
				SET_POSITION(x, y, z, 1,1,1)
				
				fz += glr::terrain::constants::RESOLUTION;
			}
			fy += glr::terrain::constants::RESOLUTION;
			fz = (glmd::float32)(gridZ * glr::terrain::constants::SIZE * glr::terrain::constants::RESOLUTION);
		}
		fx += glr::terrain::constants::RESOLUTION;
		fy = (glmd::float32)(gridY * glr::terrain::constants::SIZE * glr::terrain::constants::RESOLUTION);
		fz = (glmd::float32)(gridZ * glr::terrain::constants::SIZE * glr::terrain::constants::RESOLUTION);
	}
}

/**
 * Will resize the provided Blocks 3D vector to the appropriate size.
 * 
 * @param blocks
 */
void VoxelChunkMeshGenerator::resizeBlocks(Blocks& blocks) const
{
	blocks.resize(glr::terrain::constants::SIZE+1);
	for (glmd::int32 i = 0; i < glr::terrain::constants::SIZE+1; i++)
	{
		blocks[i].resize(glr::terrain::constants::SIZE+1);
		for (glmd::int32 j = 0; j < glr::terrain::constants::SIZE+1; j++)
			blocks[i][j].resize(glr::terrain::constants::SIZE+1);
	}
}

void VoxelChunkMeshGenerator::generateMesh(VoxelChunk& chunk, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec4>& textureBlendingValues) const
{	
	Points points = chunk.getPoints();
	glmd::int32 gridX = chunk.getGridX();
	glmd::int32 gridY = chunk.getGridY();
	glmd::int32 gridZ = chunk.getGridZ();
	
	if (isEmptyOrSolid(points))
		return;
	
	// Make our blocks array the proper size
	auto blocks = Blocks();
	
	resizeBlocks(blocks);
	
	setDensitiesAndPositions(blocks, points, gridX, gridY, gridZ);

	computeCubes(blocks, 0, gridX, gridY, gridZ, points);
	
	for (glmd::int32 y = 1; y < glr::terrain::constants::SIZE+1; y++)
	{
		computeCubes(blocks, y, gridX, gridY, gridZ, points);
		
		generateTriangles(blocks, vertices, normals, y-1);
	}
	
	// Calculate texture blending values
	textureBlendingValues.resize( vertices.size() );
	for ( glmd::uint32 i=0; i < textureBlendingValues.size(); i++)
	{
		auto& v = textureBlendingValues[i];
		auto& n = normals[i];
		
		if (n.y > 0.5f)
		{
			v.x = 1.0f;
			v.y = 0.0f;
		}
		else
		{
			v.x = 0.0f;
			v.y = 1.0f;
		}
		
		v.z = 1.0f;
		v.a = 1.0f;
	}	
}

}
}
}
