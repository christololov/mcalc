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

#include "matrix_list.h"

/**
 * Allocates memory for a new list element and returns a pointer to it.
 */
struct list_elem * list_alloc_elem(void) {
  struct list_elem *elem = new struct list_elem;
  return elem;
}

/**
 * Frees the resources used by the passed in list element.
 */
void list_free_elem(struct list_elem *elem) {
  delete[] elem->name;
  delete elem->elem;
  delete elem;
}

/**
 * Initializes the passed in pointer to a list.
 */
void list_init(struct matrix_list *l) {
  l->header = list_alloc_elem();
  l->footer = list_alloc_elem();
  l->header->prev = nullptr;
  l->footer->next = nullptr;
  l->header->next = l->footer;
  l->footer->prev = l->header;
  l->size = 0;
}

/**
 * Returns a list iterator pointing to the first element in the list.
 */
list_iter list_begin(struct matrix_list *l) {
  return l->header->next;
}

/**
 * Returns a list iterator pointing to the footer of the list.
 */
list_iter list_end(struct matrix_list *l) {
  return l->footer;
}

/**
 * Inserts the passed in matrix in the matrix list before the element pointed
 * to by the iterator.
 */
void list_insert(struct matrix_list *l, list_iter iter, Matrix *elem) {
  struct list_elem *new_elem = list_alloc_elem();
  new_elem->elem = elem;

  new_elem->prev = iter->prev;
  new_elem->next = iter;

  iter->prev->next = new_elem;
  iter->prev = new_elem;

  list_insert_determine_name(l, new_elem->prev, new_elem);

  l->size++;
}

/**
 * Determines the name of the new element in the matrix list based on name of
 * the previous element in the list. The naming starts from A and continues on
 * until Z. The next name becomes AA and the process continues.
 */
void list_insert_determine_name(struct matrix_list *l,
   struct list_elem *prev, struct list_elem *new_elem) {
  if (prev == l->header) {

    new_elem->name_size = 2;
    new_elem->name = new int [2];
    new_elem->name[0] = 65;
    new_elem->name[1] = 39;

  } else if (prev->name[prev->name_size - 2] == 90) {

    new_elem->name_size = prev->name_size + 1;

    new_elem->name = new int [new_elem->name_size];

    for (int i = 0; i < prev->name_size; i++) {
      new_elem->name[i] = 65;
    }

    new_elem->name[prev->name_size] = 39;

  } else {

    new_elem->name_size = prev->name_size;

    new_elem->name = new int [new_elem->name_size];

    for (int i = 0; i < prev->name_size - 2; i++) {
      new_elem->name[i] = prev->name[i];
    }

    new_elem->name[new_elem->name_size - 2] = prev->name[prev->name_size - 2] + 1;

    new_elem->name[new_elem->name_size - 1] = 39;

  }
}

/**
 * Returns a list iterator pointing towards the element after the list iterator
 * passed in.
 */
list_iter list_iter_next(list_iter iter) {
  return iter->next;
}

/**
 * Returns the matrix at the location in the list pointed to by the passed in
 * list iterator.
 */
Matrix* list_iter_value(list_iter iter) {
  return iter->elem;
}

/**
 * Inserts the passed in matrix after the header of the list.
 */
void list_insert_front(struct matrix_list *l, Matrix *elem) {
  list_insert(l, list_begin(l), elem);
}

/**
 * Inserts the passed in matrix before the footer of the list.
 */
void list_insert_back(struct matrix_list *l, Matrix *elem) {
  list_insert(l, list_end(l), elem);
}

/**
 * Deletes the list element pointed to by the list iterator.
 */
void list_delete(struct matrix_list *l, list_iter iter) {
  if (iter == list_end(l)) {
    list_iter temporary = iter->prev;

    iter->prev->prev->next = iter;

    iter->prev = iter->prev->prev;

    list_free_elem(temporary);

    l->size--;
  } else {
    iter->prev->next = iter->next;
    iter->next->prev = iter->prev;

    list_iter temporary = iter;

    iter = list_iter_next(iter);

    list_free_elem(temporary);

    l->size--;
  }
}

/**
 * Deletes every element in the matrix list.
 */
void list_destroy(struct matrix_list *l) {
  struct list_elem *elem = l->header;

  while (elem != nullptr) {
    struct list_elem *next = elem->next;
    list_free_elem(elem);
    elem = next;
  }
}
