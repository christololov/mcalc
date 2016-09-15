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

#ifndef __PARSER_H_INCLUDED__
#define __PARSER_H_INCLUDED__

#include <list>
#include <stack>
#include "matrix.h"
#include "matrix_list.h"

/**
 * Enumeration used to weigh the precedence on an operator.
 */
enum precedence_value {NONE = 0, LOW = 1, MEDIUM = 2, HIGH = 3, VERY_HIGH = 4, VERY_VERY_HIGH};

/**
 * A struct used to hold an operator and it's precedence.
 */
struct precedence
{
  char op;
  precedence_value prec;
};

bool compare_names(int *first, int *second, int name_size);
Matrix* find_matrix(struct matrix_list *l, int *name, int name_size);
int find_size_next_argument(std::list<char> lst);
void get_next_argument(std::list<char> *lst, int *name, int size);
double get_next_numeric_argument(std::list<char> *lst, int size);
bool correct_expression(char *value);
precedence_value find_precedence(char character);
bool check_precedence_low(char character);
bool check_precedence_medium(char character);
bool check_precedence_high(char character);
void operator_logic(stack<char> *output, stack<char> *operands, bool (*function_ptr)(char), char *current);
char* infix_to_postfix(char *data);
void clean_memory(struct matrix_list *l, int counter);
void check_existance(int *name_arg_1, int name_size_arg_1, Matrix **matrix_1, struct matrix_list *l);
void clean_up(list<char> lst, struct matrix_list *l, list_iter l_iter, int counter, int **name_arg_1, int **name_arg_2);
Matrix* calculate(struct matrix_list *l, list_iter l_iter, char *postfix);

#endif
