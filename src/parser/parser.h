/**
 * @file parser.h
 * @author mitchelk, nenamaxi and dannamer
 * @brief Header file for file processing.
 * @version 0.1
 * @date 2024-03-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef EXAMPLE_H
#define EXAMPLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/// @brief Macro for coordinate size.
#define ST_SIZE_COORDINATE 3

/// @brief Macro for parser error code
#define PARSER_OK 0
/// @brief Macro for parser error code
#define PARSER_FALSE 1

/**
 * @brief A structure representing the edges of the model.
 *
 * This structure contains information about the faces of the model, including
 * the number of vertices and the indices of the vertices that form the face.
 */
typedef struct facets {
  unsigned int count_v; /**< Number of vertices forming a face. */
  unsigned int
      *pInds; /**< Array of indices of the vertices that form the face. */
} polygon_t;

/**
 * @brief A structure representing the matrix.
 *
 * This structure contains a two-dimensional array of matrix elements, as well
 * as the number of rows and columns of the matrix.
 */
typedef struct matrix_struct {
  double **matrix; /**< Two-dimensional array of matrix elements. */
  int rows;        /**< Number of matrix rows. */
  int columns;     /**< Number of matrix columns. */
} matrix_t;

/**
 * @brief A structure representing the data to be displayed.
 *
 * This structure contains information about the number of vertices and faces,
 * an array of faces, a matrix of vertices, scaling and start flag for GIF
 * animation.
 */
typedef struct Data {
  unsigned int vertex_count; /**< Number of vertices. */
  unsigned int facets_count; /**< Number of faces. */
  polygon_t *polygons;       /**< Array of faces. */
  matrix_t vertexes;         /**< Matrix of vertices. */
  double scale[6];           /**< Scaling array. */
  int gif_start;             /**< Flag for the start of GIF animation. */
} data;

/// @brief Enumeration structure for affine rotation operation.
typedef enum {
  XM = 0, /**< Field indicating the direction to the left */
  XP = 1, /**< Field indicating the direction to the right */
  YM = 2, /**< Field indicating the direction to the down */
  YP = 3, /**< Field indicating the direction to the up */
  ZM = 4, /**< Near */
  ZP = 5  /**< Far */
} Rotation;

/// @brief Enumeration structure for coordinade
typedef enum { X = 0, Y = 1, Z = 2 } INDEX_COORDINATE;

int parser(data *drawing_data, char *filename);

// AUXILIARY
int memory_handling(data *drawing_data);
int create_matrix(int rows, int columns, matrix_t *result);
void remove_matrix(matrix_t *A);
void check_mx_allocation(double **mx, int *error);
void check_row_allocation(matrix_t *mx, int index, int *error);
void error_free(matrix_t *mx, int index);
int check_symbol(const char ch, const char compCh);
int is_vertex(char *buffer);
int is_facet(char *buffer);
void free_data(data *drawing_data);
void setNewScale(data *drawing_data);
void setScaling(const double minValue, const double maxValue, data *data_);
#ifdef __cplusplus
}
#endif

#endif
