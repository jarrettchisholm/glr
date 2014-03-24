double ffn_bilinear(double, double, const double*);  //Interpolation on FFN-like grid (hardcoded)




/* 2D interpolation */
double bilinear_interp (double, double, const double*, 	// X, Y, *TABLE
const int, const int, 					// NX, NY
const double, const double, 				// X_MIN, X_MAX
const double, const double, 				// Y_MIN, Y_MAX
const int, const double);				// OUT_OF_DOMAIN_FLAG, OUT_OF_DOMAIN_VALUE


/* 2D interpolation on non-equidistant (user-supplied) grid*/
double 
bilinear_interp_arbitrary_grid(double , double , double*,  	// X, Y, *TABLE
const double*, const int, 						// X_tbl, NX
const double*, const int,						// Y_tbl, NY
const int, const double);				// OUT_OF_DOMAIN_FLAG, OUT_OF_DOMAIN_VALUE);



/* 3D interpolation on equidistant grid*/
double trilinear_interp (double, double, double, const double*, // X, Y, Z, *TABLE
const int, const int, const int, 				// NX, NY, NZ
const double, const double, 					// X_MIN, Y_MAX
const double, const double,					// Y_MIN, Y_MAX
const double, const double,					// Z_MIN, Z_MAX
const int, const double);				// OUT_OF_DOMAIN_FLAG, OUT_OF_DOMAIN_VALUE);

/* 3D interpolation on non-equidistant (user-supplied) grid*/
double 
trilinear_interp_arbitrary_grid(double , double , double , double*,  	// X, Y, Z, *TABLE
const double*, const int, 						// X_tbl, NX
const double*, const int,						// Y_tbl, NY
const double*, const int,						// Z_tbl, NZ
const int, const double);				// OUT_OF_DOMAIN_FLAG, OUT_OF_DOMAIN_VALUE);


