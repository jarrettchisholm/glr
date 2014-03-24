#include "Interp.hpp"

/* bilinear interpolation inside unit square */
inline double bilinear_interp_unit_square
(double, double, double, double,  double, double);

/* trilinear interpolation inside unit cube */
inline double trilinear_interp_unit_cube
(double , double , double , double , double , double , double , double , double , double , double );

double 
trilinear_interp(double X, double Y, double Z, const double *tbl, 
      const int N_X, const int N_Y, const int N_Z,
      const double X_MIN, const double X_MAX, 
      const double Y_MIN, const double Y_MAX,
      const double Z_MIN, const double Z_MAX,
      const int OUT_OF_DOMAIN, const double OUT_OF_DOMAIN_VALUE);
