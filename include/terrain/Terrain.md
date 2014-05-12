Terrain
=======

Noise Generator
---------------
GLR uses density fields to generate terrain, and uses simplex noise to populate the density fields.

Smoothing the Density Field
---------------------------
Each density 'point' is a value between -1.0 and 1.0, where anything above 0 is considered 'air', and anything
below 0 is considered 'solid'.  There is an [Isosurface](http://en.wikipedia.org/wiki/Isosurface) that exists where neighbouring density values transition across
the 0 boundary.  This Isosurface will become our terrain.

Two algorithms are used for generating terrain: Dual Contouring and Marching Cubes.  At this point, Marching Cubes is preferred, as it
only uses the field function (i.e. noise generator) during the initial generation of density values, whereas Dual Contouring requires
the field function during smoothing.  Also, I think saving data and later editing it would be very difficult with Dual Contouring.

Density Field
-------------
Currently, the density field is a giant 3-dimensional array (implemented using std::vector).

You will notice that the density field is bigger than the actual 'area' that is being turned into an Isosurface.  This is because
the algorithms used for smoothing will often need to look outside the specified Isosurface boundaries in order to calculate normals (using, for
example, interpolation).

The variables that define how many 'extra' dimensions the 3d array has are glr::terrain::constants::POINT_FIELD_OFFSET and glr::terrain::constants::POINT_FIELD_OVERSET,
which are located in terrain/Constants.hpp.
