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

/** @file Fraction.cpp
	@brief This file implementes the interface of the %Fraction class.

	Private and protected members of the class are also implemented in this file.
 */

#include "Fraction.h"
#include <stdlib.h>

#if !defined(FRACTION_ONLY) || !defined(MFRACTION_ONLY)

	#include <cmath>
	#include <string>

	/*!
		@brief Determines the accuracy when converting a decimal number into a fraction.

		Currently, conversion is accurate to 3-4 decimal places.

		Increase number to increase accuracy. However, if it becomes too large, conversion operation will fail
		due to how numbers are represented in memory in c++.
	*/
	#define ACCLIMIT 100000000

	#ifndef MFRACTION_ONLY

		/**************** INITIALIZES DEFAULT FRACTION FORMAT BEHAVIOUR ****************/

		Fraction::FracFormat Fraction::FORMAT = IM_FRAC;

		/******************* DEFINES FORMAT ACCESSOR/MUTATOR METHODS *******************/

		//this method sets the fraction format (how the fraction is to be outputed)
		void Fraction::setFormat(const FracFormat &format)
		{
			FORMAT = format;
		}

		//this method returns the fraction format (how the fraction is being outputed)
		Fraction::FracFormat Fraction::getFormat()
		{
			return (FORMAT);
		}

	#endif /* #ifndef MFRACTION_ONLY */

	/*********************** HELPER METHODS PRIVATE TO CLASS ***********************/

	//returns the greatest common multiple of numerator and denominator
	unsigned int Fraction::getGCD() const
	{
		if (numerator == 0) return (0);

		int smaller = numerator, greater = denominator;

		if (smaller > greater)
		{
			smaller = denominator;
			greater = numerator;
		}

		while (true)
		{
			greater %= smaller;

			if (greater == 0) return (smaller);

			smaller %= greater;

			if (smaller == 0) return (greater);
		}
	}

	//reduces fraction to smallest numerator and denominator possible
	void Fraction::reduce()
	{
		unsigned int gcm = getGCD();

		if (gcm > 1)
		{
			numerator /= gcm;
			denominator /= gcm;
		}
		else if (gcm == 0)
			denominator = 1;
	}

	/*************************** CONSTRUCTORS/DESTRUCTORS **************************/

	//default constructor: sets fraction to 0
	Fraction::Fraction()
	{
		sign = true;
		numerator = 0;
		denominator = 1;
	}

	//constructor converts a decimal number into a fraction
	//accuracy roughly around 3-4 decimal places
	Fraction::Fraction(const double &number)
	{
		sign = number >= 0;
		denominator = 1;

		double i;
		for (i = fabs(number); i-(static_cast<unsigned int> (i)) != 0 && i < ACCLIMIT; i *= 10)
			denominator *= 10;

		numerator = static_cast<unsigned int> (i);

		reduce();
	}

	//constructor sets fraction to numerator/denominator
	Fraction::Fraction(const int &numerator, const int &denominator)
	{
		if (denominator == 0) throw (FR_DENOM_ZERO);
		else
		{
			sign = !(numerator < 0 || denominator < 0);
			this->numerator = abs(numerator);
			this->denominator = abs(denominator);

			reduce();
		}
	}

	#ifndef MFRACTION_ONLY

		//converts a valid character array into a fraction
		Fraction::Fraction(const char *frac)
		{
			//convert the character array into a string
			string fraction = frac;

			//if the string parameter has any characters other than "-0123456789./", it is invalid
			if (fraction.find_first_not_of("-0123456789./")!=-1)
				throw (FR_STR_INVALID);

			//find position of the decimal place, if present
			int pos = fraction.find_first_of(".");

			if (pos != -1)                                       //if a decimal place is present
			{   //check that all other characters are numbers only
				if (fraction.find_first_of("/")==-1 && fraction.find_first_of(".",pos+1)==-1)
					*this = Fraction(atof(fraction.c_str()));  //if so, create a Fraction object
				else
					throw (FR_STR_INVALID);                       //otherwise, string is invalid
			}
			else
			{
				pos = fraction.find_first_of("/"); //if there was no decimal place, look for a slash

				if (pos==-1)                                    //if not present, ie. string input of "5"
					*this = Fraction(atoi(fraction.c_str()));
				else if (fraction.find_first_of("/",pos+1)==-1) //if there is only 1 slash, ie. "5/6"
					*this = Fraction(atoi(fraction.substr(0,pos).c_str()),atoi(fraction.substr(pos+1,fraction.length()-pos-1).c_str()));
				else
					throw (FR_STR_INVALID);  //if there are more than 1 slash ie. 1/2/3 (Fraction objects dont support this)
			}
		}

	#endif /* #ifndef MFRACTION_ONLY */

	//copy constructor: copies another fraction into the created fraction
	Fraction::Fraction(const Fraction &frac)
	{
		sign = frac.sign;                //frac will be in reduced form already so don't
		numerator = frac.numerator;      //have to worry bout reducing the fraction being created
		denominator = frac.denominator;
	}


	//destructor: does nothing as there are no dynamic memory
	//the data members are deallocated automatically
	Fraction::~Fraction()
	{
	}

	/******************************* MUTATOR METHODS *******************************/

	//sets the numerator of the fraction
	void Fraction::setNum(int numerator)
	{
		sign = numerator >= 0;
		this->numerator = abs(numerator);
		reduce();
	}

	//sets the denominator of the fraction
	void Fraction::setDen(int denominator)
	{
		if (denominator == 0) throw (FR_DENOM_ZERO);

		if (denominator < 0) sign = !sign;
		this->denominator = abs(denominator);
		reduce();
	}
	/**************************** OVERLOADED OPERATORS *****************************/

	//the subscript operator must be a member function
	//pass in 0 to get value of numerator (signed), 1 for denominator
	int Fraction::operator [] (const unsigned int &subscript) const
	{
		if (subscript > 1) throw (FR_INDEX_OUT_BOUNDS);

		if (subscript == 0)
		{
			if (sign) return (numerator);
			else return (-static_cast<int>(numerator));
		}
		else return (denominator);
	}

	//assignment operator: must be declared as a member function
	//calling object is made equal to object on the right of operator
	Fraction& Fraction::operator = (const Fraction &right)
	{
		if (this != &right)            //nothing happens if you do x = x
		{
			sign = right.sign;
			numerator = right.numerator;
			denominator = right.denominator;
		}

		return (*this); //so tht x = y = z is possible; (x = y) returns a reference to x which gets equated to z
	}

	#ifndef MFRACTION_ONLY

		//negation operator: returns the negated value of fraction, but doesn't change original
		Fraction Fraction::operator - () const
		{
			if (sign) return (Fraction(-static_cast<int>(numerator),denominator));
			else return (Fraction(numerator,denominator));
		}

		//prefix increment operator: adds 1 to fraction
		Fraction Fraction::operator ++ ()
		{
			return (Fraction::operator ++ (1));
		}

		//prefix decrement operator: subtracts 1 from fraction
		Fraction Fraction::operator -- ()
		{
			return (Fraction::operator -- (1));
		}

		//postfix increment operator: increments fraction by inc; if inc is 0, increments by 1
		Fraction Fraction::operator ++ (int inc)
		{
			if (inc < 0) throw (FR_NEG_PARAM);

			if (inc == 0) inc = 1;

			if (sign) numerator += denominator * inc;
			else if (numerator > (denominator * inc)) numerator -= denominator*inc;
			else
			{
				sign = !sign;
				numerator = denominator * inc - numerator;
			}

			reduce();
			return (*this);
		}

		//postfix decrement operator: decrements fraction by dec; if dec is 0, decrements by 1
		Fraction Fraction::operator -- (int dec)
		{
			if (dec < 0) throw (FR_NEG_PARAM);

			if (dec == 0) dec = 1;

			if (!sign) numerator += denominator * dec;
			else if (numerator >= (denominator * dec)) numerator -= denominator*dec;
			else
			{
				sign = !sign;
				numerator = denominator*dec - numerator;
			}

			reduce();
			return (*this);
		}

		//defines the addition/assignment operator
		//adds fraction to the right of the operator to Fraction object on the left and returns this object
		Fraction Fraction::operator += (const Fraction &right)
		{
			*this = *this + right;
			return (*this);
		}

		//defines the subtraction/assignment operator
		//subtracts fraction to the right of the operator to Fraction object on the left and returns this object
		Fraction Fraction::operator -= (const Fraction &right)
		{
			*this = *this - right;
			return (*this);
		}

		//defines the multiplication/assignment operator
		//calling object is multiplied by fraction on the right of the operator and the result is assigned to the calling object
		Fraction Fraction::operator *= (const Fraction &right)
		{
			*this = *this * right;
			return (*this);
		}

		//defines the division/assignment operator
		//calling object is divided by fraction on the right of the operator and the result is assigned to the calling object
		Fraction Fraction::operator /= (const Fraction &right)
		{
			*this = *this / right;
			return (*this);
		}

		/************************ FRIENDS OVERLOADED OPERATORS *************************/

		//returns true if 2 fractions are the same
		bool operator == (const Fraction &left, const Fraction &right)
		{
			return (left.sign==right.sign && left.numerator==right.numerator && left.denominator==right.denominator);
		}

		//returns true if 2 fractions are not the same
		bool operator != (const Fraction &left, const Fraction &right)
		{
			return (left.sign!=right.sign || left.numerator!=right.numerator || left.denominator!=right.denominator);
		}

		//returns true if left fraction is less than the fraction on the right of the operator
		bool operator < (const Fraction &left, const Fraction &right)
		{
			return (static_cast<int>(left[0]*right.denominator) < static_cast<int>(right[0]*left.denominator));
		}

		//returns true if left fraction is less than or equal to the fraction on the right of the operator
		bool operator <= (const Fraction &left, const Fraction &right)
		{
			return (static_cast<int>(left[0]*right.denominator) <= static_cast<int>(right[0]*left.denominator));
		}

		//returns true if left fraction is greater than the fraction on the right of the operator
		bool operator > (const Fraction &left, const Fraction &right)
		{
			return (static_cast<int>(left[0]*right.denominator) > static_cast<int>(right[0]*left.denominator));
		}

		//returns true if left fraction is greater than or euqal to the fraction on the right of the operator
		bool operator >= (const Fraction &left, const Fraction &right)
		{
			return (static_cast<int>(left[0]*right.denominator) >= static_cast<int>(right[0]*left.denominator));
		}

		//adds 2 fractions on either side of the operator and returns the result
		Fraction operator + (const Fraction &left, const Fraction &right)
		{
			return (Fraction(left[0]*right.denominator+right[0]*left.denominator, left.denominator*right.denominator));
		}

		//subtracts 2 fractions on either side of the operator and returns the result
		Fraction operator - (const Fraction &left, const Fraction &right)
		{
			return (Fraction(left[0]*right.denominator-right[0]*left.denominator, left.denominator*right.denominator));
		}

		//multiplies 2 fractions on either side of the operator and returns the result
		Fraction operator * (const Fraction &left, const Fraction &right)
		{
			return (Fraction(left[0]*right[0], left.denominator*right.denominator));
		}

		//divides 2 fractions on either side of the operator and returns the result
		Fraction operator / (const Fraction &left, const Fraction &right)
		{
			// return (Fraction(left[0]*right.denominator, left.denominator*right[0]));
			Fraction to_return (left[0]*right.denominator, left.denominator*right[0]);
			if (to_return != Fraction ()) {
				to_return.sign = (left.sign && right.sign) || (!left.sign && !right.sign);
			}
			return to_return;
		}

		/**************************** STREAM INPUT/OUTPUT ******************************/

		//stream output
		ostream &operator << (ostream &out, const Fraction &fraction)
		{
			if (!fraction.sign) out << "-";

			if (Fraction::FORMAT == Fraction::IM_FRAC)
			{
				out << fraction.numerator;
				if (fraction.denominator != 1) out << "/" << fraction.denominator;
			}
			else out << static_cast<double>(fraction.numerator)/static_cast<double>(fraction.denominator);

			return (out);
		}

		//stream input
		istream &operator >> (istream &in, Fraction &fraction)
		{
			string input;
			in >> input;
			fraction = input.c_str();

			return (in);
		}

	#endif /* #ifndef MFRACTION_ONLY */
#endif /* #if !defined(FRACTION_ONLY) || !defined(MFRACTION_ONLY) */
