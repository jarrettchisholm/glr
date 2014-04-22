#include <iostream>
#include <sstream>
#include <fstream>
#include <mutex>

#include "terrain/marching_cubes/VoxelChunkMeshGenerator.hpp"

#include "terrain/Interpolation.hpp"

#include "terrain/Constants.hpp"

#define ABS(x) (x < 0 ? -(x) : (x))
#define NX 200
#define NY 160
#define NZ 160
#define ISOLEVEL 0

namespace glr
{
namespace terrain
{

VoxelChunkMeshGenerator::VoxelChunkMeshGenerator()
{
}

VoxelChunkMeshGenerator::~VoxelChunkMeshGenerator()
{
}

/*
	int edgeTable[256].  It corresponds to the 2^8 possible combinations of
	of the eight (n) vertices either existing inside or outside (2^n) of the
	surface.  A vertex is inside of a surface if the value at that vertex is
	less than that of the surface you are scanning for.  The table index is
	constructed bitwise with bit 0 corresponding to vertex 0, bit 1 to vert
	1.. bit 7 to vert 7.  The value in the table tells you which edges of
	the table are intersected by the surface.  Once again bit 0 corresponds
	to edge 0 and so on, up to edge 12.
	Constructing the table simply consisted of having a program run thru
	the 256 cases and setting the edge bit if the vertices at either end of
	the edge had different values (one is inside while the other is out).
	The purpose of the table is to speed up the scanning process.  Only the
	edges whose bit's are set contain vertices of the surface.
	Vertex 0 is on the bottom face, back edge, left side.
	The progression of vertices is clockwise around the bottom face
	and then clockwise around the top face of the cube.  Edge 0 goes from
	vertex 0 to vertex 1, Edge 1 is from 2->3 and so on around clockwise to
	vertex 0 again. Then Edge 4 to 7 make up the top face, 4->5, 5->6, 6->7
	and 7->4.  Edge 8 thru 11 are the vertical edges from vert 0->4, 1->5,
	2->6, and 3->7.
       4--------5     *---4----*
      /|       /|    /|       /|
     / |      / |   7 |      5 |
    /  |     /  |  /  8     /  9
   7--------6   | *----6---*   |
   |   |    |   | |   |    |   |
   |   0----|---1 |   *---0|---*
   |  /     |  /  11 /     10 /
   | /      | /   | 3      | 1
   |/       |/    |/       |/
   3--------2     *---2----*
*/
int edgeTable[256] = {
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

/*
	int triTable[256][16] also corresponds to the 256 possible combinations
	of vertices.
	The [16] dimension of the table is again the list of edges of the cube
	which are intersected by the surface.  This time however, the edges are
	enumerated in the order of the vertices making up the triangle mesh of
	the surface.  Each edge contains one vertex that is on the surface.
	Each triple of edges listed in the table contains the vertices of one
	triangle on the mesh.  The are 16 entries because it has been shown that
	there are at most 5 triangles in a cube and each "edge triple" list is
	terminated with the value -1.
	For example triTable[3] contains
	{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
	This corresponds to the case of a cube whose vertex 0 and 1 are inside
	of the surface and the rest of the verts are outside (00000001 bitwise
	OR'ed with 00000010 makes 00000011 == 3).  Therefore, this cube is
	intersected by the surface roughly in the form of a plane which cuts
	edges 8,9,1 and 3.  This quadrilateral can be constructed from two
	triangles: one which is made of the intersection vertices found on edges
	1,8, and 3; the other is formed from the vertices on edges 9,8, and 1.
	Remember, each intersected edge contains only one surface vertex.  The
	vertex triples are listed in counter clockwise order for proper facing.
*/
int triTable[256][16] = {
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
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
 * Calculate the normal for the provided point.
 */
glm::vec3 VoxelChunkMeshGenerator::calculateNormal(const glm::vec3& point, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ, Points& densityValues) const
{
	/*
	glmd::float32 d = getInterpolatedNoise(densityValues, gridX, gridY, gridZ, point.x, point.y, point.z);
	glmd::float32 nx = getInterpolatedNoise(densityValues, gridX, gridY, gridZ, point.x + glr::terrain::constants::EPSILON_VALUE, point.y, point.z) - d;
	glmd::float32 ny = getInterpolatedNoise(densityValues, gridX, gridY, gridZ, point.x, point.y + glr::terrain::constants::EPSILON_VALUE, point.z) - d;
	glmd::float32 nz = getInterpolatedNoise(densityValues, gridX, gridY, gridZ, point.x, point.y, point.z + glr::terrain::constants::EPSILON_VALUE) - d;
	
	glm::vec3 normal = glm::vec3(nx, ny, nz);

	if (normal == glm::vec3(0.0f, 0.0f, 0.0f))
		return glm::vec3(0.0f, 1.0f, 0.0f);

	return glm::normalize( normal );
	*/
	
	return glm::vec3();
}

/**
 * Determine if the cubes along the xz plane at point y have an intersection.  If a cube does, generate the vertex for that block.
 */
void VoxelChunkMeshGenerator::computeCubes(Blocks& blocks, glmd::int32 y, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ, Points& densityValues) const
{	
	/*
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
	*/
}

/*
typedef struct {
	XYZ p[8];
	XYZ n[8];
	double val[8];
} GRIDCELL;

typedef struct {
	XYZ p[3];			// Vertices
	XYZ c;				// Centroid
	XYZ n[3];			// Normal
} TRIANGLE;
*/

/*
struct Point
{
	glm::vec3 pos;
	glm::vec3 normal;
	glmd::float32 density;
};

struct Block
{
	Point points[2][2][2];
};
*/

/**
 * Generate the triangles for cubes (aka 'blocks') along the y coordinate (will move along the xz plane at point y).
 * 
 */
void VoxelChunkMeshGenerator::generateTriangles(Blocks& blocks, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, glmd::int32 y) const
{
	// GRIDCELL g, double iso, TRIANGLE *tri
	
	for (glmd::int32 x = 0; x < glr::terrain::constants::SIZE; x++)
	{
		for (glmd::int32 z = 0; z < glr::terrain::constants::SIZE; z++)
		{
			const Block& block = blocks[x][y][z];
			
			int i = 0;
			int ntri = 0;
			int cubeindex = 0;
			glm::vec3 vertlist[12];
			
			/*
				Determine the index into the edge table which
				tells us which vertices are inside of the surface
			*/
			cubeindex = 0;
			/*
			if (block.points[0][0][1].density < ISOLEVEL) cubeindex |= 1;
			if (block.points[1][0][1].density < ISOLEVEL) cubeindex |= 2;
			if (block.points[1][0][0].density < ISOLEVEL) cubeindex |= 4;
			if (block.points[0][0][0].density < ISOLEVEL) cubeindex |= 8;
			if (block.points[0][1][1].density < ISOLEVEL) cubeindex |= 16;
			if (block.points[1][1][1].density < ISOLEVEL) cubeindex |= 32;
			if (block.points[1][1][0].density < ISOLEVEL) cubeindex |= 64;
			if (block.points[0][1][0].density < ISOLEVEL) cubeindex |= 128;
			*/
			if (block.points[0][0][1].density < ISOLEVEL) cubeindex |= 1;
			if (block.points[1][0][1].density < ISOLEVEL) cubeindex |= 2;
			if (block.points[1][0][0].density < ISOLEVEL) cubeindex |= 4;
			if (block.points[0][0][0].density < ISOLEVEL) cubeindex |= 8;
			if (block.points[0][1][1].density < ISOLEVEL) cubeindex |= 16;
			if (block.points[1][1][1].density < ISOLEVEL) cubeindex |= 32;
			if (block.points[1][1][0].density < ISOLEVEL) cubeindex |= 64;
			if (block.points[0][1][0].density < ISOLEVEL) cubeindex |= 128;
			
			/* Cube is entirely in/out of the surface */
			if (edgeTable[cubeindex] == 0)
				continue;
		
			/* Find the vertices where the surface intersects the cube */
			if (edgeTable[cubeindex] & 1) {
				vertlist[0] = vertexInterp(ISOLEVEL, block.points[0][0][1].pos, block.points[1][0][1].pos, block.points[0][0][1].density, block.points[1][0][1].density);
			}
			if (edgeTable[cubeindex] & 2) {
				vertlist[1] = vertexInterp(ISOLEVEL, block.points[1][0][1].pos, block.points[1][0][0].pos, block.points[1][0][1].density, block.points[1][0][0].density);
			}
			if (edgeTable[cubeindex] & 4) {
				vertlist[2] = vertexInterp(ISOLEVEL, block.points[1][0][0].pos, block.points[0][0][0].pos, block.points[1][0][0].density, block.points[0][0][0].density);
			}
			if (edgeTable[cubeindex] & 8) {
				vertlist[3] = vertexInterp(ISOLEVEL, block.points[0][0][0].pos, block.points[0][0][1].pos, block.points[0][0][0].density, block.points[0][0][1].density);
			}
			if (edgeTable[cubeindex] & 16) {
				vertlist[4] = vertexInterp(ISOLEVEL, block.points[0][1][1].pos, block.points[1][1][1].pos, block.points[0][1][1].density, block.points[1][1][1].density);
			}
			if (edgeTable[cubeindex] & 32) {
				vertlist[5] = vertexInterp(ISOLEVEL, block.points[1][1][1].pos, block.points[1][1][0].pos, block.points[1][1][1].density, block.points[1][1][0].density);
			}
			if (edgeTable[cubeindex] & 64) {
				vertlist[6] = vertexInterp(ISOLEVEL, block.points[1][1][0].pos, block.points[0][1][0].pos, block.points[1][1][0].density, block.points[0][1][0].density);
			}
			if (edgeTable[cubeindex] & 128) {
				vertlist[7] = vertexInterp(ISOLEVEL, block.points[0][1][0].pos, block.points[0][1][1].pos, block.points[0][1][0].density, block.points[0][1][1].density);
			}
			if (edgeTable[cubeindex] & 256) {
				vertlist[8] = vertexInterp(ISOLEVEL, block.points[0][0][1].pos, block.points[0][1][1].pos, block.points[0][0][1].density, block.points[0][1][1].density);
			}
			if (edgeTable[cubeindex] & 512) {
				vertlist[9] = vertexInterp(ISOLEVEL, block.points[1][0][1].pos, block.points[1][1][1].pos, block.points[1][0][1].density, block.points[1][1][1].density);
			}
			if (edgeTable[cubeindex] & 1024) {
				vertlist[10] = vertexInterp(ISOLEVEL, block.points[1][0][0].pos, block.points[1][1][0].pos, block.points[1][0][0].density, block.points[1][1][0].density);
			}
			if (edgeTable[cubeindex] & 2048) {
				vertlist[11] = vertexInterp(ISOLEVEL, block.points[0][0][0].pos, block.points[0][1][0].pos, block.points[0][0][0].density, block.points[0][1][0].density);
			}

			/* Create the triangles */
			for (int i=0; triTable[cubeindex][i] != -1; i += 3)
			{
				const glm::vec3& p1 = vertlist[triTable[cubeindex][i  ]];
				const glm::vec3& p2 = vertlist[triTable[cubeindex][i+1]];
				const glm::vec3& p3 = vertlist[triTable[cubeindex][i+2]];
				
				vertices.push_back( p1 );
				vertices.push_back( p2 );
				vertices.push_back( p3 );
				ntri++;
				
				// TODO: Figure out how to get the normals
				glm::vec3 normal = glm::normalize( glm::cross(p2 - p1, p3 - p1) );
				normals.push_back( normal );
				normals.push_back( normal );
				normals.push_back( normal );
			}
			
			// return(ntri);
		}
	}
	
	/*
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
	*/
}

/*-------------------------------------------------------------------------
	Return the point between two points in the same ratio as
	isolevel is between valp1 and valp2
*/
glm::vec3 VoxelChunkMeshGenerator::vertexInterp(double isolevel, const glm::vec3& p1, const glm::vec3& p2, double valp1, double valp2) const
{
	double errorMargin = 0.00001;

	if (ABS(isolevel-valp1) < errorMargin)
		return p1;
	if (ABS(isolevel-valp2) < errorMargin)
		return p2;
	if (ABS(valp1-valp2) < errorMargin)
		return p1;

	double mu = (isolevel - valp1) / (valp2 - valp1);
	
	glm::vec3 p = glm::vec3();
	p.x = p1.x + mu * (p2.x - p1.x);
	p.y = p1.y + mu * (p2.y - p1.y);
	p.z = p1.z + mu * (p2.z - p1.z);

	return p;
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
	/*
	computeCubes(blocks, 0, gridX, gridY, gridZ, points);
	
	for (glmd::int32 y = 1; y < glr::terrain::constants::SIZE+1; y++)
	{
		computeCubes(blocks, y, gridX, gridY, gridZ, points);
		
		generateTriangles(blocks, vertices, normals, y-1);
	}
	*/
	for (glmd::int32 y = 1; y < glr::terrain::constants::SIZE+1; y++)
	{
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
