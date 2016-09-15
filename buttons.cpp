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

#include "buttons.h"
#include "main.h"

/**
 * Creates the unary group of buttons.
 */
void unary(Fl_Window *group) {
  // Creates the boundary box with label 'unary'.
  Fl_Box *unary = new Fl_Box(106, 165, 384, 100, "Unary");
  unary->box(FL_BORDER_BOX);
  unary->align((Fl_Align) 5);
  group->add(unary);

  // Creates the boundary box with label 'transpose'.
  Fl_Box *transpose_info = new Fl_Box(111, 165, 140, 50, "transpose");
  setup_boundary_box(group, transpose_info);
  // Creates the transpose button.
  Fl_Button *transpose = new Fl_Button(111, 170, 40, 40, "|");
  setup_button(group, transpose);

  // Creates the boundary box with label 'rref'.
  Fl_Box *rref_info = new Fl_Box(261, 165, 140, 50, "RREF");
  setup_boundary_box(group, rref_info);
  // Creates the rref button.
  Fl_Button *rref = new Fl_Button(261, 170, 40, 40, "^");
  setup_button(group, rref);

  // Creates the boundary box with label 'inverse'.
  Fl_Box *invert_info = new Fl_Box(111, 215, 140, 50, "inverse");
  setup_boundary_box(group, invert_info);
  // Creates the inverse button.
  Fl_Button *invert = new Fl_Button(111, 220, 40, 40, "&");
  setup_button(group, invert);

  // Creates the boundary box with label 'determinant'.
  Fl_Box *determinant_info = new Fl_Box(261, 215, 140, 50, "determinant");
  setup_boundary_box(group, determinant_info);
  // Creates the determinant button.
  Fl_Button *determinant = new Fl_Button(261, 220, 40, 40, "#");
  setup_button(group, determinant);
}

/**
 * Creates the binary group of buttons.
 */
void binary(Fl_Window *group) {
  // Creates the boundary box with label 'binary'.
  Fl_Box *binary = new Fl_Box(106, 85, 384, 50, "Binary");
  binary->box(FL_BORDER_BOX);
  binary->align((Fl_Align) 5);
  group->add(binary);

  // Creates the addition button.
  Fl_Button *add = new Fl_Button(111, 90, 40, 40, "+");
  setup_button(group, add);

  // Creates the subtraction button.
  Fl_Button *sub = new Fl_Button(161, 90, 40, 40, "-");
  setup_button(group, sub);

  // Creates the multiplication button.
  Fl_Button *mul = new Fl_Button(211, 90, 40, 40, "*");
  setup_button(group, mul);
}

/**
 * Creates the third group of buttons consisting of the digits from 0 to 9,
 * "(", ")", "." and "\'" (terminating character).
 */
void digits(Fl_Window *group) {
  // Creates the button 7.
  Fl_Button *seven = new Fl_Button(111, 290, 40, 40, "7");
  setup_button(group, seven);

  // Creates the button 8.
  Fl_Button *eight = new Fl_Button(161, 290, 40, 40, "8");
  setup_button(group, eight);

  // Creates the button 9.
  Fl_Button *nine = new Fl_Button(211, 290, 40, 40, "9");
  setup_button(group, nine);

  // Creates the button (.
  Fl_Button *left = new Fl_Button(261, 290, 40, 40, "(");
  setup_button(group, left);

  // Creates the button ).
  Fl_Button *right = new Fl_Button(311, 290, 40, 40, ")");
  setup_button(group, right);

  // Creates the button 4.
  Fl_Button *four = new Fl_Button(111, 340, 40, 40, "4");
  setup_button(group, four);

  // Creates the button 5.
  Fl_Button *five = new Fl_Button(161, 340, 40, 40, "5");
  setup_button(group, five);

  // Creates the button 6.
  Fl_Button *six = new Fl_Button(211, 340, 40, 40, "6");
  setup_button(group, six);

  // Creates the button 1.
  Fl_Button *one = new Fl_Button(111, 390, 40, 40, "1");
  setup_button(group, one);

  // Creates the button 2.
  Fl_Button *two = new Fl_Button(161, 390, 40, 40, "2");
  setup_button(group, two);

  // Creates the button 3.
  Fl_Button *three = new Fl_Button(211, 390, 40, 40, "3");
  setup_button(group, three);

  // Creates the button 0.
  Fl_Button *zero = new Fl_Button(111, 440, 40, 40, "0");
  setup_button(group, zero);

  // Creates the button ..
  Fl_Button *dot = new Fl_Button(161, 440, 40, 40, ".");
  setup_button(group, dot);

  // Creates the boundary box with label 'terminating'.
  Fl_Box *terminating_info = new Fl_Box(206, 435, 140, 50, "terminating");
  setup_boundary_box(group, terminating_info);
  terminating_info->box(FL_BORDER_BOX);
  // Creates the button '(terminating character).
  Fl_Button *terminating = new Fl_Button(211, 440, 40, 40, "\'");
  setup_button(group, terminating);
}

/**
 * Sets up the callback and the form of the passed in button and adds it to the
 * passed in group.
 */
void setup_button(Fl_Window *group, Fl_Button *button) {
  button->callback(click_on_cb);
  button->box(FL_PLASTIC_UP_BOX);
  group->add(button);
}

/**
 * Aligns the label of the passed in box to the top left corner and adds it to
 * the passed in group.
 */
void setup_boundary_box(Fl_Window *group, Fl_Box *box) {
  box->align((Fl_Align) 24);
  group->add(box);
}
