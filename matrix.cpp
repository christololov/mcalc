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

#include <iostream>
#include <sstream>
#include <string>
#include "matrix.h"
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Window.H>

using namespace std;

/**
 * The constructor for the Matrix class. Allocates all the elements of the
 * matrix.
 */
Matrix::Matrix (int x, int y) : rows(x), columns(y) {
  elements = new Fraction [rows * columns];
  for (int i = 0; i < rows * columns; i++) {
    elements[i] = Fraction ();
  }
}

/**
 * The destructor for the Matrix class. Returns the allocated resources.
 */
Matrix::~Matrix () {
  delete[] elements;
}

/**
 * A getter for the private field rows.
 */
int Matrix::get_rows () {
  return this->rows;
}

/**
 * A getter for the private field columns.
 */
int Matrix::get_columns() {
  return this->columns;
}

/**
 * An overloaded + operator for the addition of two matrices.
 * !!! This function may cause segmentation faults. !!!
 * A check whether the dimensions of the two matrices match is not performed.
 * POSSIBLE FUTURE ALTERNATIVE: Perform the dimensions check and return a null
 * pointer if the check fails.
 */
Matrix* Matrix::operator+ (const Matrix& other) {
  Matrix *sum = new Matrix(this->get_rows(), this->get_columns());

  for (int i = 0; i < this->get_rows() * this->get_columns(); i++) {
    sum->elements[i] = this->elements[i] + other.elements[i];
  }

  return sum;
}

/**
 * An overloaded - operator for the subtraction of two matrices.
 * !!! This function may cause segmentation faults. !!!
 * A check whether the dimensions of the two matrices match is not performed.
 * POSSIBLE FUTURE ALTERNATIVE: Perform the dimensions check and return a null
 * pointer if the check fails.
 */
Matrix* Matrix::operator- (const Matrix& other) {
  Matrix *diff = new Matrix(this->get_rows(), this->get_columns());

  for (int i = 0; i < this->get_rows() * this->get_columns(); i++) {
    diff->elements[i] = this->elements[i] - other.elements[i];
  }

  return diff;
}

/**
 * An overloaded * operator for the multiplication of two matrices.
 * !!! This function may cause segmentation faults. !!!
 * A check whether the dimensions of the two matrices match is not performed.
 * POSSIBLE FUTURE ALTERNATIVE: Perform the dimensions check and return a null
 * pointer if the check fails.
 */
Matrix* Matrix::operator* (Matrix& other) {
  Matrix *prod = new Matrix(this->get_rows(), other.get_columns());

  int t_rows = this->get_rows();
  int t_columns = this->get_columns();
  int o_columns = other.get_columns();

  for (int i = 0; i < t_rows; i++) {
    for (int j = 0; j < o_columns; j++) {
      Fraction sum;
      for (int k = 0; k < t_columns; k++) {
        sum += this->elements[i * t_columns + k] * other.elements[k * o_columns + j];
      }
      prod->elements[i * o_columns + j] = sum;
    }
  }

  return prod;
}

/**
 * An overloaded * operator for the multiplication of a matrix and a number.
 */
Matrix* Matrix::operator* (const Fraction number) {
  Matrix *prod = new Matrix(this->get_rows(), this->get_columns());

  for (int i = 0; i < this->get_rows() * this->get_columns(); i++) {
    prod->elements[i] = this->elements[i] * number;
  }

  return prod;
}

/**
 * An overloaded copy assignment.
 */
void Matrix::operator= (Matrix& other) {
  for (int i = 0; i < this->get_rows() * this->get_columns(); i++) {
    this->elements[i] = other.elements[i];
  }
}

/**
 * An overloaded move assignment.
 */
Matrix& Matrix::operator= (Matrix&& other) {
  for (int i = 0; i < this->get_rows() * this->get_columns(); i++) {
    this->elements[i] = other.elements[i];
  }

  return *this;
}

/**
 * Creates a window where the elements of a matrix can be edited by the user
 * if the preview_flag is not set. If the flag is set, then the changes are not
 * recorded.
 */
void enter(Matrix *a, int preview_flag) {
    Fl_Window *window;

    int columns = a->get_columns();
    int rows = a->get_rows();
    Fraction *elements = a->elements;

    // Creates a window based on the dimensions of the matrix.
    if (columns == 1 && rows < 10) {
      window = new Fl_Window(50 * columns + 40, 50 * rows + 30, "Enter...");
    } else if (columns < 10 && rows < 10){
      window = new Fl_Window(50 * columns + 10, 50 * rows + 30, "Enter...");
    } else {
      window = new Fl_Window(500, 500, "Enter...");
    }

    // Manually start the window group.
    window->begin();

    // Creates a scroll to hold the group.
    Fl_Scroll *scroll_bars = new Fl_Scroll(0, 0, window->w(), window->h(), nullptr);
    scroll_bars->end();

    Fl_Group *preview_group;

    // Creates a group to hold the elements' input based on the dimensions of
    // the matrix.
    if (columns == 1) {
      preview_group = new Fl_Group(0, 0, 10 + 50 * columns + 30, 10 + 50 * rows + 20);
    } else {
      preview_group = new Fl_Group(0, 0, 10 + 50 * columns, 10 + 50 * rows + 15);
    }

    preview_group->end();

    // Creates the inputs and assigns them the value of the corresponding
    // matrix element. The element is then added to the group.
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        Fl_Input *input = new Fl_Float_Input(10 + 50 * j, 10 + 50 * i, 40, 40, nullptr);

        string new_value;
        ostringstream os;

        os << elements[i * columns + j];
        new_value = os.str();

        input->value(new_value.c_str());
        input->when(FL_WHEN_CHANGED);
        input->callback(change_value_cb, &elements[i * columns + j]);

        if (preview_flag) {
          input->when(0);
        }

        preview_group->add(input);
      }
    }

    Fl_Button *done;

    // Creates a done button based on the dimensions of the matrix.
    if (columns == 1) {
      done = new Fl_Return_Button(0, 50 * rows + 10, 90, 20, "done");
    } else {
      done = new Fl_Return_Button(50 * columns - 90, 50 * rows + 5, 90, 20, "done");
    }

    // Changes the callback and the form of the done button.
    done->callback(close_cb);
    done->box(FL_PLASTIC_UP_BOX);

    // Adds the done button to the input group.
    preview_group->add(done);

    // Adds the input group to the scroll.
    scroll_bars->add(preview_group);

    window->end();
    window->show();
}

/**
 * Deletes the Enter window and its children.
 */
void close_cb(Fl_Widget *widget, void *) {
  Fl_Window *current_window = (Fl_Window *) widget->parent()->parent()->parent();

  current_window->clear();
  current_window->hide();
  delete current_window;
}

/**
 * Used to update the value of a matrix element when the user changes the value
 * in the corresponding input line.
 */
void change_value_cb(Fl_Widget *widget, void *entry) {
  Fraction *fraction = (Fraction *) entry;
  *fraction = Fraction (((Fl_Float_Input *) widget)->value());
}
