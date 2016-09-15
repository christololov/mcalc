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

#ifndef __MATRIX_LIST_H_INCLUDED__
#define __MATRIX_LIST_H_INCLUDED__

#include "matrix.h"

typedef struct list_elem *list_iter;

struct matrix_list {
  struct list_elem *header;
  struct list_elem *footer;
  int size;
};

struct list_elem {
  struct list_elem *next = nullptr;
  struct list_elem *prev = nullptr;
  int *name = nullptr;
  int name_size = 0;
  Matrix *elem = nullptr;
};

struct list_elem * list_alloc_elem(void);
void list_free_elem(struct list_elem *elem);
void list_init(struct matrix_list *l);
list_iter list_begin(struct matrix_list *l);
list_iter list_end(struct matrix_list *l);
void list_insert(struct matrix_list *l, list_iter iter, Matrix *elem);
void list_insert_determine_name(struct matrix_list *l, struct list_elem *prev, struct list_elem *new_elem);
list_iter list_iter_next(list_iter iter);
Matrix * list_iter_value(list_iter iter);
void list_insert_front(struct matrix_list *l, Matrix *elem);
void list_insert_back(struct matrix_list *l, Matrix *elem);
void list_delete(struct matrix_list *l, list_iter iter);
void list_destroy(struct matrix_list *l);

#endif
