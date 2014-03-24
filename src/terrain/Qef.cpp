//----------------------------------------------------------------------------
// ThreeD Quadric Error Function
//----------------------------------------------------------------------------

#include "terrain/Qef.hpp"

namespace glr
{
namespace terrain
{

//----------------------------------------------------------------------------

#define MAXROWS 12
#define EPSILON 1e-5

//----------------------------------------------------------------------------

glm::vec3 evaluate( glmd::float64 mat[][3], glmd::float64 *vec, glmd::int32 rows )
{
	// perform singular value decomposition on matrix mat
	// into u, v and d.
	// u is a matrix of rows x 3 (same as mat);
	// v is a square matrix 3 x 3 (for 3 columns in mat);
	// d is vector of 3 values representing the diagonal
	// matrix 3 x 3 (for 3 colums in mat).
	glmd::float64 u[MAXROWS][3], v[3][3], d[3];
	computeSVD(mat, u, v, d, rows);

	// solve linear system given by mat and vec using the
	// singular value decomposition of mat into u, v and d.
	if (d[2] < 0.1)
		d[2] = 0.0;
	if (d[1] < 0.1)
		d[1] = 0.0;
	if (d[0] < 0.1)
		d[0] = 0.0;

	glmd::float64 x[3];
	solveSVD(u, v, d, vec, x, rows);

	return glm::vec3((float)x[0], (float)x[1], (float)x[2]);
}

//----------------------------------------------------------------------------

void computeSVD(
	glmd::float64 mat[][3],				// matrix (rows x 3)
	glmd::float64 u[][3],				  // matrix (rows x 3)
	glmd::float64 v[3][3],				 // matrix (3x3)
	glmd::float64 d[3],					// vector (1x3)
	glmd::int32 rows)
{
	memcpy(u, mat, rows * 3 * sizeof(glmd::float64));

	glmd::float64 *tau_u = d;
	glmd::float64 tau_v[2];

	factorize(u, tau_u, tau_v, rows);

	unpack(u, v, tau_u, tau_v, rows);

	diagonalize(u, v, tau_u, tau_v, rows);

	singularize(u, v, tau_u, rows);
}

//----------------------------------------------------------------------------

void factorize(
	glmd::float64 mat[][3],				// matrix (rows x 3)
	glmd::float64 tau_u[3],				// vector, (1x3)
	glmd::float64 tau_v[2],				// vector, (1x2)
	glmd::int32 rows)
{
	glmd::int32 y;

	// bidiagonal factorization of (rows x 3) matrix into :-
	// tau_u, a vector of 1x3 (for 3 columns in the matrix)
	// tau_v, a vector of 1x2 (one less column than the matrix)

	for (glmd::int32 i = 0; i < 3; ++i) {

		// set up a vector to reference into the matrix
		// from mat(i,i) to mat(m,i), that is, from the
		// i'th column of the i'th row and down all the way
		// through that column
		glmd::float64 *ptrs[MAXROWS];
		glmd::int32 num_ptrs = rows - i;
		for (glmd::int32 q = 0; q < num_ptrs; ++q)
			ptrs[q] = &mat[q + i][i];

		// perform householder transformation on this vector
		glmd::float64 tau = factorize_hh(ptrs, num_ptrs);
		tau_u[i] = tau;

		// all computations below this point are performed
		// only for the first two columns:  i=0 or i=1
		if (i + 1 < 3) {

			// perform householder transformation on the matrix
			// mat(i,i+1) to mat(m,n), that is, on the sub-matrix
			// that begins in the (i+1)'th column of the i'th
			// row and extends to the end of the matrix at (m,n)
			if (tau != 0.0) {
				for (glmd::int32 x = i + 1; x < 3; ++x) {
					glmd::float64 wx = mat[i][x];
					for (y = i + 1; y < rows; ++y)
						wx += mat[y][x] * (*ptrs[y - i]);
					glmd::float64 tau_wx = tau * wx;
					mat[i][x] -= tau_wx;
					for (y = i + 1; y < rows; ++y)
						mat[y][x] -= tau_wx * (*ptrs[y - i]);
				}
			}

			// perform householder transformation on i'th row
			// (remember at this point, i is either 0 or 1)

			// set up a vector to reference into the matrix
			// from mat(i,i+1) to mat(i,n), that is, from the
			// (i+1)'th column of the i'th row and all the way
			// through to the end of that row
			ptrs[0] = &mat[i][i + 1];
			if (i == 0) {
				ptrs[1] = &mat[i][i + 2];
				num_ptrs = 2;
			} else // i == 1
				num_ptrs = 1;

			// perform householder transformation on this vector
			tau = factorize_hh(ptrs, num_ptrs);
			tau_v[i] = tau;

			// perform householder transformation on the sub-matrix
			// mat(i+1,i+1) to mat(m,n), that is, on the sub-matrix
			// that begins in the (i+1)'th column of the (i+1)'th
			// row and extends to the end of the matrix at (m,n)
			if (tau != 0.0) {
				for (y = i + 1; y < rows; ++y) {
					glmd::float64 wy = mat[y][i + 1];
					if (i == 0)
						wy += mat[y][i + 2] * (*ptrs[1]);
					glmd::float64 tau_wy = tau * wy;
					mat[y][i + 1] -= tau_wy;
					if (i == 0)
						mat[y][i + 2] -= tau_wy * (*ptrs[1]);
				}
			}

		}  // if (i + 1 < 3)
	}
}

//----------------------------------------------------------------------------

glmd::float64 factorize_hh(glmd::float64 *ptrs[], glmd::int32 n)
{
	glmd::float64 tau = 0.0;

	if (n > 1) {
		glmd::float64 xnorm;
		if (n == 2)
			xnorm = fabs(*ptrs[1]);
		else {
			glmd::float64 scl = 0.0;
			glmd::float64 ssq = 1.0;
			for (glmd::int32 i = 1; i < n; ++i) {
				glmd::float64 x = fabs(*ptrs[i]);
				if (x != 0.0) {
					if (scl < x) {
						ssq = 1.0 + ssq * (scl / x) * (scl / x);
						scl = x;
					} else
						ssq += (x / scl) * (x / scl);
				}
			}
			xnorm = scl * sqrt(ssq);
		}

		if (xnorm != 0.0) {
			glmd::float64 alpha = *ptrs[0];
			glmd::float64 beta = sqrt(alpha * alpha + xnorm * xnorm);
			if (alpha >= 0.0)
				beta = -beta;
			tau = (beta - alpha) / beta;

			glmd::float64 scl = 1.0 / (alpha - beta);
			*ptrs[0] = beta;
			for (glmd::int32 i = 1; i < n; ++i)
				*ptrs[i] *= scl;
		}
	}

	return tau;
}

//----------------------------------------------------------------------------

void unpack(
	glmd::float64 u[][3],				  // matrix (rows x 3)
	glmd::float64 v[3][3],				 // matrix (3x3)
	glmd::float64 tau_u[3],				// vector, (1x3)
	glmd::float64 tau_v[2],				// vector, (1x2)
	glmd::int32 rows)
{
	glmd::int32 i, y;

	// reset v to the identity matrix
	v[0][0] = v[1][1] = v[2][2] = 1.0;
	v[0][1] = v[0][2] = v[1][0] = v[1][2] = v[2][0] = v[2][1] = 0.0;

	for (i = 1; i >= 0; --i) {
		glmd::float64 tau = tau_v[i];

		// perform householder transformation on the sub-matrix
		// v(i+1,i+1) to v(m,n), that is, on the sub-matrix of v
		// that begins in the (i+1)'th column of the (i+1)'th row
		// and extends to the end of the matrix at (m,n).  the
		// householder vector used to perform this is the vector
		// from u(i,i+1) to u(i,n)
		if (tau != 0.0) {
			for (glmd::int32 x = i + 1; x < 3; ++x) {
				glmd::float64 wx = v[i + 1][x];
				for (y = i + 1 + 1; y < 3; ++y)
					wx += v[y][x] * u[i][y];
				glmd::float64 tau_wx = tau * wx;
				v[i + 1][x] -= tau_wx;
				for (y = i + 1 + 1; y < 3; ++y)
					v[y][x] -= tau_wx * u[i][y];
			}
		}
	}

	// copy superdiagonal of u into tau_v
	for (i = 0; i < 2; ++i)
		tau_v[i] = u[i][i + 1];

	// below, same idea for u:  householder transformations
	// and the superdiagonal copy

	for (i = 2; i >= 0; --i) {
		// copy superdiagonal of u into tau_u
		glmd::float64 tau = tau_u[i];
		tau_u[i] = u[i][i];

		// perform householder transformation on the sub-matrix
		// u(i,i) to u(m,n), that is, on the sub-matrix of u that
		// begins in the i'th column of the i'th row and extends
		// to the end of the matrix at (m,n).  the householder
		// vector used to perform this is the i'th column of u,
		// that is, u(0,i) to u(m,i)
		if (tau == 0.0) {
			u[i][i] = 1.0;
			if (i < 2) {
				u[i][2] = 0.0;
				if (i < 1)
					u[i][1] = 0.0;
			}
			for (y = i + 1; y < rows; ++y)
				u[y][i] = 0.0;
		} else {
			for (glmd::int32 x = i + 1; x < 3; ++x) {
				glmd::float64 wx = 0.0;
				for (y = i + 1; y < rows; ++y)
					wx += u[y][x] * u[y][i];
				glmd::float64 tau_wx = tau * wx;
				u[i][x] = -tau_wx;
				for (y = i + 1; y < rows; ++y)
					u[y][x] -= tau_wx * u[y][i];
			}
			for (y = i + 1; y < rows; ++y)
				u[y][i] = u[y][i] * -tau;
			u[i][i] = 1.0 - tau;
		}
	}
}

//----------------------------------------------------------------------------

void diagonalize(
	glmd::float64 u[][3],				  // matrix (rows x 3)
	glmd::float64 v[3][3],				 // matrix (3x3)
	glmd::float64 tau_u[3],				// vector, (1x3)
	glmd::float64 tau_v[2],				// vector, (1x2)
	glmd::int32 rows)
{
	glmd::int32 i, j;

	chop(tau_u, tau_v, 3);

	// progressively reduce the matrices into diagonal form

	glmd::int32 b = 3 - 1;
	while (b > 0) {
		if (tau_v[b - 1] == 0.0)
			--b;
		else {
			glmd::int32 a = b - 1;
			while (a > 0 && tau_v[a - 1] != 0.0)
				--a;
			glmd::int32 n = b - a + 1;

			glmd::float64 u1[MAXROWS][3];
			glmd::float64 v1[3][3];
			for (j = a; j <= b; ++j) {
				for (i = 0; i < rows; ++i)
					u1[i][j - a] = u[i][j];
				for (i = 0; i < 3; ++i)
					v1[i][j - a] = v[i][j];
			}

			qrstep(u1, v1, &tau_u[a], &tau_v[a], rows, n);

			for (j = a; j <= b; ++j) {
				for (i = 0; i < rows; ++i)
					u[i][j] = u1[i][j - a];
				for (i = 0; i < 3; ++i)
					v[i][j] = v1[i][j - a];
			}

			chop(&tau_u[a], &tau_v[a], n);
		}
	}
}

//----------------------------------------------------------------------------

void chop(glmd::float64 *a, glmd::float64 *b, glmd::int32 n)
{
	glmd::float64 ai = a[0];
	for (glmd::int32 i = 0; i < n - 1; ++i) {
		glmd::float64 bi = b[i];
		glmd::float64 ai1 = a[i + 1];
		if (fabs(bi) < EPSILON * (fabs(ai) + fabs(ai1)))
			b[i] = 0.0;
		ai = ai1;
	}
}

//----------------------------------------------------------------------------

void qrstep(
	glmd::float64 u[][3],				  // matrix (rows x cols)
	glmd::float64 v[][3],				  // matrix (3 x cols)
	glmd::float64 tau_u[],				 // vector (1 x cols)
	glmd::float64 tau_v[],				 // vector (1 x cols - 1)
	glmd::int32 rows, glmd::int32 cols)
{
	glmd::int32 i;

	if (cols == 2) {
		qrstep_cols2(u, v, tau_u, tau_v, rows);
		return;
	}

	if (cols == 1) {
	char *bomb = 0;
	*bomb = 0;
	}

	// handle zeros on the diagonal or at its end
	for (i = 0; i < cols - 1; ++i)
		if (tau_u[i] == 0.0) {
			qrstep_middle(u, tau_u, tau_v, rows, cols, i);
			return;
		}
	if (tau_u[cols - 1] == 0.0) {
		qrstep_end(v, tau_u, tau_v, cols);
		return;
	}

	// perform qr reduction on the diagonal and off-diagonal

	glmd::float64 mu = qrstep_eigenvalue(tau_u, tau_v, cols);
	glmd::float64 y = tau_u[0] * tau_u[0] - mu;
	glmd::float64 z = tau_u[0] * tau_v[0];

	glmd::float64 ak = 0.0;
	glmd::float64 bk = 0.0;
	glmd::float64 zk;
	glmd::float64 ap = tau_u[0];
	glmd::float64 bp = tau_v[0];
	glmd::float64 aq = tau_u[1];
	// TODO: Maybe understand this better?  Figure out why tau_v[1] isn't used???  Not sure...
	//glmd::float64 bq = tau_v[1];

	for (glmd::int32 k = 0; k < cols - 1; ++k) {
		glmd::float64 c, s;

		// perform Givens rotation on V

		computeGivens(y, z, &c, &s);

		for (i = 0; i < 3; ++i) {
			glmd::float64 vip = v[i][k];
			glmd::float64 viq = v[i][k + 1];
			v[i][k] = vip * c - viq * s;
			v[i][k + 1] = vip * s + viq * c;
		}

		// perform Givens rotation on B

		glmd::float64 bk1 = bk * c - z * s;
		glmd::float64 ap1 = ap * c - bp * s;
		glmd::float64 bp1 = ap * s + bp * c;
		glmd::float64 zp1 = aq * -s;
		glmd::float64 aq1 = aq * c;

		if (k > 0)
			tau_v[k - 1] = bk1;

		ak = ap1;
		bk = bp1;
		zk = zp1;
		ap = aq1;

		if (k < cols - 2)
			bp = tau_v[k + 1];
		else
			bp = 0.0;

		y = ak;
		z = zk;

		// perform Givens rotation on U

		computeGivens(y, z, &c, &s);

		for (i = 0; i < rows; ++i) {
			glmd::float64 uip = u[i][k];
			glmd::float64 uiq = u[i][k + 1];
			u[i][k] = uip * c - uiq * s;
			u[i][k + 1] = uip * s + uiq * c;
		}

		// perform Givens rotation on B

		glmd::float64 ak1 = ak * c - zk * s;
		bk1 = bk * c - ap * s;
		glmd::float64 zk1 = bp * -s;

		ap1 = bk * s + ap * c;
		bp1 = bp * c;

		tau_u[k] = ak1;

		ak = ak1;
		bk = bk1;
		zk = zk1;
		ap = ap1;
		bp = bp1;

		if (k < cols - 2)
			aq = tau_u[k + 2];
		else
			aq = 0.0;

		y = bk;
		z = zk;
	}

	tau_v[cols - 2] = bk;
	tau_u[cols - 1] = ap;
}

//----------------------------------------------------------------------------

void qrstep_middle(
	glmd::float64 u[][3],				 // matrix (rows x cols)
	glmd::float64 tau_u[],				 // vector (1 x cols)
	glmd::float64 tau_v[],				 // vector (1 x cols - 1)
	glmd::int32 rows, glmd::int32 cols, glmd::int32 col)
{
	glmd::float64 x = tau_v[col];
	glmd::float64 y = tau_u[col + 1];
	for (glmd::int32 j = col; j < cols - 1; ++j) {
		glmd::float64 c, s;

		// perform Givens rotation on U

		computeGivens(y, -x, &c, &s);
		for (glmd::int32 i = 0; i < rows; ++i) {
			glmd::float64 uip = u[i][col];
			glmd::float64 uiq = u[i][j + 1];
			u[i][col] = uip * c - uiq * s;
			u[i][j + 1] = uip * s + uiq * c;
		}

		// perform transposed Givens rotation on B

		tau_u[j + 1] = x * s + y * c;
		if (j == col)
			tau_v[j] = x * c - y * s;

		if (j < cols - 2) {
			glmd::float64 z = tau_v[j + 1];
			tau_v[j + 1] *= c;
			x = z * -s;
			y = tau_u[j + 2];
		}
	}
}

//----------------------------------------------------------------------------

void qrstep_end(
	glmd::float64 v[][3],				 // matrix (3 x 3)
	glmd::float64 tau_u[],				 // vector (1 x 3)
	glmd::float64 tau_v[],				 // vector (1 x 2)
	glmd::int32 cols)
{
	glmd::float64 x = tau_u[1];
	glmd::float64 y = tau_v[1];

	for (glmd::int32 k = 1; k >= 0; --k) {
		glmd::float64 c, s;

		// perform Givens rotation on V

		computeGivens(x, y, &c, &s);

		for (glmd::int32 i = 0; i < 3; ++i) {
			glmd::float64 vip = v[i][k];
			glmd::float64 viq = v[i][2];
			v[i][k] = vip * c - viq * s;
			v[i][2] = vip * s + viq * c;
		}

		// perform Givens rotation on B

		tau_u[k] = x * c - y * s;
		if (k == 1)
			tau_v[k] = x * s + y * c;
		if (k > 0) {
			glmd::float64 z = tau_v[k - 1];
			tau_v[k - 1] *= c;

			x = tau_u[k - 1];
			y = z * s;
		}
	}
}

//----------------------------------------------------------------------------

glmd::float64 qrstep_eigenvalue(
	glmd::float64 tau_u[],				 // vector (1 x 3)
	glmd::float64 tau_v[],				 // vector (1 x 2)
	glmd::int32 cols)
{
	glmd::float64 ta = tau_u[1] * tau_u[1] + tau_v[0] * tau_v[0];
	glmd::float64 tb = tau_u[2] * tau_u[2] + tau_v[1] * tau_v[1];
	glmd::float64 tab = tau_u[1] * tau_v[1];
	glmd::float64 dt = (ta - tb) / 2.0;
	glmd::float64 mu;
	if (dt >= 0.0)
		mu = tb - (tab * tab) / (dt + sqrt(dt * dt + tab * tab));
	else
		mu = tb + (tab * tab) / (sqrt(dt * dt + tab * tab) - dt);
	return mu;
}

//----------------------------------------------------------------------------

void qrstep_cols2(
	glmd::float64 u[][3],				  // matrix (rows x 2)
	glmd::float64 v[][3],				  // matrix (3 x 2)
	glmd::float64 tau_u[],				 // vector (1 x 2)
	glmd::float64 tau_v[],				 // vector (1 x 1)
	glmd::int32 rows)
{
	glmd::int32 i;
	glmd::float64 tmp;

	// eliminate off-diagonal element in [ 0  tau_v0 ]
	//								   [ 0  tau_u1 ]
	// to make [ tau_u[0]  0 ]
	//		 [ 0		 0 ]

	if (tau_u[0] == 0.0) {
		glmd::float64 c, s;

		// perform transposed Givens rotation on B
		// multiplied by X = [ 0 1 ]
		//				   [ 1 0 ]

		computeGivens(tau_v[0], tau_u[1], &c, &s);

		tau_u[0] = tau_v[0] * c - tau_u[1] * s;
		tau_v[0] = tau_v[0] * s + tau_u[1] * c;
		tau_u[1] = 0.0;

		// perform Givens rotation on U

		for (i = 0; i < rows; ++i) {
			glmd::float64 uip = u[i][0];
			glmd::float64 uiq = u[i][1];
			u[i][0] = uip * c - uiq * s;
			u[i][1] = uip * s + uiq * c;
		}

		// multiply V by X, effectively swapping first two columns

		for (i = 0; i < 3; ++i) {
			tmp = v[i][0];
			v[i][0] = v[i][1];
			v[i][1] = tmp;
		}
	}

	// eliminate off-diagonal element in [ tau_u0  tau_v0 ]
	//								   [ 0	   0	  ]

	else if (tau_u[1] == 0.0) {
		glmd::float64 c, s;

		// perform Givens rotation on B

		computeGivens(tau_u[0], tau_v[0], &c, &s);

		tau_u[0] = tau_u[0] * c - tau_v[0] * s;
		tau_v[0] = 0.0;

		// perform Givens rotation on V

		for (i = 0; i < 3; ++i) {
			glmd::float64 vip = v[i][0];
			glmd::float64 viq = v[i][1];
			v[i][0] = vip * c - viq * s;
			v[i][1] = vip * s + viq * c;
		}
	}

	// make colums orthogonal,

	else {
		glmd::float64 c, s;

		// perform Schur rotation on B

		computeSchur(tau_u[0], tau_v[0], tau_u[1], &c, &s);

		glmd::float64 a11 = tau_u[0] * c - tau_v[0] * s;
		glmd::float64 a21 = - tau_u[1] * s;
		glmd::float64 a12 = tau_u[0] * s + tau_v[0] * c;
		glmd::float64 a22 = tau_u[1] * c;

		// perform Schur rotation on V

		for (i = 0; i < 3; ++i) {
			glmd::float64 vip = v[i][0];
			glmd::float64 viq = v[i][1];
			v[i][0] = vip * c - viq * s;
			v[i][1] = vip * s + viq * c;
		}

		// eliminate off diagonal elements

		if ((a11 * a11 + a21 * a21) < (a12 * a12 + a22 * a22)) {

			// multiply B by X

			tmp = a11;
			a11 = a12;
			a12 = tmp;
			tmp = a21;
			a21 = a22;
			a22 = tmp;

			// multiply V by X, effectively swapping first
			// two columns

			for (i = 0; i < 3; ++i) {
				tmp = v[i][0];
				v[i][0] = v[i][1];
				v[i][1] = tmp;
			}
		}

		// perform transposed Givens rotation on B

		computeGivens(a11, a21, &c, &s);

		tau_u[0] = a11 * c - a21 * s;
		tau_v[0] = a12 * c - a22 * s;
		tau_u[1] = a12 * s + a22 * c;

		// perform Givens rotation on U

		for (i = 0; i < rows; ++i) {
			glmd::float64 uip = u[i][0];
			glmd::float64 uiq = u[i][1];
			u[i][0] = uip * c - uiq * s;
			u[i][1] = uip * s + uiq * c;
		}
	}
}

//----------------------------------------------------------------------------

void computeGivens(
	glmd::float64 a, glmd::float64 b, glmd::float64 *c, glmd::float64 *s)
{
	if (b == 0.0) {
		*c = 1.0;
		*s = 0.0;
	} else if (fabs(b) > fabs(a)) {
		glmd::float64 t = -a / b;
		glmd::float64 s1 = 1.0 / sqrt(1 + t * t);
		*s = s1;
		*c = s1 * t;
	} else {
		glmd::float64 t = -b / a;
		glmd::float64 c1 = 1.0 / sqrt(1 + t * t);
		*c = c1;
		*s = c1 * t;
	}
}

//----------------------------------------------------------------------------

void computeSchur(
	glmd::float64 a1, glmd::float64 a2, glmd::float64 a3,
	glmd::float64 *c, glmd::float64 *s)
{
	glmd::float64 apq = a1 * a2 * 2.0;

	if (apq == 0.0) {
		*c = 1.0;
		*s = 0.0;
	} else {
		glmd::float64 t;
		glmd::float64 tau = (a2 * a2 + (a3 + a1) * (a3 - a1)) / apq;
		if (tau >= 0.0)
			t = 1.0 / (tau + sqrt(1.0 + tau * tau));
		else
			t = - 1.0 / (sqrt(1.0 + tau * tau) - tau);
		*c = 1.0 / sqrt(1.0 + t * t);
		*s = t * (*c);
	}
}

//----------------------------------------------------------------------------

void singularize(
	glmd::float64 u[][3],				  // matrix (rows x 3)
	glmd::float64 v[3][3],				 // matrix (3x3)
	glmd::float64 d[3],					// vector, (1x3)
	glmd::int32 rows)
{
	glmd::int32 i, j, y;

	// make singularize values positive

	for (j = 0; j < 3; ++j)
		if (d[j] < 0.0) {
			for (i = 0; i < 3; ++i)
				v[i][j] = -v[i][j];
			d[j] = -d[j];
		}

	// sort singular values in decreasing order

	for (i = 0; i < 3; ++i) {
		glmd::float64 d_max = d[i];
		glmd::int32 i_max = i;
		for (j = i + 1; j < 3; ++j)
			if (d[j] > d_max) {
				d_max = d[j];
				i_max = j;
			}

		if (i_max != i) {
			// swap eigenvalues
			glmd::float64 tmp = d[i];
			d[i] = d[i_max];
			d[i_max] = tmp;

			// swap eigenvectors
			for (y = 0; y < rows; ++y) {
				tmp = u[y][i];
				u[y][i] = u[y][i_max];
				u[y][i_max] = tmp;
			}
			for (y = 0; y < 3; ++y) {
				tmp = v[y][i];
				v[y][i] = v[y][i_max];
				v[y][i_max] = tmp;
			}
		}
	}
}

//----------------------------------------------------------------------------

void solveSVD(
	glmd::float64 u[][3],				  // matrix (rows x 3)
	glmd::float64 v[3][3],				 // matrix (3x3)
	glmd::float64 d[3],					// vector (1x3)
	glmd::float64 b[],					 // vector (1 x rows)
	glmd::float64 x[3],					// vector (1x3)
	glmd::int32 rows)
{
	glmd::int32 i, j;

	// compute vector w = U^T * b

	glmd::float64 w[3] = { 0.0, 0.0, 0.0 };
	for (i = 0; i < rows; ++i) {
		if (b[i] != 0.0)
			for (j = 0; j < 3; ++j)
				w[j] += b[i] * u[i][j];
	}

	// introduce non-zero singular values in d into w

	for (i = 0; i < 3; ++i) {
		if (d[i] != 0.0)
			w[i] /= d[i];
	}

	// compute result vector x = V * w

	for (i = 0; i < 3; ++i) {
		glmd::float64 tmp = 0.0;
		for (j = 0; j < 3; ++j)
			tmp += w[j] * v[i][j];
		x[i] = tmp;
	}
}

}
}
