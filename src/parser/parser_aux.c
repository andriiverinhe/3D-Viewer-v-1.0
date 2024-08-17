/**
 * @file parser_aux.c
 * @author mitchelk, nenamaxi and dannamer
 * @brief Auxiliary file for file processing.
 * @version 0.1
 * @date 2024-03-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "parser.h"

/**
 * @brief Creates a matrix of the given dimensions.
 *
 * This method creates a matrix with the specified number of rows and columns
 * and initializes all its elements to zero.
 *
 * @param rows Number of rows in the matrix.
 * @param columns The number of columns in the matrix.
 * @param result Pointer to the data structure into which the result of creating
 * the matrix will be written.
 * @return int Matrix creation error code (PARSER_OK if successful).
 */
int create_matrix(int rows, int columns, matrix_t *result) {
  int error = PARSER_OK;

  result->rows = rows;
  result->columns = columns;

  result->matrix = (double **)calloc(rows, sizeof(double *));

  check_mx_allocation(result->matrix, &error);

  if (!error) {
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = (double *)calloc(columns, sizeof(double));

      check_row_allocation(result, i, &error);
    }
  }

  return error;
}

/**
 * @brief Frees the memory allocated for the matrix.
 *
 * This function frees memory allocated for a matrix of type matrix_t.
 *
 * @param mx Pointer to a matrix_t structure containing the matrix.
 * @param index Index indicating the number of rows of the matrix.
 */
void error_free(matrix_t *mx, int index) {
  if (mx) {
    for (int j = 0; j < index; j++)
      if (mx->matrix[j]) free(mx->matrix[j]);

    if (mx->matrix) free(mx->matrix);
  }
}

/**
 * @brief Frees the memory allocated for the matrix.
 *
 * This method frees the memory allocated for storing the matrix.
 *
 * @param A Pointer to the matrix data structure to be deleted.
 */
void remove_matrix(matrix_t *A) {
  if (A) {
    if (A->matrix) {
      for (int i = 0; i < A->rows; i++) {
        if (A->matrix[i]) {
          free(A->matrix[i]);
          A->matrix[i] = NULL;
        }
      }

      if (A->matrix) free(A->matrix);

      A->matrix = NULL;
      A->rows = 0;
      A->columns = 0;
    }
  }
}

/**
 * @brief Checks if memory for matrix double *array was actually allocated
 * @param mx a pointer to a array of double * varables
 * @param error a pointer to an error variable
 */
void check_mx_allocation(double **mx, int *error) {
  if (mx == NULL) *error = PARSER_FALSE;
}

/**
 * @brief Checks if memory for matrix double array was actually allocated
 * @param mx a matrix_t pointer
 * @param index a number of allocated array elements
 * @param error a pointer to an error code
 */
void check_row_allocation(matrix_t *mx, int index, int *error) {
  if (mx->matrix[index] == NULL) {
    error_free(mx, index);
    *error = PARSER_FALSE;
  }
}

/**
 * @brief Checks whether the character ch is equal to the character compCh.
 *
 * This function compares the character ch with the character compCh and returns
 * 1 if they are equal, or 0 otherwise.
 *
 * @param ch The character to check.
 * @param compCh The character to compare ch with.
 * @return Returns 1 if the characters are equal, 0 otherwise.
 */
int check_symbol(const char ch, const char compCh) { return (ch == compCh); }

/**
 * @brief if current string contains vertex information
 * @param buffer
 * @return 1 -yes, 2 - no
 */
int is_vertex(char *buffer) {
  int res = 0;

  if (check_symbol(buffer[0], 'v') && check_symbol(buffer[1], ' ')) {
    res = 1;
  }

  return res;
}

/// @brief if current string contains facet information
/// @param buffer
/// @return 1 -yes, 2 - no
int is_facet(char *buffer) {
  int res = 0;

  if (check_symbol(buffer[0], 'f') && check_symbol(buffer[1], ' ')) {
    res = 1;
  }

  return res;
}

/**
 * @brief Sets the scale to display the model.
 *
 * This method sets the scale to display the model according to
 * minimum and maximum coordinate values.
 *
 * @param minValue Minimum coordinate value.
 * @param maxValue The maximum value of the coordinate.
 * @param data_ Pointer to the model data structure.
 */
void setScaling(const double minValue, const double maxValue, data *data_) {
  if (data_) {
    for (int i = 0; i <= (int)ZP; i++) {
      if (i % 2)
        data_->scale[i] = minValue;
      else
        data_->scale[i] = maxValue;
    }
  }
}

/**
 * @brief Frees memory occupied by the model data structure.
 *
 * This method frees the memory allocated for storing the vertices and faces of
 * the model.
 *
 * @param drawing_data Pointer to the model data structure.
 */
void free_data(data *drawing_data) {
  if (drawing_data) {
    for (unsigned int i = 0; i < drawing_data->facets_count; i++) {
      if (drawing_data->polygons[i].pInds)
        free(drawing_data->polygons[i].pInds);
    }

    if (drawing_data->polygons) free(drawing_data->polygons);

    remove_matrix(&drawing_data->vertexes);

    drawing_data->vertex_count = 0.0;
    drawing_data->facets_count = 0.0;

    setScaling(0, 0, drawing_data);
  }
}

/**
 * @brief Returns the maximum or minimum value along the given axis.
 *
 * This method returns the maximum or minimum value along a given axis for the
 * model's vertices.
 *
 * @param drawing_data Pointer to the model data structure.
 * @param axec Axis number (X, Y or Z).
 * @param mode Mode: 1 to search for the minimum value, 0 to search for the
 * maximum value.
 * @return double The found value is returned.
 */
static double getValueMaxOrMin(const data *drawing_data, const size_t axec,
                               const int mode) {
  double value = drawing_data->vertexes.matrix[0][axec];
  for (int i = 0; i < drawing_data->vertexes.rows; i++) {
    if ((mode == 1) && (value > drawing_data->vertexes.matrix[i][axec]))
      value = drawing_data->vertexes.matrix[i][axec];
    if ((mode == 0) && (value < drawing_data->vertexes.matrix[i][axec]))
      value = drawing_data->vertexes.matrix[i][axec];
  }
  return value;
}

/**
 * @brief Sets a new model scale based on extreme values in the X, Y, and Z
 * axes.
 *
 * This method sets new model scale values based on extreme values along the X,
 * Y, and Z axes.
 *
 * @param drawing_data Pointer to the model data structure.
 */
void setNewScale(data *drawing_data) {
  if (drawing_data) {
    drawing_data->scale[XM] = getValueMaxOrMin(drawing_data, X, 1);
    drawing_data->scale[XP] = getValueMaxOrMin(drawing_data, X, 0);
    drawing_data->scale[YM] = getValueMaxOrMin(drawing_data, Y, 1);
    drawing_data->scale[YP] = getValueMaxOrMin(drawing_data, Y, 0);
    drawing_data->scale[ZM] = getValueMaxOrMin(drawing_data, Z, 1);
    drawing_data->scale[ZP] = getValueMaxOrMin(drawing_data, Z, 0);
  }
}