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
#include "buttons.h"
#include "main.h"
#include "matrix.h"
#include "operations.h"
#include "parser.h"
#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Scroll.H>

using namespace std;

/**
 * Global variables
 */
struct matrix_list l;
list_iter iter;

Fl_Window *matrix_window;
Fl_Window *edit_window = new Fl_Window(80, 380, "Edit Menu");
Fl_Window *delete_window = new Fl_Window(120, 380, "Delete Menu");
Fl_Window *about_window = new Fl_Window(500, 200, "About");

Fl_Group *matrix_group;
Fl_Group *edit_group;
Fl_Group *delete_group;

Fl_Input *expression_input;

/**
 * The main function firstly initializes a list and an iterator.
 * It then calls initialize_calculator.
 * Lastly Fl::run is called in order to run the program.
 */
int main(int argc, char **argv) {
  list_init(&l);
  iter = list_begin(&l);

  initialize_calculator();

  return Fl::run();
}

/**
 * The initialize_calculator deals with setting up the calculator's interface.
 */
void initialize_calculator(void) {
  // Creates the main calculator window.
  Fl_Window *window = new Fl_Window(500, 500, "MCalc");

  // Creates the input line.
  expression_input = new Fl_Input(10, 10, 480, 30, nullptr);

  // Creates the matrix window where all the available matrices are shown.
  matrix_window = new Fl_Window(10, 80, 80, 400);
  matrix_window->end();

  // Creates the vertical separation bar between the matrix list and the
  // buttons.
  Fl_Box *seperate = new Fl_Box(100, 80, 1, 400, nullptr);
  seperate->box(FL_BORDER_BOX);

  // Creates the edit checkbox.
  Fl_Check_Button *edit = new Fl_Check_Button(10, 40, 40, 40, "&edit");
  edit->shortcut(FL_CTRL + 'e');
  edit->callback(toggle_cb, edit_window);
  edit_window->callback(uncheck_cb, edit);

  // Creates the delete checkbox.
  Fl_Check_Button *del = new Fl_Check_Button(60, 40, 40, 40, "&delete");
  del->shortcut(FL_CTRL + 'd');
  del->callback(toggle_cb, delete_window);
  delete_window->callback(uncheck_cb, del);

  // Initializes the matrix list and the edit and delete menus.
  initialize_windows();

  // Creates the unary and binary operators alongside the digits, full stop
  // and brackets.
  unary(window);
  binary(window);
  digits(window);

  // Creates the save checkbox.
  Fl_Check_Button *save = new Fl_Check_Button(350, 40, 40, 40, "&save");
  save->shortcut(FL_CTRL + 's');

  // Creates the preview checkbox.
  Fl_Check_Button *preview = new Fl_Check_Button(410, 40, 40, 40, "&preview");
  preview->shortcut(FL_CTRL + 'p');
  preview->set();

  // Creates the backspace button.
  Fl_Button *backspace = new Fl_Button(361, 390, 100, 40, "&backspace");
  backspace->shortcut(FL_CTRL + 'b');
  backspace->callback(backspace_cb);
  backspace->box(FL_PLASTIC_UP_BOX);

  // Creates the calculate button.
  Fl_Button *calculate = new Fl_Button(361, 440, 100, 40, "ca&lculate");
  calculate->shortcut(FL_CTRL + 'l');
  Fl_Widget **args = new Fl_Widget* [2];
  args[0] = save;
  args[1] = preview;
  calculate->callback(calculate_cb, args);
  calculate->box(FL_PLASTIC_UP_BOX);

  // Creates the about checkbox.
  Fl_Check_Button *about = new Fl_Check_Button(205, 40, 40, 40, "&about");
  about->shortcut(FL_CTRL + 'a');
  about->callback(toggle_cb, about_window);
  about_box(about);

  // Changes the callback of the main window, ends the group and draws it.
  window->callback(calculator_close_cb);
  window->end();
  window->show();
}

/**
 * The about_box function creates the about box for the copyright information.
 */
void about_box(Fl_Widget *widget) {
  Fl_Box *about_text = new Fl_Box(0, 0, about_window->w(), about_window->h(), "MCalc  Copyright (C) 2016  Christo Lolov\nThis program comes with ABSOLUTELY NO WARRANTY;\nfor details refer to the GNU GPL at <http://www.gnu.org/licenses/>.\nThis is free software, and you are welcome to redistribute it\nunder certain conditions; for details refer to the\nGNU GPL at <http://www.gnu.org/licenses/>.");
  about_window->add(about_text);
  about_window->callback(uncheck_cb, widget);
}

/**
 * The init_window function deals with initializing different windows and
 * their respective buttons.
 * window_number = 0 means initiate edit window
 * window_number = 1 means initiate delete window
 * window_number = 2 means initiate matrix window
 */
void init_window(Fl_Window *window, Fl_Group **group, int window_number) {
  // Creates a new group which will hold the buttons.
  *group = new Fl_Group(0, 0, window->w(), 60 + 50 * l.size - 10);
  (*group)->end();

  // Creates a new vertical scroll and adds the above created group as a child.
  Fl_Scroll *scroll = new Fl_Scroll(0, 0, window->w(), window->h() - 10);
  scroll->end();
  scroll->type(Fl_Scroll::VERTICAL);
  scroll->add(*group);

  // Adds the scroll to the respective window.
  window->add(scroll);

  int counter = 0;

  // A button is created for each matrix in the program's memory.
  // The button type and callback varies depending on the window/menu.
  // The button is added to the window's/menu's group for easier access.
  // The counter variable is used to vary the y-coordinate of the buttons.
  for (list_iter iter = list_begin(&l); iter != list_end(&l); iter = list_iter_next(iter), counter++) {
    Fl_Button *button;
    if (window_number == 0) {
      button = new Fl_Button(10, 10 + 50 * counter, 40, 40, convert(iter));
      button->callback(display_matrix_cb, list_iter_value(iter));
      button->box(FL_PLASTIC_UP_BOX);
    } else if (window_number == 1) {
      button = new Fl_Check_Button(10, 10 + 50 * counter, 40, 40, convert(iter));
    } else {
      button = new Fl_Button(10, 10 + 50 * counter, 40, 40, convert(iter));
      button->callback(click_on_cb);
      button->box(FL_PLASTIC_UP_BOX);
    }
    (*group)->add(button);
  }

  // A 'new' or 'done' button is created if the window which is being
  // initialized is the matrix list or the delete menu.
  if (window_number == 1) {
    Fl_Button *done = new Fl_Button(5, l.size * 50 + 10, 90, 20, "done");
    done->callback(delete_checked_cb);
    done->box(FL_PLASTIC_UP_BOX);
    (*group)->add(done);
  } else if (window_number == 2) {
    Fl_Button *add_new = new Fl_Button(10, 10 + 50 * counter, 40, 40, "&new");
    add_new->shortcut(FL_CTRL + 'n');
    add_new->callback(initialize_matrix_cb);
    add_new->box(FL_PLASTIC_UP_BOX);
    (*group)->add(add_new);
  }
}

/**
 * The initialize_windows function calls init_window on the matrix list,
 * the edit menu and the delete menu.
 */
void initialize_windows(void) {
  init_window(matrix_window, &matrix_group, 2);
  init_window(edit_window, &edit_group, 0);
  init_window(delete_window, &delete_group, 1);
}

/**
 * The clear_windows function deals with deleting the allocated names of the
 * buttons in the matrix list, edit menu and delete menu. It then clears the
 * windows and deletes all remaining allocated data.
 */
void clear_windows(void) {
  int max_num_children = matrix_group->children();

  Fl_Widget *const *available_children = matrix_group->array();
  Fl_Widget *const *edit_children = edit_group->array();
  Fl_Widget *const *delete_children = delete_group->array();

  for (int i = 0; i < max_num_children - 1; i++) {
    delete[] available_children[i]->label();
    delete[] edit_children[i]->label();
    delete[] delete_children[i]->label();
  }

  matrix_window->clear();
  edit_window->clear();
  delete_window->clear();
}

/**
 * The redraw_windows function redraws all windows by first clearing their
 * contents and then reinitializing them. Lastly the redraw function is
 * called on each window.
 */
void redraw_windows(void) {
  clear_windows();

  initialize_windows();

  matrix_window->redraw();
  edit_window->redraw();
  delete_window->redraw();
}

/**
 * The convert function returns the name of the element in the list pointed to
 * by the passed in list iterator.
 */
char* convert(struct list_elem *iter) {
  char *name = new char [iter->name_size + 1];

  for (int i = 0; i < iter->name_size; i++) {
    name[i] = (char) iter->name[i];
  }

  name[iter->name_size] = '\0';

  return name;
}

/**
 * The calculator_close_cb function deals with cleaning up the allocated resources
 * by the calculator when the main window is closed.
 */
void calculator_close_cb(Fl_Widget *widget, void*) {
  Fl_Window *window = (Fl_Window *) widget;

  clear_windows();

  list_destroy(&l);

  window->hide();
}

/**
 * The click_on_cb function writes the value of the widget that called it
 * to the input line.
 */
void click_on_cb(Fl_Widget *widget, void *) {
  const char *value = expression_input->value();
  string str (value);
  str.append(widget->label());
  const char *new_value = str.c_str();
  expression_input->value(new_value);
}

/**
 * The display_matrix_cb function shows the elements of the selected matrix
 * and allows the user to edit them.
 */
void display_matrix_cb(Fl_Widget *widget, void *data) {
  Matrix *matrix = (Matrix *) data;
  enter(matrix, 0);
}

/**
 * The toggle_cb function hides or shows the respective menu associated with of
 * the checkbox that called it.
 */
void toggle_cb(Fl_Widget *widget, void *window_ptr) {
  Fl_Check_Button *checkbox = (Fl_Check_Button *) widget;
  Fl_Window *window = (Fl_Window *) window_ptr;
  if (checkbox->value()) {
    window->redraw();
    window->show();
  } else {
    window->hide();
  }
}

/**
 * The uncheck_cb function is used when the edit or delete window
 * is closed. It unticks the checkbox used to activate the window.
 */
void uncheck_cb(Fl_Widget *widget, void *button) {
  Fl_Check_Button *check_button = (Fl_Check_Button *) button;
  check_button->clear();
  Fl_Window *window = (Fl_Window *) widget;
  window->hide();
}

/**
 * The delete_checked_cb function goes through all checkboxes in the
 * delete group and deletes the matrices associated with the ticked ones.
 */
void delete_checked_cb(Fl_Widget *widget, void *) {
  list_iter iter = list_begin(&l);
  Fl_Widget *const *children = delete_group->array();

  for (int j = 0; j < delete_group->children() - 1; j++) {

    list_iter temporary = iter;

    if (iter != list_end(&l)) {
      temporary = list_iter_next(iter);
      if (temporary != list_end(&l)) {
        temporary = list_iter_next(temporary);
      }
    }

    if ((int) ((Fl_Check_Button *)children[j])->value()) {
      list_delete(&l, iter);
      iter = temporary;
    } else {
      iter = list_iter_next(iter);
    }
  }

  redraw_windows();
}

/**
 * The backspace_cb deletes the rightmost character in the input line.
 * If there are no characters it does nothing.
 */
void backspace_cb(Fl_Widget *widget, void *) {
  const char *value = expression_input->value();
  string str (value);
  if (str.length() == 0) {
    return;
  } else {
    str.pop_back();
    const char *new_value = str.c_str();
    expression_input->value(new_value);
  }
}

/**
 * The calculate_cb function evaluates the expression in the input line.
 */
void calculate_cb(Fl_Widget *widget, void *args) {
  Fl_Widget **widgets = (Fl_Widget **) args;

  // If the input line is empty, then the function returns.
  if (*expression_input->value() == '\0') {
    return;
  }

  char *expression = (char *) expression_input->value();

  int save_value = (int) ((Fl_Check_Button *) widgets[0])->value();
  int preview_value = (int) ((Fl_Check_Button *) widgets[1])->value();

  // Checks if the expression is valid. If it is not, then the function returns.
  if (!correct_expression(expression)) {
    fl_alert("This is not a valid expression!");
    return;
  }

  // Changes the expression from infix notation to postfix notation.
  char *postfix = infix_to_postfix(expression);

  // If there were problems with the postfix conversion, then the function
  // returns.
  if (postfix == nullptr) {
    return;
  }

  // Calls the calculate function and assigns the result to a new matrix.
  Matrix *calculated = calculate(&l, iter, postfix);

  // Return the resources allocated for postfix as it is no longer needed.
  delete[] postfix;

  // If there were problems with the calculations, then the function returns.
  if (calculated == nullptr) {
    return;
  }

  // If the preview checkbox is ticked, then the calculated matrix is previewed.
  if (preview_value) {
    enter(calculated, 1);
  }

  // If the save checkbox is unticked, then the calculated matrix is deleted
  // from the matrix list.
  if (!save_value) {
    list_delete(&l, list_end(&l));
  }

  redraw_windows();
}

/**
 * The initialize_matrix_cb function deals with initializing a new matrix.
 */
void initialize_matrix_cb(Fl_Widget *widget, void *) {
  // The initialize window is created.
  Fl_Window *window = new Fl_Window(150, 150, "Initialize...");

  // The input lines for the matrix dimensions are created.
  Fl_Input *rows = new Fl_Int_Input(80, 10, 60, 20, "rows: ");
  Fl_Input *columns = new Fl_Int_Input(80, 50, 60, 20, "columns: ");

  // The dimensions of matrix are stored into an array in order to be passed
  // to the create_matrix_cb when the matrix is created.
  Fl_Widget **widgets = new Fl_Widget* [2];
  widgets[0] = rows;
  widgets[1] = columns;

  // The done button is created.
  Fl_Button *done = new Fl_Return_Button(10, 90, 130, 20, "done");
  done->callback(create_matrix_cb, widgets);
  done->box(FL_PLASTIC_UP_BOX);

  // The cancel button is created.
  Fl_Button *cancel = new Fl_Button(10, 120, 130, 20, "&cancel");
  cancel->shortcut(FL_CTRL + 'c');
  cancel->callback(close_window_cb, widgets);
  cancel->box(FL_PLASTIC_UP_BOX);

  window->end();
  window->show();
}

/**
 * The create_matrix_cb function creates a matrix with the specified dimensions.
 */
void create_matrix_cb(Fl_Widget *widget, void *data) {
  Fl_Widget **widgets = (Fl_Widget **) data;

  int rows = atoi(((Fl_Int_Input *) widgets[0])->value());
  int columns = atoi(((Fl_Int_Input *) widgets[1])->value());

  // If the dimensions are not positive, then the process is aborted and the
  // matrix is not created.
  if (rows <= 0 || columns <= 0) {
    fl_alert("Both dimensions must be positive!");
    return;
  }

  // A matrix with the specified dimensions is created and the user is
  // prompted to enter the elements.
  Matrix *matrix = new Matrix(rows, columns);
  enter(matrix, 0);

  // The matrix is inserted in the list of matrices.
  list_insert(&l, iter, matrix);

  redraw_windows();

  Fl_Window *window = (Fl_Window *) widget->parent();
  window->hide();
}

/**
 * The close_window_cb function deals with returning the allocated resources
 * by the initialize_matrix_cb.
 */
void close_window_cb(Fl_Widget *widget, void *data) {
  Fl_Window *window = (Fl_Window *) widget->parent();
  Fl_Widget **widgets = (Fl_Widget **) data;
  delete[] widgets;
  window->clear();
  window->hide();
  delete window;
}
