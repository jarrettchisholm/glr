//----------------------------------------------------------------------------
// ThreeD Quadric Error Function
//----------------------------------------------------------------------------

#ifndef QEF_H
#define QEF_H

#include <string.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace glr
{
namespace terrain
{
namespace dual_contouring
{

namespace glmd = glm::detail;

/**
 * QEF, a class implementing the quadric error function
 *	  E[x] = P - Ni . Pi
 *
 * Given at least three points Pi, each with its respective
 * normal vector Ni, that describe at least two planes,
 * the QEF evalulates to the point x.
 */

glm::vec3 evaluate(glmd::float64 mat[][3], glmd::float64 *vec, glmd::int32 rows);

// compute svd

void computeSVD(
	glmd::float64 mat[][3],			// matrix (rows x 3)
	glmd::float64 u[][3],				// matrix (rows x 3)
	glmd::float64 v[3][3],				// matrix (3x3)
	glmd::float64 d[3],				// vector (1x3)
	glmd::int32 rows);

// factorize

void factorize(
	glmd::float64 mat[][3],				// matrix (rows x 3)
	glmd::float64 tau_u[3],				// vector (1x3)
	glmd::float64 tau_v[2],				// vectors, (1x2)
	glmd::int32 rows);

glmd::float64 factorize_hh(glmd::float64 *ptrs[], glmd::int32 n);

// unpack

void unpack(
	glmd::float64 u[][3],				// matrix (rows x 3)
	glmd::float64 v[3][3],				// matrix (3x3)
	glmd::float64 tau_u[3],			// vector, (1x3)
	glmd::float64 tau_v[2],			// vector, (1x2)
	glmd::int32 rows);

// diagonalize

void diagonalize(
	glmd::float64 u[][3],				// matrix (rows x 3)
	glmd::float64 v[3][3],				// matrix (3x3)
	glmd::float64 tau_u[3],			// vector, (1x3)
	glmd::float64 tau_v[2],			// vector, (1x2)
	glmd::int32 rows);

void chop(glmd::float64 *a, glmd::float64 *b, glmd::int32 n);

void qrstep(
	glmd::float64 u[][3],				 // matrix (rows x cols)
	glmd::float64 v[][3],				 // matrix (3 x cols)
	glmd::float64 tau_u[],				 // vector (1 x cols)
	glmd::float64 tau_v[],				 // vector (1 x cols - 1)
	glmd::int32 rows, glmd::int32 cols);

void qrstep_middle(
	glmd::float64 u[][3],				 // matrix (rows x cols)
	glmd::float64 tau_u[],				 // vector (1 x cols)
	glmd::float64 tau_v[],				 // vector (1 x cols - 1)
	glmd::int32 rows, glmd::int32 cols, glmd::int32 col);

void qrstep_end(
	glmd::float64 v[][3],				 // matrix (3 x 3)
	glmd::float64 tau_u[],				 // vector (1 x 3)
	glmd::float64 tau_v[],				 // vector (1 x 2)
	glmd::int32 cols);

glmd::float64 qrstep_eigenvalue(
	glmd::float64 tau_u[],				 // vector (1 x 3)
	glmd::float64 tau_v[],				 // vector (1 x 2)
	glmd::int32 cols);

void qrstep_cols2(
	glmd::float64 u[][3],				 // matrix (rows x 2)
	glmd::float64 v[][3],				 // matrix (3 x 2)
	glmd::float64 tau_u[],				 // vector (1 x 2)
	glmd::float64 tau_v[],				 // vector (1 x 1)
	glmd::int32 rows);

void computeGivens(glmd::float64 a, glmd::float64 b, glmd::float64 *c, glmd::float64 *s);

void computeSchur(glmd::float64 a1, glmd::float64 a2, glmd::float64 a3, glmd::float64 *c, glmd::float64 *s);

// singularize
void singularize(
	glmd::float64 u[][3],				// matrix (rows x 3)
	glmd::float64 v[3][3],				// matrix (3x3)
	glmd::float64 d[3],				// vector, (1x3)
	glmd::int32 rows);

// solve svd
void solveSVD(
	glmd::float64 u[][3],				// matrix (rows x 3)
	glmd::float64 v[3][3],				// matrix (3x3)
	glmd::float64 d[3],				// vector (1x3)
	glmd::float64 b[],					// vector (1 x rows)
	glmd::float64 x[3],				// vector (1x3)
	glmd::int32 rows);

}
}
}

#endif // QEF_H
