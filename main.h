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

#ifndef __MAIN_H_INCLUDED__
#define __MAIN_H_INCLUDED__

#include "matrix_list.h"
#include <FL/Fl_Input.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>

extern Fl_Input *rows;

void initialize_calculator(void);
void about_box(Fl_Widget *widget);
void init_window(Fl_Window *window, Fl_Group **group, int window_number);
void initialize_windows(void);
void clear_windows(void);
void redraw_windows(void);
char* convert(struct list_elem *iter);
void calculator_close_cb(Fl_Widget *widget, void *);
void click_on_cb(Fl_Widget *widget, void *);
void display_matrix_cb(Fl_Widget *widget, void *iter);
void toggle_cb(Fl_Widget *widget, void *window_ptr);
void uncheck_cb(Fl_Widget *widget, void *button);
void delete_checked_cb(Fl_Widget *widget, void *data);
void backspace_cb(Fl_Widget *widget, void *);
void calculate_cb(Fl_Widget *widget, void *args);
void initialize_matrix_cb(Fl_Widget *widget, void *);
void create_matrix_cb(Fl_Widget *widget, void *init);
void close_window_cb(Fl_Widget *widget, void *);

#endif
