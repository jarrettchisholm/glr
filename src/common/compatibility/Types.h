/*
 * Types.h
 *
 *  Created on: 2011-04-27
 *      Author: jarrett
 */

#ifndef TYPES_H_
#define TYPES_H_

namespace compatibility {

/* This is a typedef for an 8 bit unsigned char, which ensures portability of the code.
 */
#ifdef _MSC_VER
typedef unsigned __int8 uchar8;
#else
typedef unsigned char uchar8;
#endif

/* This is a typedef for an 8 bit signed char, which ensures portability of the code.
 */
#ifdef _MSC_VER
typedef __int8 schar8;
#else
typedef signed char schar8;
#endif

/* This is a typedef for an 8 bit char, which ensures portability of the code.
 */
typedef char char8;

/* This is a typedef for an 16 bit unsigned short, which ensures portability of the code.
 */
#ifdef _MSC_VER
typedef unsigned __int16 uint16;
#else
typedef unsigned short uint16;
#endif

/* This is a typedef for an 16 bit signed short, which ensures portability of the code.
 */
#ifdef _MSC_VER
typedef __int16 sint16;
#else
typedef signed short sint16;
#endif

/* This is a typedef for an 32 bit unsigned int, which ensures portability of the code.
 */
#ifdef _MSC_VER
typedef unsigned __int32 uint32;
#else
typedef unsigned int uint32;
#endif

/* This is a typedef for an 32 bit signed int, which ensures portability of the code.
 */
#ifdef _MSC_VER
typedef __int32 sint32;
#else
typedef signed int sint32;
#endif

/* This is a typedef for an 64 bit signed int, which ensures portability of the code.
 * However, it is only supported by Microsoft Compilers (at the moment), and so is commented out.
 */
//typedef __int64				sint64;

/* This is a typedef for an 32 bit float, which ensures portability of the code.
 */
typedef float float32;

/* This is a typedef for an 64 bit float, which ensures portability of the code.
 */
typedef double float64;

}

#endif /* TYPES_H_ */
