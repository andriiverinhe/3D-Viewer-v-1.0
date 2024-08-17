/**
 * @file parser.c
 * @author mitchelk, nenamaxi and dannamer
 * @brief file to process the file and get vertices and polygons
 * @version 0.1
 * @date 2024-03-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "parser.h"

/**
 * @brief Function for allocating memory for polygons
 *
 * @param drawing_data A `date` type structure for storing information about
 * vertices.
 * @return Returns the error code(PARSER_OK or PARSER_FALSE).
 */
int memory_handling(data *drawing_data) {
  int error = PARSER_OK;

  drawing_data->polygons =
      (polygon_t *)calloc(drawing_data->facets_count, sizeof(polygon_t));

  if (!drawing_data->polygons) error = PARSER_FALSE;

  if (!error) {
    unsigned int rows = drawing_data->vertex_count;
    error = create_matrix(rows, ST_SIZE_COORDINATE, &drawing_data->vertexes);
  }

  return error;
}

/**
 * @brief Counts the number of edges in a buffer line and stores the result in a
 * data structure.
 *
 * This static method is used to parse the buffer string and determine the
 * number of vertices and edges. It looks through the buffer line, counts the
 * number of spaces followed by digits, and uses this to determine the number of
 * vertices and edges in the row. Results are saved into a data structure for
 * later use.
 *
 * @param buffer Pointer to a buffer string containing vertex coordinates and
 * edge indices.
 * @param index The index of the current face in the faces array of the data
 * structure.
 * @param drawing_data Pointer to the data structure into which the analysis
 * results are saved.
 * @return int Returns the error code, if any (PARSER_OK if successful).
 * @see check_symbol
 */
static int counting_vf(char *buffer, int index, data *drawing_data) {
  int error = PARSER_OK;

  size_t len = strlen(buffer);
  unsigned int count = 0;

  for (size_t i = 0; i < len; i++) {
    if (check_symbol(buffer[i], ' ') && isdigit(buffer[i + 1])) {
      count++;
    }
  }

  if (count < ST_SIZE_COORDINATE - 1) {
    error = PARSER_FALSE;
  }

  if (!error) {
    drawing_data->polygons[index].count_v = count;
  }

  return error;
}

/**
 * @brief Counts the number of vertices and faces in a file and stores the
 * result in a data structure.
 *
 * This static method opens a file with the given name and parses its contents
 * line by line, counting the number of lines containing information about
 * vertices and faces. Counting results are saved into a data structure for
 * later use.
 *
 * @param filename The name of the file to parse.
 * @param drawing_data Pointer to the data structure into which the calculation
 * results are stored.
 * @return int Returns the error code, if any (PARSER_OK if successful).
 */
static int counting_function(char *filename, data *drawing_data) {
  int error = PARSER_OK;

  char buffer[256];
  memset(buffer, 0, sizeof(buffer));

  FILE *file = fopen(filename, "r");

  while (fgets(buffer, sizeof(buffer), file)) {
    if (is_vertex(buffer)) {
      drawing_data->vertex_count++;
    }

    if (is_facet(buffer)) {
      drawing_data->facets_count++;
    }
  }

  fclose(file);

  if (!drawing_data->vertex_count || !drawing_data->facets_count) {
    error = PARSER_FALSE;
  }

  return error;
}

/**
 * @brief Sets the vertex coordinates in the data structure.
 *
 * This static method reads the vertex coordinates from the buffer and sets them
 * in the vertex matrix at the specified index. It also updates the boundaries
 * along each axis for later use when object display.
 *
 * @param buffer A buffer containing a string containing the vertex coordinates.
 * @param index The index of the vertex in the vertex matrix.
 * @param drawing_data Pointer to a data structure containing information about
 * the vertices.
 * @return int Returns the error code, if any (PARSER_OK if successful).
 */
static int set_vertex_coord(char *buffer, int index, data *drawing_data) {
  int error = PARSER_OK;

  double x = 0;
  double y = 0;
  double z = 0;

  if (sscanf(buffer + 2, "%lf %lf %lf", &x, &y, &z) != ST_SIZE_COORDINATE) {
    error = PARSER_FALSE;
  }

  if (!error) {
    drawing_data->vertexes.matrix[index][X] = x;
    drawing_data->vertexes.matrix[index][Y] = y;
    drawing_data->vertexes.matrix[index][Z] = z;

    if (x < drawing_data->scale[XM]) drawing_data->scale[XM] = x;
    if (x > drawing_data->scale[XP]) drawing_data->scale[XP] = x;
    if (y < drawing_data->scale[YM]) drawing_data->scale[YM] = y;
    if (y > drawing_data->scale[YP]) drawing_data->scale[YP] = y;
    if (z < drawing_data->scale[ZM]) drawing_data->scale[ZM] = z;
    if (z > drawing_data->scale[ZP]) drawing_data->scale[ZP] = z;
  }

  return error;
}

/**
 * @brief Sets the vertices for the face of the object.
 *
 * This static method reads the indices of the vertices forming the face from
 * the buffer and sets them to an array of indices of the face's vertices at the
 * specified index.
 *
 * @param buffer A buffer containing a string with the indices of the face's
 * vertices.
 * @param facet_index The index of a facet in the facet array.
 * @param drawing_data Pointer to a data structure containing edge information.
 * @return int Returns the error code, if any (PARSER_OK if successful).
 */
static int set_vertex_for_facet(char *buffer, int facet_index,
                                data *drawing_data) {
  int error = PARSER_OK;

  unsigned int num = drawing_data->polygons[facet_index].count_v;
  drawing_data->polygons[facet_index].pInds =
      (unsigned int *)calloc(num, sizeof(unsigned int));

  if (!drawing_data->polygons[facet_index].pInds) {
    error = PARSER_FALSE;
  }

  if (!error) {
    char *ptr = buffer;
    int flag = 0;
    int tmp = 0;
    int index = 0;

    while (*ptr && !error) {
      if (check_symbol(*ptr, ' ') && isdigit(*(ptr + 1))) {
        flag = 0;
      }

      if (isdigit(*ptr) && check_symbol(*(ptr - 1), ' ')) {
        flag = 1;
      }

      if (flag) {
        sscanf(ptr, "%u", &tmp);

        if (tmp > (int)drawing_data->vertex_count) {
          error = PARSER_FALSE;
        }

        if (!error) {
          drawing_data->polygons[facet_index].pInds[index] = tmp - 1;
          tmp = 0;
          flag = 0;
          index++;
        }
      }

      ptr++;
    }
  }

  return error;
}

/**
 * @brief Parses lines from a file and populates the data structure with vertex
 * and edge information.
 *
 * This static method reads lines from the specified file, determines the data
 * type (vertex or edge), and accordingly calls the appropriate functions for
 * data processing.
 *
 * @param filename The name of the file to parse.
 * @param drawing_data Pointer to the data structure into which the parsing
 * result will be written.
 * @return int Returns the error code, if any (PARSER_OK if successful).
 */
static int string_parsing(char *filename, data *drawing_data) {
  int error = PARSER_OK;
  char buffer[256];
  memset(buffer, 0, sizeof(buffer));
  int vertex_index = 0;
  int facet_index = 0;

  FILE *file = fopen(filename, "r");

  while (fgets(buffer, sizeof(buffer), file) && !error) {
    if (is_vertex(buffer)) {
      error = set_vertex_coord(buffer, vertex_index, drawing_data);
      vertex_index++;
    }

    if (is_facet(buffer) && !error) {
      error = counting_vf(buffer, facet_index, drawing_data);

      if (!error) {
        error = set_vertex_for_facet(buffer, facet_index, drawing_data);
        facet_index++;
      }
    }
  }

  fclose(file);
  return error;
}

/**
 * @brief Processes the specified file, counting the number of vertices and
 * faces, and then parsing the data.
 *
 * This static method processes the specified file by first counting the number
 * of vertices and faces and then parsing the data from the file and populating
 * the data structure according to that data.
 *
 * @param filename The name of the file to process.
 * @param drawing_data Pointer to the data structure into which the processing
 * result will be written.
 * @return int Returns the error code, if any (PARSER_OK if successful).
 */
static int file_processing(char *filename, data *drawing_data) {
  int error = counting_function(filename, drawing_data);

  if (!error) {
    error = memory_handling(drawing_data);
    if (!error) error = string_parsing(filename, drawing_data);
  }
  return error;
}

/**
 * @brief Parses data from a file and populates the data structure.
 *
 * This method parses data from the specified file and populates the appropriate
 * data structure information about the vertices and faces of the model.
 *
 * @param drawing_data Pointer to the data structure into which the parsing
 * result will be written.
 * @param filename The name of the file to parse.
 * @return int Parsing error code (PARSER_OK if successful).
 */
int parser(data *drawing_data, char *filename) {
  int error = PARSER_OK;

  drawing_data->vertex_count = 0;
  drawing_data->facets_count = 0;
  drawing_data->gif_start = 0;

  setScaling(__DBL_MIN__, __DBL_MAX__, drawing_data);

  drawing_data->polygons = NULL;

  if (access(filename, R_OK) == 0) {
    error = file_processing(filename, drawing_data);

  } else {
    error = PARSER_FALSE;
  }

  return error;
}
