/**
 * MCalc is a matrix calculator which allows you to save matrices and evaluate
 * expressions containing more than one operator.
 * Copyright (C) 2016 Christo Lolov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.

 * Christo Lolov can be contacted by writing an e-mail to cl7815@imperial.ac.uk.
 */

#ifndef __MATRIX_H_INCLUDED__
#define __MATRIX_H_INCLUDED__

#include "Fraction.h"
#include <FL/Fl_Widget.H>

class Matrix {
private:
  int rows;
  int columns;
public:
  Fraction *elements;
  Matrix(int, int);
  ~Matrix();
  int get_rows(void);
  int get_columns(void);
  Matrix* operator + (const Matrix& other);
  Matrix* operator - (const Matrix& other);
  Matrix* operator * (Matrix& other);
  Matrix* operator * (const Fraction number);
  void operator = (Matrix& other);
  Matrix& operator = (Matrix&& other);
};

void enter(Matrix *a, int preview_flag);
void close_cb(Fl_Widget *widget, void *);
void change_value_cb(Fl_Widget *widget, void *entry);

#endif
