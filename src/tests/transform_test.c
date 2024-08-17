#include "test.h"

// static void print_vertexes(const data *drawing_data){
//   unsigned int num = drawing_data->vertex_count;

//   for(unsigned int i = 0; i < num; i++){
//     for(int j = 0; j < COORD_NUM; j++){
//       printf("%lf ",drawing_data->vertexes.matrix[i][j]);
//     }
//     printf("\n");
//   }
// }

static void vertex_comparing(data drawing_data, data res_data) {
  unsigned int num = drawing_data.vertex_count;

  for (unsigned int i = 0; i < num; i++) {
    for (int j = 0; j < COORD_NUM; j++) {
      ck_assert_double_eq_tol(drawing_data.vertexes.matrix[i][j],
                              res_data.vertexes.matrix[i][j], 0.000001);
    }
  }
}

START_TEST(transform_move_test) {
  data drawing_data = {0};
  data res_data = {0};
  char *filename = "test_source/cube.obj";
  parser(&drawing_data, filename);

  res_data.vertex_count = drawing_data.vertex_count;
  res_data.facets_count = drawing_data.facets_count;

  memory_handling(&res_data);

  Rotation var_x = XM;
  Rotation var_y = YP;
  Rotation var_z = ZM;

  double step = 3.5;

  affineMovingOperation(&drawing_data, step, var_x);
  affineMovingOperation(&drawing_data, step, var_y);
  affineMovingOperation(&drawing_data, step, var_z);

  res_data.vertexes.matrix[0][0] = -2.500000;
  res_data.vertexes.matrix[0][1] = 2.500000;
  res_data.vertexes.matrix[0][2] = -4.500000;

  res_data.vertexes.matrix[1][0] = -2.500000;
  res_data.vertexes.matrix[1][1] = 2.500000;
  res_data.vertexes.matrix[1][2] = -2.500000;

  res_data.vertexes.matrix[2][0] = -4.500000;
  res_data.vertexes.matrix[2][1] = 2.500000;
  res_data.vertexes.matrix[2][2] = -2.500000;

  res_data.vertexes.matrix[3][0] = -4.500000;
  res_data.vertexes.matrix[3][1] = 2.500000;
  res_data.vertexes.matrix[3][2] = -4.500000;

  res_data.vertexes.matrix[4][0] = -2.500000;
  res_data.vertexes.matrix[4][1] = 4.500000;
  res_data.vertexes.matrix[4][2] = -4.500000;

  res_data.vertexes.matrix[5][0] = -2.500000;
  res_data.vertexes.matrix[5][1] = 4.500000;
  res_data.vertexes.matrix[5][2] = -2.500000;

  res_data.vertexes.matrix[6][0] = -4.500000;
  res_data.vertexes.matrix[6][1] = 4.500000;
  res_data.vertexes.matrix[6][2] = -2.500000;

  res_data.vertexes.matrix[7][0] = -4.500000;
  res_data.vertexes.matrix[7][1] = 4.500000;
  res_data.vertexes.matrix[7][2] = -4.500000;

  vertex_comparing(drawing_data, res_data);

  free_data(&drawing_data);
  free_data(&res_data);
}

END_TEST

START_TEST(transform_scale_01_test) {
  data drawing_data = {0};
  data res_data = {0};
  char *filename = "test_source/cube.obj";
  parser(&drawing_data, filename);

  res_data.vertex_count = drawing_data.vertex_count;
  res_data.facets_count = drawing_data.facets_count;

  memory_handling(&res_data);

  Scaling var = INCREASE;

  affineScalingOperation(&drawing_data, 0.5, var);

  res_data.vertexes.matrix[0][0] = 1.500000;
  res_data.vertexes.matrix[0][1] = -1.500000;
  res_data.vertexes.matrix[0][2] = -1.500000;

  res_data.vertexes.matrix[1][0] = 1.500000;
  res_data.vertexes.matrix[1][1] = -1.500000;
  res_data.vertexes.matrix[1][2] = 1.500000;

  res_data.vertexes.matrix[2][0] = -1.500000;
  res_data.vertexes.matrix[2][1] = -1.500000;
  res_data.vertexes.matrix[2][2] = 1.500000;

  res_data.vertexes.matrix[3][0] = -1.500000;
  res_data.vertexes.matrix[3][1] = -1.500000;
  res_data.vertexes.matrix[3][2] = -1.500000;

  res_data.vertexes.matrix[4][0] = 1.500000;
  res_data.vertexes.matrix[4][1] = 1.500000;
  res_data.vertexes.matrix[4][2] = -1.500000;

  res_data.vertexes.matrix[5][0] = 1.500000;
  res_data.vertexes.matrix[5][1] = 1.500000;
  res_data.vertexes.matrix[5][2] = 1.500000;

  res_data.vertexes.matrix[6][0] = -1.500000;
  res_data.vertexes.matrix[6][1] = 1.500000;
  res_data.vertexes.matrix[6][2] = 1.500000;

  res_data.vertexes.matrix[7][0] = -1.500000;
  res_data.vertexes.matrix[7][1] = 1.500000;
  res_data.vertexes.matrix[7][2] = -1.500000;

  vertex_comparing(drawing_data, res_data);

  ck_assert_int_eq(affineScalingOperation(&drawing_data, 0, INCREASE),
                   ERROR_DIV_ZERO);
  ck_assert_int_eq(affineScalingOperation(NULL, 1, INCREASE), ERROR);

  free_data(&drawing_data);
  free_data(&res_data);
}
END_TEST

START_TEST(transform_scale_02_test) {
  data drawing_data = {0};
  data res_data = {0};
  char *filename = "test_source/cube.obj";
  parser(&drawing_data, filename);

  res_data.vertex_count = drawing_data.vertex_count;
  res_data.facets_count = drawing_data.facets_count;

  memory_handling(&res_data);

  Scaling var = DECREASE;

  affineScalingOperation(&drawing_data, 0.5, var);

  res_data.vertexes.matrix[0][0] = 0.500000;
  res_data.vertexes.matrix[0][1] = -0.500000;
  res_data.vertexes.matrix[0][2] = -0.500000;

  res_data.vertexes.matrix[1][0] = 0.500000;
  res_data.vertexes.matrix[1][1] = -0.500000;
  res_data.vertexes.matrix[1][2] = 0.500000;

  res_data.vertexes.matrix[2][0] = -0.500000;
  res_data.vertexes.matrix[2][1] = -0.500000;
  res_data.vertexes.matrix[2][2] = 0.500000;

  res_data.vertexes.matrix[3][0] = -0.500000;
  res_data.vertexes.matrix[3][1] = -0.500000;
  res_data.vertexes.matrix[3][2] = -0.500000;

  res_data.vertexes.matrix[4][0] = 0.500000;
  res_data.vertexes.matrix[4][1] = 0.500000;
  res_data.vertexes.matrix[4][2] = -0.500000;

  res_data.vertexes.matrix[5][0] = 0.500000;
  res_data.vertexes.matrix[5][1] = 0.500000;
  res_data.vertexes.matrix[5][2] = 0.500000;

  res_data.vertexes.matrix[6][0] = -0.500000;
  res_data.vertexes.matrix[6][1] = 0.500000;
  res_data.vertexes.matrix[6][2] = 0.500000;

  res_data.vertexes.matrix[7][0] = -0.500000;
  res_data.vertexes.matrix[7][1] = 0.500000;
  res_data.vertexes.matrix[7][2] = -0.500000;

  vertex_comparing(drawing_data, res_data);

  free_data(&drawing_data);
  free_data(&res_data);
}
END_TEST

START_TEST(transform_scale_03_test) {
  data drawing_data = {0};
  data res_data = {0};
  char *filename = "test_source/cube.obj";
  parser(&drawing_data, filename);

  res_data.vertex_count = drawing_data.vertex_count;
  res_data.facets_count = drawing_data.facets_count;

  memory_handling(&res_data);

  Scaling var = DECREASE;

  affineScalingOperation(&drawing_data, 2.5, var);

  res_data.vertexes.matrix[0][0] = 0.400000;
  res_data.vertexes.matrix[0][1] = -0.400000;
  res_data.vertexes.matrix[0][2] = -0.400000;

  res_data.vertexes.matrix[1][0] = 0.400000;
  res_data.vertexes.matrix[1][1] = -0.400000;
  ;
  res_data.vertexes.matrix[1][2] = 0.400000;
  ;

  res_data.vertexes.matrix[2][0] = -0.400000;
  res_data.vertexes.matrix[2][1] = -0.400000;
  ;
  res_data.vertexes.matrix[2][2] = 0.400000;
  ;

  res_data.vertexes.matrix[3][0] = -0.400000;
  res_data.vertexes.matrix[3][1] = -0.400000;
  res_data.vertexes.matrix[3][2] = -0.400000;

  res_data.vertexes.matrix[4][0] = 0.400000;
  res_data.vertexes.matrix[4][1] = 0.400000;
  res_data.vertexes.matrix[4][2] = -0.400000;

  res_data.vertexes.matrix[5][0] = 0.400000;
  res_data.vertexes.matrix[5][1] = 0.400000;
  res_data.vertexes.matrix[5][2] = 0.400000;

  res_data.vertexes.matrix[6][0] = -0.400000;
  res_data.vertexes.matrix[6][1] = 0.400000;
  res_data.vertexes.matrix[6][2] = 0.400000;

  res_data.vertexes.matrix[7][0] = -0.400000;
  res_data.vertexes.matrix[7][1] = 0.400000;
  res_data.vertexes.matrix[7][2] = -0.400000;

  vertex_comparing(drawing_data, res_data);

  free_data(&drawing_data);
  free_data(&res_data);
}
END_TEST

START_TEST(transform_scale_04_test) {
  data drawing_data = {0};
  data res_data = {0};
  char *filename = "test_source/cube.obj";
  parser(&drawing_data, filename);

  res_data.vertex_count = drawing_data.vertex_count;
  res_data.facets_count = drawing_data.facets_count;

  memory_handling(&res_data);

  Scaling var = DECREASE;

  affineScalingOperation(&drawing_data, 1.0, var);

  res_data.vertexes.matrix[0][0] = 0.0100000;
  res_data.vertexes.matrix[0][1] = -0.0100000;
  res_data.vertexes.matrix[0][2] = -0.0100000;

  res_data.vertexes.matrix[1][0] = 0.0100000;
  res_data.vertexes.matrix[1][1] = -0.0100000;
  res_data.vertexes.matrix[1][2] = 0.0100000;

  res_data.vertexes.matrix[2][0] = -0.0100000;
  res_data.vertexes.matrix[2][1] = -0.0100000;
  res_data.vertexes.matrix[2][2] = 0.0100000;

  res_data.vertexes.matrix[3][0] = -0.0100000;
  res_data.vertexes.matrix[3][1] = -0.0100000;
  res_data.vertexes.matrix[3][2] = -0.0100000;

  res_data.vertexes.matrix[4][0] = 0.0100000;
  res_data.vertexes.matrix[4][1] = 0.0100000;
  res_data.vertexes.matrix[4][2] = -0.0100000;

  res_data.vertexes.matrix[5][0] = 0.0100000;
  res_data.vertexes.matrix[5][1] = 0.0100000;
  res_data.vertexes.matrix[5][2] = 0.0100000;

  res_data.vertexes.matrix[6][0] = -0.0100000;
  res_data.vertexes.matrix[6][1] = 0.0100000;
  res_data.vertexes.matrix[6][2] = 0.0100000;

  res_data.vertexes.matrix[7][0] = -0.0100000;
  res_data.vertexes.matrix[7][1] = 0.0100000;
  res_data.vertexes.matrix[7][2] = -0.0100000;

  vertex_comparing(drawing_data, res_data);

  free_data(&drawing_data);
  free_data(&res_data);
}
END_TEST

START_TEST(transform_rotation_01_test) {
  data drawing_data = {0};
  data res_data = {0};
  char *filename = "test_source/cube.obj";
  parser(&drawing_data, filename);

  res_data.vertex_count = drawing_data.vertex_count;
  res_data.facets_count = drawing_data.facets_count;
  memory_handling(&res_data);

  double angle = 30;
  Rotation var = ZP;
  affineRotationOperation(&drawing_data, angle, var);

  res_data.vertexes.matrix[0][0] = -0.833780;
  res_data.vertexes.matrix[0][1] = -1.142283;
  res_data.vertexes.matrix[0][2] = -1.000000;

  res_data.vertexes.matrix[1][0] = -0.833780;
  res_data.vertexes.matrix[1][1] = -1.142283;
  res_data.vertexes.matrix[1][2] = 1.000000;

  res_data.vertexes.matrix[2][0] = -1.142283;
  res_data.vertexes.matrix[2][1] = 0.833780;
  res_data.vertexes.matrix[2][2] = 1.000000;

  res_data.vertexes.matrix[3][0] = -1.142283;
  res_data.vertexes.matrix[3][1] = 0.833780;
  res_data.vertexes.matrix[3][2] = -1.000000;

  res_data.vertexes.matrix[4][0] = 1.142283;
  res_data.vertexes.matrix[4][1] = -0.833780;
  res_data.vertexes.matrix[4][2] = -1.000000;

  res_data.vertexes.matrix[5][0] = 1.142283;
  res_data.vertexes.matrix[5][1] = -0.833780;
  res_data.vertexes.matrix[5][2] = 1.000000;

  res_data.vertexes.matrix[6][0] = 0.833780;
  res_data.vertexes.matrix[6][1] = 1.142283;
  res_data.vertexes.matrix[6][2] = 1.000000;

  res_data.vertexes.matrix[7][0] = 0.833780;
  res_data.vertexes.matrix[7][1] = 1.142283;
  res_data.vertexes.matrix[7][2] = -1.000000;

  vertex_comparing(drawing_data, res_data);

  ck_assert_int_eq(affineRotationOperation(NULL, 1, XM), ERROR);

  free_data(&drawing_data);
  free_data(&res_data);
}

START_TEST(transform_rotation_02_test) {
  data drawing_data = {0};
  data res_data = {0};
  char *filename = "test_source/cube.obj";
  parser(&drawing_data, filename);

  res_data.vertex_count = drawing_data.vertex_count;
  res_data.facets_count = drawing_data.facets_count;
  memory_handling(&res_data);

  double angle = 45;
  Rotation var = YP;
  affineRotationOperation(&drawing_data, angle, var);

  res_data.vertexes.matrix[0][0] = -0.325582;
  res_data.vertexes.matrix[0][1] = -1.000000;
  res_data.vertexes.matrix[0][2] = -1.376226;

  res_data.vertexes.matrix[1][0] = 1.376226;
  res_data.vertexes.matrix[1][1] = -1.000000;
  res_data.vertexes.matrix[1][2] = -0.325582;

  res_data.vertexes.matrix[2][0] = 0.325582;
  res_data.vertexes.matrix[2][1] = -1.000000;
  res_data.vertexes.matrix[2][2] = 1.376226;

  res_data.vertexes.matrix[3][0] = -1.376226;
  res_data.vertexes.matrix[3][1] = -1.000000;
  res_data.vertexes.matrix[3][2] = 0.325582;

  res_data.vertexes.matrix[4][0] = -0.325582;
  res_data.vertexes.matrix[4][1] = 1.000000;
  res_data.vertexes.matrix[4][2] = -1.376226;

  res_data.vertexes.matrix[5][0] = 1.376226;
  res_data.vertexes.matrix[5][1] = 1.000000;
  res_data.vertexes.matrix[5][2] = -0.325582;

  res_data.vertexes.matrix[6][0] = 0.325582;
  res_data.vertexes.matrix[6][1] = 1.000000;
  res_data.vertexes.matrix[6][2] = 1.376226;

  res_data.vertexes.matrix[7][0] = -1.376226;
  res_data.vertexes.matrix[7][1] = 1.000000;
  res_data.vertexes.matrix[7][2] = 0.325582;

  vertex_comparing(drawing_data, res_data);

  free_data(&drawing_data);
  free_data(&res_data);
}

START_TEST(transform_rotation_03_test) {
  data drawing_data = {0};
  data res_data = {0};
  char *filename = "test_source/cube.obj";
  parser(&drawing_data, filename);

  res_data.vertex_count = drawing_data.vertex_count;
  res_data.facets_count = drawing_data.facets_count;
  memory_handling(&res_data);

  double angle = 45;
  Rotation var = XM;
  affineRotationOperation(&drawing_data, angle, var);

  res_data.vertexes.matrix[0][0] = 1.000000;
  res_data.vertexes.matrix[0][1] = -1.376226;
  res_data.vertexes.matrix[0][2] = 0.325582;

  res_data.vertexes.matrix[1][0] = 1.000000;
  res_data.vertexes.matrix[1][1] = 0.325582;
  res_data.vertexes.matrix[1][2] = 1.376226;

  res_data.vertexes.matrix[2][0] = -1.000000;
  res_data.vertexes.matrix[2][1] = 0.325582;
  res_data.vertexes.matrix[2][2] = 1.376226;

  res_data.vertexes.matrix[3][0] = -1.000000;
  res_data.vertexes.matrix[3][1] = -1.376226;
  res_data.vertexes.matrix[3][2] = 0.325582;

  res_data.vertexes.matrix[4][0] = 1.000000;
  res_data.vertexes.matrix[4][1] = -0.325582;
  res_data.vertexes.matrix[4][2] = -1.376226;

  res_data.vertexes.matrix[5][0] = 1.000000;
  res_data.vertexes.matrix[5][1] = 1.376226;
  res_data.vertexes.matrix[5][2] = -0.325582;

  res_data.vertexes.matrix[6][0] = -1.000000;
  res_data.vertexes.matrix[6][1] = 1.376226;
  res_data.vertexes.matrix[6][2] = -0.325582;

  res_data.vertexes.matrix[7][0] = -1.000000;
  res_data.vertexes.matrix[7][1] = -0.325582;
  res_data.vertexes.matrix[7][2] = -1.376226;

  vertex_comparing(drawing_data, res_data);

  free_data(&drawing_data);
  free_data(&res_data);
}

START_TEST(transform_move_to_center_test) {
  data drawing_data = {0};
  data res_data = {0};
  char *filename = "test_source/transform_test.obj";
  parser(&drawing_data, filename);

  res_data.vertex_count = drawing_data.vertex_count;
  res_data.facets_count = drawing_data.facets_count;

  memory_handling(&res_data);

  setFigureToCenter(&drawing_data);

  res_data.vertexes.matrix[0][0] = 29.0000000;
  res_data.vertexes.matrix[0][1] = -29.0000000;
  res_data.vertexes.matrix[0][2] = -123.00000;

  res_data.vertexes.matrix[1][0] = 29.0000000;
  res_data.vertexes.matrix[1][1] = -29.0000000;
  res_data.vertexes.matrix[1][2] = 123.00000;

  res_data.vertexes.matrix[2][0] = -29.0000000;
  res_data.vertexes.matrix[2][1] = -29.0000000;
  res_data.vertexes.matrix[2][2] = 123.00000;

  res_data.vertexes.matrix[3][0] = -29.0000000;
  res_data.vertexes.matrix[3][1] = -29.0000000;
  res_data.vertexes.matrix[3][2] = -123.00000;

  res_data.vertexes.matrix[4][0] = 29.0000000;
  res_data.vertexes.matrix[4][1] = 29.0000000;
  res_data.vertexes.matrix[4][2] = -123.00000;

  res_data.vertexes.matrix[5][0] = 29.0000000;
  res_data.vertexes.matrix[5][1] = 29.0000000;
  res_data.vertexes.matrix[5][2] = 123.00000;

  res_data.vertexes.matrix[6][0] = -29.0000000;
  res_data.vertexes.matrix[6][1] = 29.0000000;
  res_data.vertexes.matrix[6][2] = 123.00000;

  res_data.vertexes.matrix[7][0] = -29.0000000;
  res_data.vertexes.matrix[7][1] = 29.0000000;
  res_data.vertexes.matrix[7][2] = -123.00000;

  vertex_comparing(drawing_data, res_data);
  ck_assert_int_eq(affineMovingOperation(NULL, 1, XM), ERROR);

  free_data(&drawing_data);
  free_data(&res_data);
}
END_TEST

Suite *suite_transform(void) {
  Suite *s = suite_create("suite_transform");
  TCase *tc = tcase_create("case_transform");

  tcase_add_test(tc, transform_move_test);
  tcase_add_test(tc, transform_scale_01_test);
  tcase_add_test(tc, transform_scale_02_test);
  tcase_add_test(tc, transform_scale_03_test);
  tcase_add_test(tc, transform_scale_04_test);
  tcase_add_test(tc, transform_rotation_01_test);
  tcase_add_test(tc, transform_rotation_02_test);
  tcase_add_test(tc, transform_rotation_03_test);
  tcase_add_test(tc, transform_move_to_center_test);

  suite_add_tcase(s, tc);
  return s;
}