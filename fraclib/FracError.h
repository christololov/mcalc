// The MIT License (MIT)
// 
// Copyright (c) 2014 Rafat Rashid
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/** @file FracError.h
	@brief This file defines error conditions that can result when using %Fraction and %MFraction classes.
	
	For instance, when the user tries to initilize a fraction object with a denominator of 0, this is not valid
	and an exception will be thrown of type FR_ERROR with value of FR_DENOM_ZERO.
	See details below.

	@warning Error handling for the fraction library is not complete. Particularly, exceptions might be thrown
	         by other operations (implemented in the standard c++ libraries) which are used to implement the
			 fraction operations. These are not caught and taken care of within the fraction class implementation
			 and instead left to the user of the %Fraction library. (Use a try-catch block. In most situations,
			 you do not need to worry about errors of this kind.)

	@warning Additionally, the %Fraction library uses unsigned integers to represent the whole, numerator and
			 denominator parts of a fraction and similar to how unsigned integers have a limitation in their
			 numerical representation, so does objects of type %Fraction/%MFraction.

	@warning I will attempt to tackle and resolve these issues in the later releases of this library. For now, all
	         exceptions which incur are thrown back to the caller of the fraction operations to be caught. (If they
			 are not caught at some point, the program will exit with an error code.) Also, at the moment, the
			 numerical representation limitation issue is ignored. The standard libraries do not pay much attention
			 to this either. (see code block below)

	@include test2.cpp	
 */

#ifndef FRAC_ERROR_H
	#define FRAC_ERROR_H

	/*! @brief %Fraction error code variable: stores the type of error incurred. */
	typedef unsigned short FR_ERROR;

	/*! @brief A value FR_ERROR takes when a fraction's denominator is equal to 0. */
	#define FR_DENOM_ZERO ((FR_ERROR)1)

	/*! @brief A value FR_ERROR takes when a string value can't be converted into a fraction. */
	#define FR_STR_INVALID ((FR_ERROR)2)

	/*! @brief A value FR_ERROR takes when index is out of bounds when using the [] operator to access the private
	    member variables.
	    @sa Fraction::operator [] and MFraction::operator [] */
	#define FR_INDEX_OUT_BOUNDS ((FR_ERROR)3)

	/*! @brief A value FR_ERROR takes when a positive integer is expected but a negative value is passed in. */		
	#define FR_NEG_PARAM ((FR_ERROR)4)
#endif