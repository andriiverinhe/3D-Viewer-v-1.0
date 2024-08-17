/**
 * @file transform.c
 * @author mitchelk, nenamaxi and dannamer
 * @brief affine operations implementation file
 * @version 0.1
 * @date 2024-03-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "transform.h"

/// @brief Function to determine the center of a figure
/// @param drawing_data A `date` type structure for storing information about
/// vertices.
/// @param center An array of three points x,y,z.
static void findCenterFigure(data *drawing_data, double *center) {
  if (center && drawing_data) {
    size_t count = drawing_data->vertex_count;

    for (size_t i = 0; i < count; i++) {
      center[X] += drawing_data->vertexes.matrix[i][X];
      center[Y] += drawing_data->vertexes.matrix[i][Y];
      center[Z] += drawing_data->vertexes.matrix[i][Z];
    }

    for (size_t i = 0; i < (size_t)ST_SIZE_COORDINATE; i++) center[i] /= count;
  }
}
/// @brief Function to move a figure to the center or from the center to its
/// original position.
/// @param drawing_data A date type structure for storing information about
/// vertices.
/// @param posCenterFigure An array of three center points x,y,z.
/// @param pos Argument of type `MOVING_TO_POSITION` to define relocation.
static void movingToPosition(data **drawing_data, const double *posCenterFigure,
                             const MOVING_TO_POSITION pos) {
  if ((*drawing_data) && posCenterFigure) {
    for (size_t i = 0; i < (*drawing_data)->vertex_count; i++)
      for (size_t j = 0; j < (size_t)ST_SIZE_COORDINATE; j++) {
        if (pos == CENTER)
          (*drawing_data)->vertexes.matrix[i][j] -= posCenterFigure[j];
        if (pos == ORIGINAL)
          (*drawing_data)->vertexes.matrix[i][j] += posCenterFigure[j];
      }
  }
}

/**
 * @brief
 *
 * @param drawing_data Helper function for performing affine rotation operation
 * around the Z
 * @param valueCos Cosine of rotation angle.
 * @param valueSin Sine of rotation angle.
 */
void rotation_by_ox(data *drawing_data, const double valueCos,
                    const double valueSin) {
  if (drawing_data) {
    for (int i = 0; i < (int)drawing_data->vertex_count; i++) {
      double tmp_y = drawing_data->vertexes.matrix[i][Y];
      double tmp_z = drawing_data->vertexes.matrix[i][Z];

      drawing_data->vertexes.matrix[i][Y] = valueCos * tmp_y - valueSin * tmp_z;
      drawing_data->vertexes.matrix[i][Z] = valueSin * tmp_y + valueCos * tmp_z;
    }
  }
}

/**
 * @brief
 *
 * @param drawing_data Helper function for performing affine rotation operation
 * around the Z
 * @param valueCos Cosine of rotation angle.
 * @param valueSin Sine of rotation angle.
 */
void rotation_by_oy(data *drawing_data, const double valueCos,
                    const double valueSin) {
  if (drawing_data) {
    for (int i = 0; i < (int)drawing_data->vertex_count; i++) {
      double tmp_x = drawing_data->vertexes.matrix[i][X];
      double tmp_z = drawing_data->vertexes.matrix[i][Z];

      drawing_data->vertexes.matrix[i][X] = valueCos * tmp_x + valueSin * tmp_z;
      drawing_data->vertexes.matrix[i][Z] =
          -valueSin * tmp_x + valueCos * tmp_z;
    }
  }
}

/**
 * @brief
 *
 * @param drawing_data Helper function for performing affine rotation operation
 * around the Z
 * @param valueCos Cosine of rotation angle.
 * @param valueSin Sine of rotation angle.
 */
void rotation_by_oz(data *drawing_data, const double valueCos,
                    const double valueSin) {
  if (drawing_data) {
    for (int i = 0; i < (int)drawing_data->vertex_count; i++) {
      double tmp_x = drawing_data->vertexes.matrix[i][X];
      double tmp_y = drawing_data->vertexes.matrix[i][Y];

      drawing_data->vertexes.matrix[i][X] = valueCos * tmp_x - valueSin * tmp_y;
      drawing_data->vertexes.matrix[i][Y] = valueSin * tmp_x + valueCos * tmp_y;
    }
  }
}
/// @brief Function to get sign.
/// @param rotation_ Argument of type `Rotation` to determine the sign depending
/// on the direction.
/// @return sign.
int getSign(const Rotation rotation_) {
  int sign = 1;
  if (rotation_ == XM || rotation_ == YM || rotation_ == ZM) sign = -1;
  return sign;
}

/// @brief Affine rotation operation.
/// @param data A `date` type structure for storing information about vertices.
/// @param angle Angle of rotation.
/// @param rotation_ Argument of type "Rotation" to determine the direction.
/// @return Returns the result of the operation: `ERROR` or `OK`.
/// @see findCenterFigure
/// @see movingToPosition
/// @see getSign
/// @see rotation_by_ox
/// @see rotation_by_oy
/// @see rotation_by_oz
/// @see movingToPosition
int affineRotationOperation(data *data, const double angle,
                            const Rotation rotation_) {
  if (!data) return ERROR;

  double center[ST_SIZE_COORDINATE] = {0};

  findCenterFigure(data, center);
  movingToPosition(&data, center, CENTER);

  int sign = getSign(rotation_);

  const double valueCos = cos(sign * angle);
  const double valueSin = sin(sign * angle);

  if (rotation_ == XM || rotation_ == XP)
    rotation_by_ox(data, valueCos, valueSin);
  if (rotation_ == YM || rotation_ == YP)
    rotation_by_oy(data, valueCos, valueSin);
  if (rotation_ == ZM || rotation_ == ZP)
    rotation_by_oz(data, valueCos, valueSin);

  movingToPosition(&data, center, ORIGINAL);
  return OK;
}
/// @brief Affine moving operation.
/// @param data A `date` type structure for storing information about vertices.
/// @param step Step of moving.
/// @param rotation_ Argument of type "Rotation" to determine the direction.
/// @return Returns the result of the operation: `ERROR` or `OK`.
/// @see getSign
int affineMovingOperation(data *data, const double step,
                          const Rotation rotation_) {
  if (!data) return ERROR;
  double modifier_step = step * getSign(rotation_);

  for (int i = 0; i < (int)data->vertex_count; i++) {
    INDEX_COORDINATE coord = X;
    if (rotation_ == YM || rotation_ == YP) coord = Y;
    if (rotation_ == ZM || rotation_ == ZP) coord = Z;

    data->vertexes.matrix[i][coord] =
        data->vertexes.matrix[i][coord] + modifier_step;
  }

  return OK;
}

/// @brief Helper function for performing the scaling operation.
/// @param drawing_data  A `date` type structure for storing information about
/// vertices.
/// @param factor Scaling factor.
void scaling(data *drawing_data, double factor) {
  if (drawing_data) {
    int count = (int)drawing_data->vertex_count;

    for (int i = 0; i < count; i++) {
      drawing_data->vertexes.matrix[i][X] =
          drawing_data->vertexes.matrix[i][X] * factor;
      drawing_data->vertexes.matrix[i][Y] =
          drawing_data->vertexes.matrix[i][Y] * factor;
      drawing_data->vertexes.matrix[i][Z] =
          drawing_data->vertexes.matrix[i][Z] * factor;
    }
  }
}

/// @brief Affine scaling operation.
/// @param data  A `date` type structure for storing information about vertices.
/// @param coefficient Scaling factor.
/// @param scaling_ Argument of type `Scaling` to define scaling(`INCREASE` or
/// `DECREASE`).
/// @return Returns the result of the operation: `ERROR` or `OK`.
/// @see scaling
int affineScalingOperation(data *data, const double coefficient,
                           const Scaling scaling_) {
  if (!data) return ERROR;
  if (coefficient == 0) return ERROR_DIV_ZERO;

  double modifier_coef = 1;

  if (scaling_ == INCREASE) modifier_coef += coefficient;
  if (scaling_ == DECREASE) {
    if (coefficient > modifier_coef)
      modifier_coef /= coefficient;
    else if (coefficient < modifier_coef)
      modifier_coef -= coefficient;
    else
      modifier_coef -= ST_AROUND_VALUE_ONE;
  }

  scaling(data, modifier_coef);
  return OK;
}

/// @brief Sets the figure to the center.
/// @param glData A `date` type structure for storing information about
/// vertices.
/// @see findCenterFigure
/// @see movingToPosition
/// @see setNewScale
void setFigureToCenter(data *glData) {
  if (glData) {
    double center[ST_SIZE_COORDINATE] = {0};
    findCenterFigure(glData, center);
    movingToPosition(&glData, center, CENTER);
    setNewScale(glData);
  }
}