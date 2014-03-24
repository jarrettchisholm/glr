  /*
Written by A. Odrzywolek
*/
/*
TODO: trilinear interpolation
TODO: remove "wrong grid select" bug from bilinear_interp
TODO: better 2D and 3D interpolation; FFN-specific accurate interpolation
*/
//#define EXTRAPOLATE 0
//#define ZERO_OUT_OF_DOMAIN 1

#include <math.h>
#include "terrain/Interpolation.hpp"
//#include <stdio.h>

/*
Bilinear interpolation of the FFN-like table
WARNING: this is not enough to get accurate results,
table is to sparse and variation of the values too big!
*/
static const double T9_tbl[14] = {0.0,0.01,0.1,0.2,0.4,0.7,1.0,1.5,2.0,3.0,5.0,10.0,30.0,100.0};
static const double lg_rhoYe_tbl[12] = {0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0};

double 
ffn_bilinear (double T9, double lg_rhoYe, const double *rate_tbl)
{

  double f11,f12,f21,f22, f_interp;
  double T9_1,T9_2,lg_rhoYe_1,lg_rhoYe_2,dT9,dlg_rhoYe;
  int i,j;

/*
  double rate[14][12];

  for(i=0;i<14;i++)
    for(j=0;j<12;j++)
      rate[i][j]=*(rate_tbl+12*i+j);
*/


  if(T9<0.01) return 0.0;  

  if( T9<100.0 )
   { 
    for(i=1; T9_tbl[i]<T9 ;i++) ;
   }
  else
   {
    i=13;
    T9 = 100.0;
   }

  if(lg_rhoYe<1.0) return 0.0;

  if( lg_rhoYe<11.0)
   {  
    for(j=1; lg_rhoYe_tbl[j]<lg_rhoYe ;j++) ;
   }
  else
   {
    j=11;
    lg_rhoYe=11.0; 
   }

//  j=(int) floor(lg_rhoYe);     SLOWER !

 
  f11 = *(rate_tbl+12*i+j-12-1);  //rate[i-1][j-1];
  f12 = *(rate_tbl+12*i+j-12);      // rate[i-1][j];
  f21 = *(rate_tbl+12*i+j-1);      //rate[i][j-1];
  f22 = *(rate_tbl+12*i+j);         //rate[i][j];  

  T9_1    = T9_tbl[i-1];
  T9_2    = T9_tbl[i];

  lg_rhoYe_1 = lg_rhoYe_tbl[j-1];
  lg_rhoYe_2 = lg_rhoYe_tbl[j];

  dT9 = T9_2-T9_1;
  dlg_rhoYe = lg_rhoYe_2 - lg_rhoYe_1;

  f_interp =  f11*(T9_2-T9)*(lg_rhoYe_2-lg_rhoYe)
            - f21*(T9_1-T9)*(lg_rhoYe_2-lg_rhoYe)
            - f12*(T9_2-T9)*(lg_rhoYe_1-lg_rhoYe)
            + f22*(T9_1-T9)*(lg_rhoYe_1-lg_rhoYe);



  return f_interp/dT9/dlg_rhoYe;

}

inline double 
bilinear_interp_unit_square(double X, double Y, double A00, double A10, double A11, double A01)
{

  return A00*(1.0-X)*(1.0-Y) + A10*X*(1.0-Y) + A01*(1.0-X)*Y + A11*X*Y;
}

inline double
trilinear_interp_unit_cube
(double X, double Y, double Z, double A000, double A100, double A101, double A001, double A010, double A110, double A111, double A011)
{

  return A000*(1.0-X)*(1.0-Y)*(1.0-Z) + A100*X*(1.0-Y)*(1.0-Z) + A010*(1.0-X)*Y*(1.0-Z) + A001*(1.0-X)*(1.0-Y)*Z 
       + A101* X*(1.0 - Y)*Z + A011* (1.0 - X)*Y*Z + A110* X* Y* (1.0 - Z) + A111* X*Y*Z;
}


/*
General puprose bilinear interpolation of the emissivities etc
X,Y - we want function value at this point
N_X, N_Y - grid size 
NOTE: N_? usually is included from thermal_tbl.h, but actual table size is N_?+1 !!! 
TODO: FIX this nonsense!?
(X_MIN,X_MAX)x(Y_MIN,Y_MAX) - range of tabulated data

OY (0,1) = f12
^
|
|
|
|
|
 -----------> OX (1,0) = f21

OUT_OF_DOMAIN behavior:
  0 - return OUT_OF_DOMAIN_VALUE;
  1 - return edge values (try to extrapolate)

*/
double 
bilinear_interp(double X, double Y, const double *tbl, 
              const int N_X, const int N_Y, 
        const double X_MIN, const double X_MAX, 
        const double Y_MIN, const double Y_MAX, 
	const int OUT_OF_DOMAIN, const double OUT_OF_DOMAIN_VALUE)
{

  double f11, f12, f21, f22;
  //double f_interp;
  double X_A, X_B, Y_A, Y_B;

/* grid spacing */
  const double delta_X = (X_MAX-X_MIN)/(N_X-1),
               delta_Y = (Y_MAX-Y_MIN)/(N_Y-1);
  const double delta_X_inv = (N_X-1)/(X_MAX-X_MIN),
               delta_Y_inv = (N_Y-1)/(Y_MAX-Y_MIN);
  int i,j;

  double X_unit, Y_unit;

/* select upper right grid point BUG!!!!!! randomly (but very rarely) 
 sometimes select WRONG point!!!*/
     i = (int) ceil( (X-X_MIN)*delta_X_inv);
     j = (int) ceil( (Y-Y_MIN)*delta_Y_inv);
/* ale inaczej jest jeszcze jeszcze gorzej .....            
Moze sprobowac tak jak w ffn_bilinear, jest szybciej niz przez ceil/floor.....
ale to troche inna sprawa; nie ma pre-definowanej siatki X-Y

WARNINIG: numerical round-off causes this! E.g. 0.1+0.1+0.1+0.1 = 0.4 <- not true!
 */




/* Return egde values if X or Y are under/overflown */
/* edge of the domain and out-of-range values */


  if( OUT_OF_DOMAIN==0 ) 
   {
      if( (X<X_MIN) || (Y<Y_MIN) || (X>X_MAX) || (Y>Y_MAX) ) return OUT_OF_DOMAIN_VALUE;
   };


/*
VERY primitive extrapolation. 
*/
  if( OUT_OF_DOMAIN==1 ) 
   {
    if((X<=X_MIN) && (Y<=Y_MIN)) return *tbl;             //1
    if((X<=X_MIN) && (Y<Y_MAX) ) return *(tbl+j);            //2
    if((X<=X_MIN) && (Y>=Y_MAX)) return *(tbl+N_Y-1);      //3
    if((X<=X_MAX) && (Y<=Y_MIN)) return *(tbl+(N_Y)*i); //4
    if((X<=X_MAX) && (Y>=Y_MAX)) return *(tbl+(N_Y)*i+N_Y-1);  //5
    if((X>X_MAX)  && (Y<=Y_MIN)) return *(tbl+(N_Y)*(N_X-1));   //6
    if((X>X_MAX)  && (Y<=Y_MAX)) return *(tbl+(N_Y)*(N_X-1)+j); //7
    if((X>X_MAX)  && (Y>Y_MAX) ) return *(tbl+(N_Y)*(N_X-1)+N_Y-1); //8
   };


/* Return 0.0  if X or Y are under/overflown */
/* edge of the domain and out-of-range values */


/*   just to remember:   tbl[i][j]=*(tbl+(N_Y)*i+j);  */
  

/* cell corner values */ 
 
  f11 = *(tbl + (N_Y)*(i-1)+j-1);
  f12 = *(tbl + (N_Y)*(i-1)+j);
  f21 = *(tbl + (N_Y)*i+j-1);
  f22 = *(tbl + (N_Y)*i+j);
  
/* cell corner coordinates */
  X_A = X_MIN + (i-1)*delta_X;    
  X_B = X_MIN + i*delta_X;    
  
  Y_A = Y_MIN + (j-1)*delta_Y;
  Y_B = Y_MIN + j*delta_Y;


/* interpolated value returned (OLD version)

  f_interp = f11*(X_B-X)*(Y_B-Y)
            - f21*(X_A-X)*(Y_B-Y)
            - f12*(X_B-X)*(Y_A-Y)
            + f22*(X_A-X)*(Y_A-Y);

  return f_interp*delta_X_inv*delta_Y_inv;
*/
  
  
/* rescalling X,Y, to unit square */

   X_unit = (X-X_A)/(X_B-X_A);
   Y_unit = (Y-Y_A)/(Y_B-Y_A);

   return bilinear_interp_unit_square(X_unit,Y_unit, f11,f21,f22,f12);

}




double 
bilinear_interp_arbitrary_grid(double X, double Y, double *tbl, 
                const double *Xtbl, const int N_X,
                const double *Ytbl, const int N_Y,
                const int OUT_OF_DOMAIN, const double OUT_OF_DOMAIN_VALUE)
{
  
  double f11,f21,f22,f12;
  double X_A,X_B,Y_A,Y_B;
  
  int i=0,j=0;
  double X_unit, Y_unit;
  
  /* Return 0.0 if X or Y or Z are under/overflown */
  /* edge of the domain and out-of-range values */

  if( OUT_OF_DOMAIN==0)
  {
    if( (X<Xtbl[0]) || (Y<Ytbl[0])  || (X>Xtbl[N_X-1]) || (Y>Ytbl[N_Y-1]) ) 
    return OUT_OF_DOMAIN_VALUE;
  }

  while( (Xtbl[i]<X) && (i<N_X) ) i++;
  while( (Ytbl[j]<Y) && (j<N_Y) ) j++;



/* cell corner values */ 
  f11 = *(tbl + (N_Y)*(i-1)+j-1);
  f12 = *(tbl + (N_Y)*(i-1)+j);
  f21 = *(tbl + (N_Y)*i+j-1);
  f22 = *(tbl + (N_Y)*i+j);
  
  X_A = Xtbl[i-1];
  X_B = Xtbl[i];
  
  Y_A = Ytbl[j-1];
  Y_B = Ytbl[j];
  

  /* Rescalling to unit square {0,1}^2 */
  X_unit = (X-X_A)/(X_B-X_A);
  Y_unit = (Y-Y_A)/(Y_B-Y_A);
  
  
  return bilinear_interp_unit_square(X_unit,Y_unit, f11,f21,f22,f12);
  
}



/*
General puprose trilinear interpolation of the emissivities etc
X,Y,Z - we want function value at this point
N_X, N_Y, N_Z - grid size  (see NOTE for bilinear_interp() )
(X_MIN,X_MAX)x(Y_MIN,Y_MAX)x(Z_MIN,Z_MAX) - range of tabulated data

(X,Y,Z) coords:

Z(1,1,2)=f112
^
|    
|
|    Y(1,2,1)=f121
|   /
|  /
| /
|/
 ------------------->X (2,1,1)=f211
O (1,1,1)=f111
*/

double 
trilinear_interp(double X, double Y, double Z, const double *tbl, 
      const int N_X, const int N_Y, const int N_Z,
      const double X_MIN, const double X_MAX, 
      const double Y_MIN, const double Y_MAX,
      const double Z_MIN, const double Z_MAX,
      const int OUT_OF_DOMAIN, const double OUT_OF_DOMAIN_VALUE)
{

  double f111,f211,f212,f112, f121,f221,f222,f122;
  double X_A,X_B,Y_A,Y_B,Z_A,Z_B;

/* grid spacing */
  const double delta_X = (X_MAX-X_MIN)/(N_X-1);
  const double delta_Y = (Y_MAX-Y_MIN)/(N_Y-1);
  const double delta_Z = (Z_MAX-Z_MIN)/(N_Z-1);
 
  const double delta_X_inv = (N_X-1)/(X_MAX-X_MIN);
  const double delta_Y_inv = (N_Y-1)/(Y_MAX-Y_MIN);
  const double delta_Z_inv = (N_Z-1)/(Z_MAX-Z_MIN);
  int i,j,k;
  double X_unit, Y_unit, Z_unit;


  /* select upper right grid point BUG!!!!!! randomly (but very rarely) 
 sometimes select WRONG point!!!*/
  i = (int) ceil( (X-X_MIN)*delta_X_inv);
  j = (int) ceil( (Y-Y_MIN)*delta_Y_inv);
  k = (int) ceil( (Z-Z_MIN)*delta_Z_inv);

/* Return OUT_OF_DOMAIN_VALUE if X or Y or Z are under/overflown */
/* edge of the domain and out-of-range values */
   if( OUT_OF_DOMAIN==0 ) 
   {
     if( (X<X_MIN) || (Y<Y_MIN) || (Z<Z_MIN) || (X>X_MAX) || (Y>Y_MAX) || (Z>Z_MAX) ) return OUT_OF_DOMAIN_VALUE;
   };
   
/* cell corner values */ 
// f222=TBL[i][j][k] declared as double TBL[N_X+1][N_Y+1][N_Z+1]; tbl=&TBL[0][0][0]

  f111 = *(tbl+(N_Z)*(N_Y)*(i-1)+(N_Z)*(j-1)+k-1)   ; //TBL[i-1][j-1][k-1];
  f211 = *(tbl+(N_Z)*(N_Y)*i+(N_Z)*(j-1)+k-1)   ;// TBL[i][j-1][k-1];
  f212 = *(tbl+(N_Z)*(N_Y)*i+(N_Z)*(j-1)+k)   ;//TBL[i][j-1][k];
  f112 = *(tbl+(N_Z)*(N_Y)*(i-1)+(N_Z)*(j-1)+k)   ;//TBL[i-1][j-1][k];
  f121 = *(tbl+(N_Z)*(N_Y)*(i-1)+(N_Z)*j+k-1);    //TBL[i-1][j][k-1];
  f221 = *(tbl+(N_Z)*(N_Y)*i+(N_Z)*j+k-1);  //TBL[i][j][k-1];
  f222 = *(tbl+(N_Z)*(N_Y)*i+(N_Z)*j+k)   ; //TBL[i][j][k];
  f122 = *(tbl+(N_Z)*(N_Y)*(i-1)+(N_Z)*j+k); //TBL[i-1][j][k];

  X_A = X_MIN + (i-1)*delta_X;    
  X_B = X_MIN + i*delta_X;    
  
  Y_A = Y_MIN + (j-1)*delta_Y;
  Y_B = Y_MIN + j*delta_Y;

  Z_A = Z_MIN + (k-1)*delta_Z;
  Z_B = Z_MIN + k*delta_Z;


/* Rescalling to unit cube {0,1}^3 */
  X_unit = (X-X_A)/(X_B-X_A);
  Y_unit = (Y-Y_A)/(Y_B-Y_A);
  Z_unit = (Z-Z_A)/(Z_B-Z_A);

  return trilinear_interp_unit_cube(X_unit,Y_unit,Z_unit, f111,f211,f212,f112,f121,f221,f222,f122);

}




double 
trilinear_interp_arbitrary_grid(double X, double Y, double Z, double *tbl, 
                const double *Xtbl, const int N_X,
                const double *Ytbl, const int N_Y,
                const double *Ztbl, const int N_Z,
		const int OUT_OF_DOMAIN, const double OUT_OF_DOMAIN_VALUE)
{
  
  double f111,f211,f212,f112, f121,f221,f222,f122;
  double X_A,X_B,Y_A,Y_B,Z_A,Z_B;
  
  int i=0,j=0,k=0;
  double X_unit, Y_unit, Z_unit;
  
  /* Return 0.0 if X or Y or Z are under/overflown */
  /* edge of the domain and out-of-range values */

  if( OUT_OF_DOMAIN==0)
  {
    if( (X<Xtbl[0]) || (Y<Ytbl[0]) || (Z<Ztbl[0]) || (X>Xtbl[N_X-1]) || (Y>Ytbl[N_Y-1]) || (Z>Ztbl[N_Z-1]) ) 
    return OUT_OF_DOMAIN_VALUE;
  }

  while( (Xtbl[i]<X) && (i<N_X) ) i++;
  while( (Ytbl[j]<Y) && (j<N_Y) ) j++;
  while( (Ztbl[k]<Z) && (k<N_Z) ) k++;


/* cell corner values */ 
  // f222=TBL[i][j][k] declared as double TBL[N_X+1][N_Y+1][N_Z+1]; tbl=&TBL[0][0][0]
  
  f111 = *(tbl+(N_Z)*(N_Y)*(i-1)+(N_Z)*(j-1)+k-1)   ; //TBL[i-1][j-1][k-1];
  f211 = *(tbl+(N_Z)*(N_Y)*i+(N_Z)*(j-1)+k-1)   ;// TBL[i][j-1][k-1];
  f212 = *(tbl+(N_Z)*(N_Y)*i+(N_Z)*(j-1)+k)   ;//TBL[i][j-1][k];
  f112 = *(tbl+(N_Z)*(N_Y)*(i-1)+(N_Z)*(j-1)+k)   ;//TBL[i-1][j-1][k];
  f121 = *(tbl+(N_Z)*(N_Y)*(i-1)+(N_Z)*j+k-1);    //TBL[i-1][j][k-1];
  f221 = *(tbl+(N_Z)*(N_Y)*i+(N_Z)*j+k-1);  //TBL[i][j][k-1];
  f222 = *(tbl+(N_Z)*(N_Y)*i+(N_Z)*j+k)   ; //TBL[i][j][k];
  f122 = *(tbl+(N_Z)*(N_Y)*(i-1)+(N_Z)*j+k); //TBL[i-1][j][k];
  
  X_A = Xtbl[i-1];
  X_B = Xtbl[i];
  
  Y_A = Ytbl[j-1];
  Y_B = Ytbl[j];
  
  Z_A = Ztbl[k-1];
  Z_B = Ztbl[k];
  

  /* Rescalling to unit cube {0,1}^3 */
  X_unit = (X-X_A)/(X_B-X_A);
  Y_unit = (Y-Y_A)/(Y_B-Y_A);
  Z_unit = (Z-Z_A)/(Z_B-Z_A);
  
  return trilinear_interp_unit_cube(X_unit,Y_unit,Z_unit, f111,f211,f212,f112,f121,f221,f222,f122);
  
}




