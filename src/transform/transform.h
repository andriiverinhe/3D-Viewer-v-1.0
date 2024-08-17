/**
 * @file transform.h
 * @author mitchelk, nenamaxi and dannamer
 * @brief Header file of affine operations
 * @version 0.1
 * @date 2024-03-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef TRANSFORM_H
#define TRANSFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

#include "../parser/parser.h"

/// @brief Operation status macro.
#define OK 1
/// @brief Operation status macro.
#define ERROR 0
/// @brief Operation status macro.
#define ERROR_DIV_ZERO -1
/// @brief Macro for minimal scaling.
#define ST_MIN_SCALING 0.1
/// @brief Macro for around value one.
#define ST_AROUND_VALUE_ONE 0.99

/// @brief Enumeration structure for affine scaling operation.
typedef enum {
  INCREASE = 0, /**< Field indicating magnification */
  DECREASE = 1  /**< field indicating reduction */
} Scaling;

/// @brief Enumeration structure for operation moving
typedef enum {
  CENTER,  /**< Move to center */
  ORIGINAL /**< Move to start position */
} MOVING_TO_POSITION;

void scaling(data *drawing_data, double factor);
void rotation_by_ox(data *drawing_data, const double valueCos,
                    const double valueSin);
void rotation_by_oy(data *drawing_data, const double valueCos,
                    const double valueSin);
void rotation_by_oz(data *drawing_data, const double valueCos,
                    const double valueSin);
int getSign(const Rotation rotation_);
void setFigureToCenter(data *glData);

int affineScalingOperation(data *data, const double coefficient,
                           const Scaling scaling);
int affineRotationOperation(data *data, const double angle,
                            const Rotation rotation_);
int affineMovingOperation(data *data, const double step,
                          const Rotation rotation_);

#ifdef __cplusplus
}
#endif

#endif
