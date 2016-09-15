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

#include <cmath>
#include <iostream>
#include <list>
#include <stack>
#include "matrix_list.h"
#include "operations.h"
#include <FL/fl_ask.H>
#include <FL/Fl_Check_Button.H>

using namespace std;

/**
 * Adds two matrices if their dimensions match, else returns a null pointer.
 * POSSIBLE FUTURE ALTERNATIVE: See the suggested alternative in matrix.cpp.
 */
Matrix* add(Matrix *a, Matrix *b) {
  if (a->get_rows() == b->get_rows() && a->get_columns() == b->get_columns()) {
    return *a + *b;
  } else {
    fl_alert("The matrices' dimensions do not match!");
    return nullptr;
  }
}

/**
 * Subtracts two matrices if their dimensions match, else returns a null pointer.
 * POSSIBLE FUTURE ALTERNATIVE: See the suggested alternative in matrix.cpp.
 */
Matrix* subtract(Matrix *a, Matrix *b) {
  if (a->get_rows() == b->get_rows() && a->get_columns() == b->get_columns()) {
    return *a - *b;
  } else {
    fl_alert("The matrices' dimensions do not match!");
    return nullptr;
  }
}

/**
 * Multiplies two matrices if their dimensions match, else returns a null pointer.
 * POSSIBLE FUTURE ALTERNATIVE: See the suggested alternative in matrix.cpp.
 */
Matrix* multiply(Matrix *a, Matrix *b) {
  if (a->get_columns() == b->get_rows()) {
    return *a * *b;
  } else {
    fl_alert("The matrices' dimensions do not match!");
    return nullptr;
  }
}

/**
 * Multiplies a matrix by a number.
 */
Matrix* multiply_by_number(Matrix *a, double number) {
  return *a * number;
}

/**
 * Returns the transpose of the passed in matrix.
 */
Matrix* transpose(Matrix *a) {
  Matrix *c = new Matrix(a->get_columns(), a->get_rows());

  for (int i = 0; i < a->get_rows(); i++) {
    for (int j = 0; j < a->get_columns(); j++) {
      c->elements[j * a->get_rows() + i] = a->elements[i * a->get_columns() + j];
    }
  }

  return c;
}

/**
 * Returns the reduced row echelon form of the passed in matrix.
 * POSSIBLE FUTURE ALTERNATIVE: Split the function into four smaller ones to
 * increase readibility and clarity.
 */
Matrix* reduced_row_echelon_form(Matrix *a) {
  // Create a new matrix to hold the reduced row echelon form.
  Matrix *c = new Matrix(a->get_rows(), a->get_columns());

  // Copy the passed in matrix.
  *c = *a;

  // Create a counter for the columns.
  int j = 0;

  for (int i = 0; i < a->get_rows(); i++) {

    // If j is greater than the number of columns return the result.
    if (j >= a->get_columns()) {
      return c;
    }

    // Create a boolean to check whether all elements in the column j are zero.
    bool are_all_zero = true;

    // Check the value of each element in column j.
    for (int k = 0; k < a->get_rows(); k++) {
      if (c->elements[k * a->get_columns() + j] != Fraction ()) {
        are_all_zero = false;
      }
    }

    // Go to next iteration if there is a non-zero element.
    if (are_all_zero) {
      j++;
      continue;
    }

    // If the element at location [i, j] is zero, then swap the current row
    // with the first row, which is below the current row and has a non-zero
    // element at location [x, j] where x is the row number.
    if (c->elements[i * a->get_columns() + j] == Fraction ()) {
      // Create a boolean to check whether all elements below the current
      // element in column j are zero.
      bool are_all_below_zero = true;

      // If a non-zero element is discovered, swap the rows.
      for (int p = i + 1; p < a->get_rows(); p++) {
        if (c->elements[p * a->get_columns() + j] != Fraction ()) {
          are_all_below_zero = false;
          for (int q = 0; q < a->get_columns(); q++) {
            swap(c->elements[i * a->get_columns() + q], c->elements[p * a->get_columns() + q]);
          }
          break;
        }
      }

      // If all elements are zero go to the next column, while keeping the row
      // the same.
      if (are_all_below_zero) {
        j++;
        i--;
        continue;
      }
    }

    // Create a copy of the element at location [i, j].
    Fraction aij = c->elements[i * a->get_columns() + j];

    // Divide each element at row i by the element at location [i, j] in order
    // to make the pivot 1. The above copy of the element at location [i, j] is
    // needed after the first division the element will become one and all
    // subsequent elements will be divided by 1 and remain the same.
    // POSSIBLE FUTURE ALTERNATIVE: Perform the division in reverse.
    for (int d = 0; d < a->get_columns(); d++) {
      c->elements[i * a->get_columns() + d] /= aij;
      if (c->elements[i * a->get_columns() + d].operator[](0) == (-0)) {
        c->elements[i * a->get_columns() + d] = Fraction ();
      }
    }

    // For each row k different from i, subtract a multiple of i (i multiplied
    // by the element at location [k, j]) such that all elements above and below
    // the pivot element in i become 0.
    for (int k = 0; k < a->get_rows(); k++) {
      if (k == i) {
        continue;
      } else {
        Fraction akj = c->elements[k * a->get_columns() + j];
        for (int d = 0; d < a->get_columns(); d++) {
          c->elements[k * a->get_columns() + d] -= (c->elements[i * a->get_columns() + d] * akj);
        }
      }
    }
  }

  return c;
}

/**
 * Swaps the values of the fractions at the two addresses.
 */
void swap(Fraction *a, Fraction *b) {
  Fraction temp = *a;
  *a = *b;
  *b = temp;
}

/**
 * Returns the identity_matrix matrix with the passed in dimensions.
 */
Matrix* identity_matrix(int rows, int columns) {
  Matrix *a = new Matrix(rows, columns);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      if (i == j) {
        a->elements[i * columns + j] = Fraction (1);
      }
    }
  }

  return a;
}

/**
 * Glues together two matrices.
 * !!! This is a helper function used by invert. It most certainly will have
 * undefined behaviour when used in other contexts. !!!
 */
Matrix* put_together(Matrix *a, Matrix *b) {
  Matrix *c = new Matrix(a->get_rows(), a->get_columns() + b->get_columns());

  for (int i = 0; i < a->get_rows(); i++) {
    for (int j = 0; j < a->get_columns(); j++) {
      c->elements[i * c->get_columns() + j] = a->elements[i * a->get_columns() + j];
    }
  }

  for (int i = 0; i < b->get_rows(); i++) {
    for (int j = a->get_columns(); j < a->get_columns() + b->get_columns(); j++) {
      c->elements[i * c->get_columns() + j] = b->elements[i * b->get_columns() + j - a->get_columns()];
    }
  }

  return c;
}

/**
 * Splits apart a matrix into two smaller ones.
 * !!! This is a helper function used by invert. It most certainly will have
 * undefined behaviour when used in other contexts. !!!
 */
void split_apart(Matrix *from, Matrix *a, Matrix *b) {
  for (int i = 0; i < a->get_rows(); i++) {
    for (int j = 0; j < a->get_columns(); j++) {
      a->elements[i * a->get_columns() + j] = from->elements[i * from->get_columns() + j];
    }
  }

  for (int i = 0; i < b->get_rows(); i++) {
    for (int j = a->get_columns(); j < a->get_columns() + b->get_columns(); j++) {
      b->elements[i * b->get_columns() + j - a->get_columns()] = from->elements[i * from->get_columns() + j];
    }
  }
}

/**
 * Compares two matrices and determines whether they are equal.
 */
bool compare_elements(Matrix *from, Matrix *to) {
  if (from->get_rows() != to->get_rows() || from->get_columns() != to->get_columns()) {
    return false;
  } else {

    for (int i = 0; i < from->get_rows() * from->get_columns(); i++) {
      if (to->elements[i] != from->elements[i]) {
        return false;
      }
    }

    return true;
  }
}

/**
 * Returns the inverse of a matrix if one exists, else returns a null pointer.
 * POSSIBLE FUTURE ALTERNATIVE: Optimize the deletion of the allocated resources.
 */
Matrix* invert(Matrix *a) {
  if (a->get_rows() != a->get_columns()) {
    fl_alert("The matrices' dimensions do not match!");
    return nullptr;
  }

  if (determinant(a) == Fraction ()) {
    fl_alert("The matrix is singular!");
    return nullptr;
  }

  Matrix *i_elements = identity_matrix(a->get_rows(), a->get_columns());

  Matrix *augmented = put_together(a, i_elements);

  Matrix *rref = reduced_row_echelon_form(augmented);

  Matrix *b = new Matrix(a->get_rows(), a->get_columns());
  Matrix *c = new Matrix(a->get_rows(), a->get_columns());

  split_apart(rref, b, c);

  if (compare_elements(b, i_elements)) {
    delete i_elements;
    delete augmented;
    delete rref;
    delete b;

    return c;
  } else {
    delete i_elements;
    delete augmented;
    delete rref;
    delete b;
    delete c;

    fl_alert("The matrix doesn't have an inverse.");
    return nullptr;
  }
}

/**
 * Returns the determinant of a matrix.
 */
Fraction determinant(Matrix *a) {
  Fraction det;
  Matrix *m;

  if (a->get_rows() != a->get_columns()) {

  } else if (a->get_rows() == 2 && a->get_columns() == 2) {
    det = a->elements[3] * a->elements[0] - a->elements[1] * a->elements[2];
  } else {
    for (int j1 = 0; j1 < a->get_rows(); j1++) {
      m = new Matrix(a->get_rows() - 1, a->get_columns() - 1);
      for (int i = 1; i < a->get_rows(); i++) {
        int j2 = 0;
        for (int j = 0; j < a->get_columns(); j++) {
          if (j == j1) {
            continue;
          }

          m->elements[(i - 1) * m->get_columns() + j2] = a->elements[i * a->get_columns() + j];
          j2++;
        }
      }
      det += pow(-1.0, 1.0 + j1 + 1.0) * a->elements[j1] * determinant(m);
      delete m;
    }
  }

  return det;
}
