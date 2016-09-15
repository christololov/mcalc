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

/** @file PartialLib.h
	@brief This file allows you to compile a library which supports only %Fraction or %MFraction or both.

	Right now, it is set up to support both %Fraction and %MFraction classes and their associated operations.

	Uncomment the first block (the 3 lines) of code to only have %Fraction class functionality. That is, fractions
	that can be expressed as decimal or improper only. Do not use %MFraction specific operations in this case.

	If you want to express fractions as improper, mixed or decimal, uncomment the second block (the final 3 lines)
	of code to only have %MFraction class functionality. Do not use %Fraction specific operations in this case.

	Suffice to say, both blocks should not be commented out at the same time. If you do this, you will lose both
	%Fraction and %MFraction functionality.

	@note This is only meant for efficiency (as you can decide not to compile functionality you do not need). If you
	      are using this library for the first time and a little new at C/C++ programming, leave the code below alone.
		  With the current setup (both blocks commented out), everything in the library will be compiled.

   @par Steps to Take:
        - Uncomment the first 3 lines for %Fraction functionality only OR the last 3 lines for %MFraction
		  functionality only.
   @par
		- Build/compile the lib_create project using Microsoft Visual C++ by right-clicking on the project name
		  and selecting build. The .lib output file is located in /lib directory. If you are using the Debug
		  configuration, which is the default, then the output file will be in the /lib/Debug subdirectory.
   @par
	    - Link the library file into your external project. See the projects in the /tests directory for examples
		  as to how to do this.
 */

//#ifndef FRACTION_ONLY
//	#define FRACTION_ONLY
//#endif

//#ifndef MFRACTION_ONLY
//	#define MFRACTION_ONLY
//#endif