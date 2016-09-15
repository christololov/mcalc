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

/** @file Fraction.h
	@brief This file declares the interface of the %Fraction class.

		   Private and protected members of the class are also declared in this file.
 */

#ifndef FRACTION_H
	#define FRACTION_H

	#include "PartialLib.h"

	#if !defined(FRACTION_ONLY) || !defined(MFRACTION_ONLY)

		#include <iostream>
		#include "FracError.h"
		using namespace std;

		/**
			@brief This class allows you to handle improper fractions just like how basic types such as integers
				   and doubles are handled in c++.

			Improper fractions are of the form numerator/denominator. For instance, 5/6 or 10/3.
			These fractions will always be in their reduced form.

			The programmer has a choice of outputting the fraction as a decimal number or as a fraction.
			This applies to all fraction objects that are of type %Fraction.
			For instance, 3.5 or 7/2. See documentation of "Static Public Member Functions" for details.

			The programmer can initilize fraction objects by providing:
				- a single number (can be integer or decimal value)
				- the numerator and denominator of the fraction to represent (integer values)
				- a valid string value (such as "5/6", "3.5", "4")
				- another %Fraction object
				- see the constructor documentation below for details

			Arithmetic and comparison operations can also be performed on %Fraction objects, analogous to how
			these operations work on values of basic types such as an int.

			I/O with %Fractions works similarly to how it works with integers as well.
			For instance, 2/3, 8/4 and 5.6 are all valid inputs. These inputs will be stored in their reduced form.
		*/
		class Fraction
		{
			#ifndef MFRACTION_ONLY

				public:

					/*! @brief Enumeration type to track how objects of type %Fraction should be outputted.
						@sa Fraction::FORMAT */
					enum FracFormat
					{
						IM_FRAC = 0,  ///< = 0: output as improper fractions ie. 5/6, 8/5
						DECI = 1      ///< = 1: output as a decimal number ie. 3.5
					};

				private:

					//! Stores how objects of type %Fraction is to be outputted.
					/*!	This applies to all objects of type %Fraction. The default behaviour is as an improper fraction.
						@sa Fraction::setFormat(), Fraction::getFormat() */
					static FracFormat FORMAT;

				public:

					//static functions to set and get the %Fraction format
					//static functions cant access non-static variables

					/*! @brief Sets the format of how objects of type %Fraction is outputted.
						@param format What the format of the fraction should be:
							 			  - either Fraction::IM_FRAC
										  - or Fraction::DECI
					*/
					static void setFormat(const FracFormat &format);

					/*! @brief Use to see what the current output format of fractions of type %Fraction is.
						@return Returns current output format for fractions of type %Fraction.
								This is a value of type Fraction::FracFormat.
					*/
					static FracFormat getFormat();

			#endif /* #ifndef MFRACTION_ONLY */

			protected:
				/* THE DATA MEMBERS: */
				//protected allows only %Fraction class and classes tht inherites from %Fraction class access

				/*! @brief Stores sign of fraction. */
				bool sign;                       //!< Value of true for positive fractions. False for negative.
				unsigned int numerator;          //!< Stores the numerator of the fraction.
				unsigned int denominator;        //!< Stores the denominator of the fraction.

			private:
				/* HELPER METHODS PRIVATE TO CLASS */

				/*! @brief Gets the Greatest Common Divisor (GCD) of the numerator and denominator.
					@return Returns the GCD of the numerator and denominator.
							If numerator of fraction is equal to 0, method returns 0.
				*/
				unsigned int getGCD() const;

				/*! @brief Converts a fraction into its reduced improper form. */
				void reduce();      //!< For instance, 6/3 -> 2 and 8/6 -> 4/3

			public:

				/*! @name Constructors/Destructor
					A constructor initilizes (creates) an object of type %Fraction.
					A destructor destroys an object of type %Fraction when it goes out of scope. */
				//@{

				//! Default Constructor: sets fraction to 0.
				/*! This is the constructor that is used when no constructor is explicitely specifed.
					@return %Fraction object.
				*/
				Fraction();

				/*! @brief Converts a decimal number (or integer) into a fraction.
					@param number Decimal number (or integer) to convert to fraction.
					@return %Fraction object.
				*/
				Fraction(const double &number);

				/*! @brief Takes numerator and denominator of fraction.
					@param numerator Numerator of fraction.
					@param denominator Denominator of fraction.
					@return %Fraction object.
					@exception If denominator is zero, throws FR_ERROR with value FR_DENOM_ZERO. Object is left
								uncreated (also means destructor will not run as well).
				*/
				Fraction(const int &numerator, const int &denominator);

				#ifndef MFRACTION_ONLY

					/*! @brief Converts a valid character array into a fraction.
						@param frac Character array to convert. Possible values include "3.4" and "7/8".
				 					Invalid values include "3.4.5", "1/2/3" and "3.4/5.6".
						@return %Fraction object.
						@exception If input is invalid, throws FR_ERROR with value FR_STR_INVALID. Object is left
								   uncreated (also means destructor will not run as well).
					*/
					Fraction(const char *frac);

				#endif /* #ifndef MFRACTION_ONLY */

				/*! @brief Copy Constructor: copies a fraction object into the one being initilized.
					@param frac %Fraction object to copy.
					@return %Fraction object.
				*/
				Fraction(const Fraction &frac);

				//! Destructor: used to destory objects of type %Fraction.
				/*! This cannot be called explicitely. */
				~Fraction();

				//@}

				/*! @name Mutator Methods
					Use these methods to set the numerator/denominator independently.

					(to get the values of the numerator and denominator, see Fraction::operator [])
				*/
				//@{

				//! Sets the numerator of the fraction.
				/*! @param numerator The number to set the numerator of the fraction to. */
				void setNum(int numerator);

				//! Sets the denomerator of the fraction.
				/*! @param denominator The number to set the denomerator of the fraction to.
					@exception Throws FR_ERROR with value FR_DENOM_ZERO if parameter's value is 0.
				*/
				void setDen(int denominator);

				//@}

				/*! @name Member Overloaded Operators
					The following are overloaded operators which are members of the class.

					One of the "parameters" to the operation is implicit and is the "calling object".
					The calling object must be of type %Fraction.
					For instance, x[0] and --x, where x is the calling object.
				*/
				//@{

				//! Subscript operator: Pass in 0 to get numerator (signed), 1 for denominator (unsigned).
				/*! This operator must be a member function.

					@param subscript value of 0 or 1. Any other value will throw an exception.
					@return Returns an integer value (not a reference)
								- numerator (signed) if subscript = 0
								- denominator (unsigned) if subscript = 1
					@exception Throws FR_ERROR with value FR_INDEX_OUT_BOUNDS if subscript is greater than 1.
				*/
				int operator [] (const unsigned int &subscript) const;

				//! Assignment operator: Calling object is made equal to object on the right of operator.
				/*! This operator must be declared as a member function.

					@param right %Fraction object on the right of the equal sign. Automatic type conversions
								 are made when values of integer, double or string is provided.
					@return Returns reference to object to the left of the equal sign. This makes it possible to
							do x = y = z.
				*/
				Fraction &operator = (const Fraction& right);

				#ifndef MFRACTION_ONLY

					/*! @brief Negation operator: returns the negated value of the calling %Fraction object.
						@note Doesn't change the value of the calling object. The calling object must be of type %Fraction.
						@return Returns the negated value of the calling %Fraction object.
								Note that operation doesn't return a reference.
								ie. -b=c, b wont be assigned to c.
					*/
					Fraction operator - () const;

					/*! @brief Prefix increment operator: adds 1 to fraction.
						@return Returns the updated fraction object so that c = ++b is possible, (here 1 is added to
								fraction and then b is assigned to c).
						@note If returned reference instead, in cases such as ++d = b, 1 would be added to d and
							  then assigned value of b.
					*/
					Fraction operator ++ ();

					/*! @brief Prefix decrement operator: subtracts 1 from fraction.
						@return Returns the updated fraction object so that c = --b is possible, (here 1 is subtracted
								from fraction and then b is assigned to c).
						@note If returned reference instead, in cases such as --d = b, 1 would be subtracted and
							  then assigned value of b.
					*/
					Fraction operator -- ();

					/*! @brief Postfix increment operator: increments fraction by inc; if inc is 0, increments by 1.
						@param inc
								 - Implicit - c++ : increments fraction by 1 (inc will be equal to 0)
								 - Explicit - Fraction::operator ++ (value) : increments fraction by value (inc = value)
						@return Returns the updated fraction object so that c = b++ is possible, (here, b is assigned to
								c and then 1 is added to b).
						@note If returned reference instead, in cases such as d++ = b, b's value would be assigned to d
							  and then 1 would be added to d.
						@exception If inc is a negative value, throws FR_ERROR with value FR_NEG_PARAM. Calling object
								   remains unchanged.
					*/
					Fraction operator ++ (int inc);

					/*! @brief Postfix decrement operator: decrements fraction by dec; if dec is 0, decrements by 1.
						@param dec
								 - Implicit - c-- : decrements fraction by 1 (dec will be equal to 0)
								 - Explicit - Fraction::operator -- (value) : decrements fraction by value (dec = value)
						@return Returns the updated fraction object so that c = b-- is possible, (here, b is assigned to
								c and then 1 is subtracted from b).
						@note If returned reference instead, in cases such as d-- = b, b's value would be assigned to d
							  and then 1 would be subtracted from d.
						@exception If dec is a negative value, throws FR_ERROR with value FR_NEG_PARAM. Calling object
								   remains unchanged.
					*/
					Fraction operator -- (int dec);

					//! Addition/assignment operator: adds fraction on the right of operator to fraction on the left.
					/*! Calling object must be of type %Fraction. The calling object is the fraction that is on the
						left of the operator.

						@param right %Fraction object on the right of operator that is to be added. If values of
							   integer, double or strings are provided, they will be automatically converted to
							   %Fraction objects prior to addition/assignment.
						@return Returns a fraction object that is equal to the value of the updated calling object.
					*/

					Fraction operator += (const Fraction &right);

					/*! @brief Subtraction/assignment operator: subtracts fraction on the right of operator from
							   fraction on the left.

						Calling object must be of type %Fraction. The calling object is the fraction that is on the
						left of the operator.

						@param right %Fraction object on the right of operator that is to be subtracted. If values of
							   integer, double or strings are provided, they will be automatically converted to
							   %Fraction objects prior to subtraction/assignment.
						@return Returns a fraction object that is equal to the value of the updated calling object.
					*/
					Fraction operator -= (const Fraction &right);

					/*! @brief Multiplication/assignment operator: calling object is multiplied by fraction on the right
							   of the operator.

						Calling object must be of type %Fraction. The calling object is the fraction that is on the left
						of the operator.

						@param right %Fraction object on the right of operator that is to be multiplied with. If values of
							   integer, double or strings are provided, they will be automatically converted to
							   %Fraction objects prior to multiplication/assignment.
						@return Returns a fraction object that is equal to the value of the updated calling object.
					*/
					Fraction operator *= (const Fraction &right);

					/*! @brief Divition/assignment operator: calling object is divided by fraction on the right
							   of the operator.

						Calling object must be of type %Fraction. The calling object is the fraction that is on the left
						of the operator.

						@param right %Fraction object on the right of operator that is to be divided by. If values of
							   integer, double or strings are provided, they will be automatically converted to
							   %Fraction objects prior to division/assignment.
						@return Returns a fraction object that is equal to the value of the updated calling object.
					*/
					Fraction operator /= (const Fraction &right);

				#endif /* #ifndef MFRACTION_ONLY */

				//@}

				#ifndef MFRACTION_ONLY

					/* FRIENDS: OVERLOADED OPERATORS */

					/** @name Comparison Operations
						These are declared as friends so that it is possible to 25 < x or 5==x where x is a %Fraction
						object. Parameters are automatically converted to %Fraction objects when values of integer,
						double or string are provided. However, note that at least one of the parameters must be an
						object of type %Fraction. */
					//@{

					//! Equality operator: checks if two fractions are same
					/*! @param left %Fraction object to the left of the operator.
						@param right %Fraction object to the right of the operator.
						@return Returns true if the two fraction objects are equal. False otherwise.
					*/
					friend bool operator == (const Fraction &left, const Fraction &right);

					//! Inequality operator: checks if two fractions arent equal
					/*! @param left %Fraction object to the left of the operator.
						@param right %Fraction object to the right of the operator.
						@return Returns true if the two fraction objects aren't equal. False otherwise.
					*/
					friend bool operator != (const Fraction &left, const Fraction &right);

					//! Less than operator: checks if fraction on the left of operator is less than fraction on the right.
					/*! @param left %Fraction object to the left of the operator.
						@param right %Fraction object to the right of the operator.
						@return Returns true if left fraction is less than right fraction. False otherwise.
					*/
					friend bool operator < (const Fraction &left, const Fraction &right);

					/*! @brief Less than or equal to operator: checks if fraction on the left of operator is less than
							   or equal to fraction on the right.

						@param left %Fraction object to the left of the operator.
						@param right %Fraction object to the right of the operator.
						@return Returns true if left fraction is less than or equal to right fraction. False otherwise.
					*/
					friend bool operator <= (const Fraction &left, const Fraction &right);

					/*! @brief Greater than operator: checks if fraction on the left of operator is greater than
							   fraction on the right.

						@param left %Fraction object to the left of the operator.
						@param right %Fraction object to the right of the operator.
						@return Returns true if left fraction is greater than right fraction. False otherwise.
					*/
					friend bool operator > (const Fraction &left, const Fraction &right);

					/*! @brief Greater than or equal to operator: checks if fraction on the left of operator is greater than
							   or equal to fraction on the right.

						@param left %Fraction object to the left of the operator.
						@param right %Fraction object to the right of the operator.
						@return Returns true if left fraction is greater than or equal to right fraction. False otherwise.
					*/
					friend bool operator >= (const Fraction &left, const Fraction &right);

					//@}

					/** @name Arithmitic Operations
						These are declared as friends so that it is possible to 25 + x or 5/x where x is a %Fraction
						object. Parameters are automatically converted to %Fraction objects when values of integer,
						double or string are provided. However, note that at least one of the parameters must be an
						object of type %Fraction.
					*/
					//@{

					/*! @brief Addition operator: adds the two fractions on either side of the operator.
						@param left %Fraction on the left of the operator
						@param right %Fraction on the right of the operator
						@return Returns the reduced sum of the two fractions.
					*/
					friend Fraction operator + (const Fraction &left, const Fraction &right);

					/*! @brief Subtraction operator: subtracts the two fractions on either side of the operator.
						@param left %Fraction on the left of the operator
						@param right %Fraction on the right of the operator
						@return Returns the reduced difference of the two fractions.
					*/
					friend Fraction operator - (const Fraction &left, const Fraction &right);

					/*! @brief Multiplication operator: multiplies the two fractions on either side of the operator.
						@param left %Fraction on the left of the operator
						@param right %Fraction on the right of the operator
						@return Returns the reduced product of the two fractions.
					*/
					friend Fraction operator * (const Fraction &left, const Fraction &right);

					/*! @brief Division operator: left fraction is divided by the fraction on the right of the operator.
						@param left %Fraction on the left of the operator
						@param right %Fraction on the right of the operator
						@return Returns the reduced quotient of the two fractions.
					*/
					friend Fraction operator / (const Fraction &left, const Fraction &right);

					//@}

					/** @name Stream input/output operations */
					//@{

					/*! @brief Stream output: output fraction in the format specified by Fraction::FORMAT
						@param out output stream (operation assumes stream is already setup (ie. connected to file, screen etc.)
						@param fraction %Fraction object to output.
						@return Returns reference to output stream so that for instance, cout << x << y is possible.
					*/
					friend ostream &operator << (ostream &out, const Fraction &fraction);

					/*! @brief Stream input: assign input to a fraction object
						@param in input stream (operation assumes stream is already setup (ie. connected to file, screen etc.)
						@param fraction The input from stream is assigned to this fraction object.
						@return Returns reference to input stream so that for instance, cin >> x >> y is possible.
					*/
					friend istream &operator >> (istream &in, Fraction &fraction);

					//@}

				#endif /* #ifndef MFRACTION_ONLY */
		};
	#endif /* #if !defined(FRACTION_ONLY) || !defined(MFRACTION_ONLY) */
#endif /* #ifndef FRACTION_H */
