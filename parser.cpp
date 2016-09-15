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

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include "operations.h"
#include "parser.h"
#include <FL/fl_ask.H>

using namespace std;

/**
 * Compares the two arrays of integers and evaluates whether they are the same.
 * Since the matrix names are stored as arrays of integers, this function is
 * used to compare names.
 */
bool compare_names(int *first, int *second, int name_size) {
  for (int i = 0; i < name_size; i++) {
    if (first[i] != second[i]) {
      return false;
    }
  }

  return true;
}

/**
 * Given a matrix list and a name the function returns a pointer to the matrix.
 * If the matrix is not found it returns a null pointer.
 */
Matrix* find_matrix(struct matrix_list *l, int *name, int name_size) {
  int counter = 0;

  for (list_iter iter = list_begin(l); iter != list_end(l); iter = list_iter_next(iter), counter++) {
    if (iter->name_size == name_size) {
      if (compare_names(iter->name, name, name_size)) {
        return list_iter_value(iter);
      }
    }
  }

  return nullptr;
}

/**
 * Returns the size of the next argument given a list of characters.
 */
int find_size_next_argument(list<char> lst) {
  list<char>::iterator iter = lst.begin();
  int size = 0;

  if (lst.size() == 0) {
    return 0;
  }

  // Keep on increasing the size until a terminating character or the end is
  // reached.
  while (*iter != '\'' && *iter != '\0') {
    size++;
    iter++;
  }

  // Account for the terminating character as it is part of the name.
  size++;

  return size;
}

/**
 * Get the name of the next argument in the list by removing the passed in size
 * characters from it. Used for matrix names.
 */
void get_next_argument(list<char> *lst, int *name, int size) {
  for (int i = 0; i < size; i++) {
    name[i] = (char) lst->front();
    lst->pop_front();
  }
}

/**
 * Get the name of the next argument in the list by removing the passed in size
 * characters from it. Used for numbers.
 */
double get_next_numeric_argument(list<char> *lst, int size) {
  char *c_str = new char [size];

  for (int i = 0; i < size - 1; i++) {
    c_str[i] = (char) lst->front();
    lst->pop_front();
  }

  lst->pop_front();

  c_str[size - 1] = '\0';

  double number = atof((const char *) c_str);

  delete[] c_str;

  return number;
}

/**
 * Used to check whether the expression in the input line is valid.
 */
bool correct_expression(char *value) {
  char *current = value;
  int arguments = 0;
  int operators = 0;
  int terminating = 0;

  while (*current != '\0') {
    if (isupper((int)*current) || isdigit((int)*current)) {
      // Check if the current character is an argument by checking whether it is
      // the beginning of a matrix name or the first digit of a number.
      arguments++;

      // A matrix can have a name longer than one character and a number can
      // consist of more than one digit. Once a matrix name or a number is
      // detected, the remainder of the name or the number is skipped.
      do {
        current++;
      } while (isdigit((int)*current) || *current == '.');
      continue;
    } else if (islower((int)*current)) {
      return false;
    } else if (*current == '+' || *current == '-' || *current == '*') {
      // Check if the current character is an operator.
      operators++;
    } else if (*current == '\'') {
      // Check if the current character is a terminating character.
      terminating++;
    }

    current++;
  }

  // If the number of arguments does not equal the number of terminating
  // characters, then the expression is incorrectly typed. Likewise, if the
  // number of arguments is not equal to the number of operators plus one, then
  // the expression is incorrectly typed.
  if (arguments != terminating || arguments != (operators + 1)) {
    return false;
  }

  return true;
}

/**
 * A function which returns the precedence of the passed in character.
 */
precedence_value find_precedence(char character) {
  struct precedence precedence_table[] =
  { { '+', LOW },
    { '-', LOW },
    { '*', MEDIUM },
    { '|', HIGH },
    { '^', HIGH },
    { '&', HIGH },
    { '#', HIGH },
    { '(', VERY_HIGH },
    { ')', VERY_VERY_HIGH } };

  for (int i = 0; i < 9; i++) {
    if (character == precedence_table[i].op) {
      return precedence_table[i].prec;
    }
  }

  return NONE;
}

/**
 * A helper function used to check whether the precedence of the passed in
 * character is lower than the precedence of the character on top of the
 * operands stack.
 */
bool check_precedence_low(char character) {
  return true;
}

/**
 * A helper function used to check whether the precedence of the passed in
 * character is lower than the precedence of the character on top of the
 * operands stack.
 */
bool check_precedence_medium(char character) {
  return find_precedence(character) <= LOW;
}

/**
 * A helper function used to check whether the precedence of the passed in
 * character is lower than the precedence of the character on top of the
 * operands stack.
 */
bool check_precedence_high(char character) {
  return find_precedence(character) <= MEDIUM;
}

/**
 * A helper function used to decide how to continue the convertion from infix to
 * postfix notation based on the current operator.
 */
void operator_logic(stack<char> *output, stack<char> *operands, bool (*function_ptr)(char), char *current) {
  bool (*function)(char) = function_ptr;

  if ((*operands).empty() || (*operands).top() == '(' || function((*operands).top())) {

    (*operands).push(*current);

  } else {

    while (!(*operands).empty()) {
      if ((*operands).top() != '(' && !function((*operands).top())) {
        (*output).push((*operands).top());
        (*operands).pop();
      } else {
        break;
      }
    }

    (*operands).push(*current);
  }
}

/**
 * Used to turn the notation of the expression from infix to postfix.
 */
char* infix_to_postfix(char *data) {
  string input = string(data);
  int open = count(input.begin(), input.end(), '(');
  int close = count(input.begin(), input.end(), ')');

  if (open != close) {
    fl_alert("The number of brackets does not match!");
    return nullptr;
  }

  std::size_t first_operator = input.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.'");

  if (first_operator == std::string::npos) {
    fl_alert("You have no operators!");
    return nullptr;
  }

  stack<char> output;
  stack<char> operands;
  char *current = data;

  while (*current != '\0') {

    switch (find_precedence(*current)) {
      case LOW:
        operator_logic(&output, &operands, &check_precedence_low, current);
        break;
      case MEDIUM:
        operator_logic(&output, &operands, &check_precedence_medium, current);
        break;
      case HIGH:
        operator_logic(&output, &operands, &check_precedence_high, current);
        break;
      case VERY_HIGH:
        operands.push(*current);
        break;
      case VERY_VERY_HIGH:
        while (!operands.empty()) {
          if (operands.top() == '(') {
            operands.pop();
            break;
          } else {
            output.push(operands.top());
            operands.pop();
          }
        }
        break;
      default:
        output.push(*current);
        break;
    }

    current++;
  }

  while (!operands.empty()) {
    output.push(operands.top());
    operands.pop();
  }

  char *postfix = new char [output.size() + 1];

  postfix[output.size()] = '\0';

  for (int i = output.size() - 1; i >= 0; i--) {
    postfix[i] = output.top();
    output.pop();
  }

  return postfix;
}

/**
 * Used to clean the intermediate matrices from the matrix list.
 */
void clean_memory(struct matrix_list *l, int counter) {
  if (!counter) {
    return;
  }

  list_iter iter = list_begin(l);
  int list_size = l->size;
  int start = list_size - counter;

  for (int i = 0; i < list_size - 1; i++) {
    // A temporary list iterator used to store the state of the current list
    // iterator in case an element is deleted.
    list_iter temporary = iter;

    // If the end of the list is not reached the temporary list iterator is set
    // to point to the element after the current list iterator. This is needed
    // because if the element pointed to by the current list iterator is
    // deleted, then list_iter_next should not be called on the current list
    // iterator because it will have undefined behavior.
    if (iter != list_end(l)) {
      temporary = list_iter_next(iter);
    }

    if (i < start) {
      // If no element is deleted it is safe to call list_iter_next on the
      // current iterator.
      iter = list_iter_next(iter);
      continue;
    } else {
      list_delete(l, iter);
      // If an element is deleted then the current list iterator becomes the
      // temporary list iterator because it is not safe to call list_iter_next.
      iter = temporary;
    }
  }
}

/**
 * Used to alert the user if the matrix required for the calculation does not exist.
 */
void check_existance(int *name_arg_1, int name_size_arg_1, Matrix **matrix_1, struct matrix_list *l) {
  *matrix_1 = find_matrix(l, name_arg_1, name_size_arg_1);
  if (*matrix_1 == nullptr) {
    fl_alert("The matrix you want to use either does not exist or is not a matrix!");
  }
}

/**
 * Used to clean the resources allocated in calculate.
 */
void clean_up(list<char> lst, struct matrix_list *l, list_iter l_iter, int counter, int **name_arg_1, int **name_arg_2) {
  lst.clear();
  clean_memory(l, counter);
  delete[] *name_arg_1;
  delete[] *name_arg_2;
}

/**
 * Carries out the evaluation of the supplied postfix expression. Returns a
 * pointer to the result or a null pointer if a problem was encountered.
 */
Matrix* calculate(struct matrix_list *l, list_iter l_iter, char *postfix) {
  list<char> lst;
  char *current = postfix;
  bool mul_by_number = false;
  bool first_number = false;

  int counter = 0;

  while (*current != '\0') {
    if (*current != '+' && *current != '-' && *current != '*'
        && *current != '|' && *current != '^' && *current != '&'
        && *current != '#') {

      stack<char> stck;
      while(*current != '\'') {
        stck.push(*current);
        current++;
      }
      lst.push_front(*current);
      while (!stck.empty()) {
        lst.push_front(stck.top());
        stck.pop();
      }
    } else {

      double number;

      int name_size_arg_1 = 0;
      int *name_arg_1 = nullptr;
      int name_size_arg_2 = 0;
      int *name_arg_2 = nullptr;

      Matrix *matrix_1 = nullptr;
      Matrix *matrix_2 = nullptr;
      Matrix *matrix_3 = nullptr;

      name_size_arg_1 = find_size_next_argument(lst);
      name_arg_1 = new int [name_size_arg_1];

      if (*current == '|' || *current == '^'
          || *current == '&' || *current == '#') {

        get_next_argument(&lst, name_arg_1, name_size_arg_1);

        check_existance(name_arg_1, name_size_arg_1, &matrix_1, l);

        if (matrix_1 == nullptr) {
          clean_up(lst, l, l_iter, counter, &name_arg_1, &name_arg_2);
          return nullptr;
        }

        switch (*current) {
          case '|':
            matrix_3 = transpose(matrix_1);
            break;
          case '^':
            matrix_3 = reduced_row_echelon_form(matrix_1);
            break;
          case '&':
            matrix_3 = invert(matrix_1);
            break;
          case '#':
            matrix_3 = new Matrix(1, 1);
            *matrix_3->elements = determinant(matrix_1);
            break;
        }

        goto skip;
      }

      if (*current == '*' && lst.front() >= '0' && lst.front() <= '9') {
        number = get_next_numeric_argument(&lst, name_size_arg_1);
        mul_by_number = true;
        first_number = true;
      } else {
        get_next_argument(&lst, name_arg_1, name_size_arg_1);
      }

      name_size_arg_2 = find_size_next_argument(lst);
      name_arg_2 = new int [name_size_arg_2];

      if (*current == '*' && lst.front() >= '0' && lst.front() <= '9') {
        number = get_next_numeric_argument(&lst, name_size_arg_2);
        mul_by_number = true;
      } else {
        get_next_argument(&lst, name_arg_2, name_size_arg_2);
      }

      if (mul_by_number) {
        if (first_number) {
          check_existance(name_arg_2, name_size_arg_2, &matrix_2, l);

          if (matrix_2 == nullptr) {
            clean_up(lst, l, l_iter, counter, &name_arg_1, &name_arg_2);
            return nullptr;
          }

          matrix_3 = multiply_by_number(matrix_2, number);
          first_number = false;
        } else {
          check_existance(name_arg_1, name_size_arg_1, &matrix_1, l);

          if (matrix_1 == nullptr) {
            clean_up(lst, l, l_iter, counter, &name_arg_1, &name_arg_2);
            return nullptr;
          }

          matrix_3 = multiply_by_number(matrix_1, number);
        }

        mul_by_number = false;
        goto skip;
      } else {
        check_existance(name_arg_1, name_size_arg_1, &matrix_1, l);
        check_existance(name_arg_2, name_size_arg_2, &matrix_2, l);

        if (matrix_1 == nullptr || matrix_2 == nullptr) {
          clean_up(lst, l, l_iter, counter, &name_arg_1, &name_arg_2);
          return nullptr;
        }
      }

      if (*current == '+') {
        matrix_3 = add(matrix_2, matrix_1);
      } else if (*current == '-') {
        matrix_3 = subtract(matrix_2, matrix_1);
      } else {
        matrix_3 = multiply(matrix_2, matrix_1);
      }

skip:
      if (matrix_3 == nullptr) {
        clean_up(lst, l, l_iter, counter + 1, &name_arg_1, &name_arg_2);
        return nullptr;
      }

      list_insert(l, l_iter, matrix_3);

      counter++;

      int name_result_size = l_iter->prev->name_size;
      int *name_result = l_iter->prev->name;

      for (int i = name_result_size - 1; i >= 0; i--) {
        lst.push_front((char) name_result[i]);
      }

      delete[] name_arg_1;
      delete[] name_arg_2;
    }

    current++;
  }

  lst.clear();

  clean_memory(l, counter);

  list_insert_determine_name(l, l_iter->prev->prev, l_iter->prev);

  return l_iter->prev->elem;
}
