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

#ifndef __OPERATIONS_H_INCLUDED__
#define __OPERATIONS_H_INCLUDED__

#include <string>
#include "matrix.h"

Matrix* add(Matrix *a, Matrix *b);
Matrix* subtract(Matrix *a, Matrix *b);
Matrix* multiply(Matrix *a, Matrix *b);
Matrix* multiply_by_number(Matrix *a, double number);
Matrix* transpose(Matrix *a);
Matrix* reduced_row_echelon_form(Matrix *a);
void swap(Fraction *a, Fraction *b);
Matrix* identity_matrix(int rows, int columns);
Matrix* put_together(Matrix *a, Matrix *b);
void split_apart(Matrix *from, Matrix *a, Matrix *b);
bool compare_elements(Matrix *from, Matrix *to);
Matrix* invert(Matrix *a);
Fraction determinant(Matrix *a);

#endif
