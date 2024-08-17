#include "test.h"

static void data_comparing(data drawing_data, data res_data) {
  for (unsigned int i = 0; i < drawing_data.vertex_count; i++) {
    for (int j = 0; j < COORD_NUM; j++) {
      ck_assert_double_eq(res_data.vertexes.matrix[i][j],
                          drawing_data.vertexes.matrix[i][j]);
    }
  }

  for (unsigned int i = 0; i < drawing_data.facets_count; i++) {
    ck_assert_uint_eq(res_data.polygons[i].count_v,
                      drawing_data.polygons[i].count_v);

    for (unsigned int j = 0; j < drawing_data.polygons[i].count_v; j++) {
      ck_assert_uint_eq(drawing_data.polygons[i].pInds[j],
                        res_data.polygons[i].pInds[j] - 1);
    }

    for (int i = 0; i < SCALE_NUM; i++) {
      ck_assert_double_eq(res_data.scale[i], drawing_data.scale[i]);
    }
  }
}

START_TEST(parser_test_01) {
  data drawing_data = {0};
  data res_data = {0};

  char *filename = "test_source/cube.obj";

  parser(&drawing_data, filename);

  ck_assert_uint_eq(drawing_data.vertex_count, 8);

  ck_assert_uint_eq(drawing_data.facets_count, 12);

  res_data.vertex_count = drawing_data.vertex_count;
  res_data.facets_count = drawing_data.facets_count;

  memory_handling(&res_data);

  res_data.vertexes.matrix[0][0] = 1.000000;
  res_data.vertexes.matrix[0][1] = -1.000000;
  res_data.vertexes.matrix[0][2] = -1.000000;

  res_data.vertexes.matrix[1][0] = 1.000000;
  res_data.vertexes.matrix[1][1] = -1.000000;
  res_data.vertexes.matrix[1][2] = 1.000000;

  res_data.vertexes.matrix[2][0] = -1.000000;
  res_data.vertexes.matrix[2][1] = -1.000000;
  res_data.vertexes.matrix[2][2] = 1.000000;

  res_data.vertexes.matrix[3][0] = -1.000000;
  res_data.vertexes.matrix[3][1] = -1.000000;
  res_data.vertexes.matrix[3][2] = -1.000000;

  res_data.vertexes.matrix[4][0] = 1.000000;
  res_data.vertexes.matrix[4][1] = 1.000000;
  res_data.vertexes.matrix[4][2] = -1.000000;

  res_data.vertexes.matrix[5][0] = 1.000000;
  res_data.vertexes.matrix[5][1] = 1.000000;
  res_data.vertexes.matrix[5][2] = 1.000000;

  res_data.vertexes.matrix[6][0] = -1.000000;
  res_data.vertexes.matrix[6][1] = 1.000000;
  res_data.vertexes.matrix[6][2] = 1.000000;

  res_data.vertexes.matrix[7][0] = -1.000000;
  res_data.vertexes.matrix[7][1] = 1.000000;
  res_data.vertexes.matrix[7][2] = -1.000000;

  for (unsigned int i = 0; i < drawing_data.facets_count; i++) {
    res_data.polygons[i].count_v = 3;
  }

  for (unsigned int i = 0; i < drawing_data.facets_count; i++) {
    res_data.polygons[i].pInds = (unsigned int *)calloc(
        res_data.polygons[i].count_v, sizeof(unsigned int));
  }

  res_data.polygons[0].pInds[0] = 2;
  res_data.polygons[0].pInds[1] = 3;
  res_data.polygons[0].pInds[2] = 4;

  res_data.polygons[1].pInds[0] = 8;
  res_data.polygons[1].pInds[1] = 7;
  res_data.polygons[1].pInds[2] = 6;

  res_data.polygons[2].pInds[0] = 5;
  res_data.polygons[2].pInds[1] = 6;
  res_data.polygons[2].pInds[2] = 2;

  res_data.polygons[3].pInds[0] = 6;
  res_data.polygons[3].pInds[1] = 7;
  res_data.polygons[3].pInds[2] = 3;

  res_data.polygons[4].pInds[0] = 3;
  res_data.polygons[4].pInds[1] = 7;
  res_data.polygons[4].pInds[2] = 8;

  res_data.polygons[5].pInds[0] = 1;
  res_data.polygons[5].pInds[1] = 4;
  res_data.polygons[5].pInds[2] = 8;

  res_data.polygons[6].pInds[0] = 1;
  res_data.polygons[6].pInds[1] = 2;
  res_data.polygons[6].pInds[2] = 4;

  res_data.polygons[7].pInds[0] = 5;
  res_data.polygons[7].pInds[1] = 8;
  res_data.polygons[7].pInds[2] = 6;

  res_data.polygons[8].pInds[0] = 1;
  res_data.polygons[8].pInds[1] = 5;
  res_data.polygons[8].pInds[2] = 2;

  res_data.polygons[9].pInds[0] = 2;
  res_data.polygons[9].pInds[1] = 6;
  res_data.polygons[9].pInds[2] = 3;

  res_data.polygons[10].pInds[0] = 4;
  res_data.polygons[10].pInds[1] = 3;
  res_data.polygons[10].pInds[2] = 8;

  res_data.polygons[11].pInds[0] = 5;
  res_data.polygons[11].pInds[1] = 1;
  res_data.polygons[11].pInds[2] = 8;

  setNewScale(&res_data);

  data_comparing(drawing_data, res_data);

  free_data(&drawing_data);
  free_data(&res_data);
}

END_TEST

START_TEST(parser_test_02) {
  data drawing_data = {0};
  data res_data = {0};

  char *filename = "test_source/parser_test.obj";
  parser(&drawing_data, filename);

  ck_assert_uint_eq(drawing_data.vertex_count, 16);

  ck_assert_uint_eq(drawing_data.facets_count, 14);

  res_data.vertex_count = drawing_data.vertex_count;
  res_data.facets_count = drawing_data.facets_count;

  memory_handling(&res_data);

  res_data.vertexes.matrix[0][0] = 0.5;
  res_data.vertexes.matrix[0][1] = -1.5;
  res_data.vertexes.matrix[0][2] = 0.7071068;

  res_data.vertexes.matrix[1][0] = 1.5;
  res_data.vertexes.matrix[1][1] = -0.5;
  res_data.vertexes.matrix[1][2] = -0.7071068;

  res_data.vertexes.matrix[2][0] = 1.2071068;
  res_data.vertexes.matrix[2][1] = 1.2071068;
  res_data.vertexes.matrix[2][2] = 0.2928932;

  res_data.vertexes.matrix[3][0] = 0.2071068;
  res_data.vertexes.matrix[3][1] = 0.2071068;
  res_data.vertexes.matrix[3][2] = 1.7071068;

  res_data.vertexes.matrix[4][0] = -1.2071068;
  res_data.vertexes.matrix[4][1] = -1.2071068;
  res_data.vertexes.matrix[4][2] = -0.2928932;

  res_data.vertexes.matrix[5][0] = -0.2071068;
  res_data.vertexes.matrix[5][1] = -0.2071068;
  res_data.vertexes.matrix[5][2] = -1.7071068;

  res_data.vertexes.matrix[6][0] = -0.5;
  res_data.vertexes.matrix[6][1] = 1.5;
  res_data.vertexes.matrix[6][2] = -0.7071068;

  res_data.vertexes.matrix[7][0] = -1.5;
  res_data.vertexes.matrix[7][1] = 0.5;
  res_data.vertexes.matrix[7][2] = 0.7071068;

  res_data.vertexes.matrix[8][0] = 0.5;
  res_data.vertexes.matrix[8][1] = -1.5;
  res_data.vertexes.matrix[8][2] = 0.7071068;

  res_data.vertexes.matrix[9][0] = 1.5;
  res_data.vertexes.matrix[9][1] = -0.5;
  res_data.vertexes.matrix[9][2] = -0.7071068;

  res_data.vertexes.matrix[10][0] = 1.2071068;
  res_data.vertexes.matrix[10][1] = 1.2071068;
  res_data.vertexes.matrix[10][2] = 0.2928932;

  res_data.vertexes.matrix[11][0] = 0.2071068;
  res_data.vertexes.matrix[11][1] = 0.2071068;
  res_data.vertexes.matrix[11][2] = 1.7071068;

  res_data.vertexes.matrix[12][0] = -1.2071068;
  res_data.vertexes.matrix[12][1] = -1.2071068;
  res_data.vertexes.matrix[12][2] = -0.2928932;

  res_data.vertexes.matrix[13][0] = -0.2071068;
  res_data.vertexes.matrix[13][1] = -0.2071068;
  res_data.vertexes.matrix[13][2] = -1.7071068;

  res_data.vertexes.matrix[14][0] = -0.5;
  res_data.vertexes.matrix[14][1] = 1.5;
  res_data.vertexes.matrix[14][2] = -0.7071068;

  res_data.vertexes.matrix[15][0] = -1.5;
  res_data.vertexes.matrix[15][1] = 0.5;
  res_data.vertexes.matrix[15][2] = 0.7071068;

  for (unsigned int i = 0; i < drawing_data.facets_count; i++) {
    res_data.polygons[i].count_v = 3;
  }

  res_data.polygons[1].count_v = 4;
  res_data.polygons[2].count_v = 5;
  res_data.polygons[5].count_v = 9;
  res_data.polygons[8].count_v = 4;
  res_data.polygons[9].count_v = 11;
  res_data.polygons[12].count_v = 4;
  res_data.polygons[13].count_v = 15;

  for (unsigned int i = 0; i < drawing_data.facets_count; i++) {
    res_data.polygons[i].pInds = (unsigned int *)calloc(
        res_data.polygons[i].count_v, sizeof(unsigned int));
  }

  res_data.polygons[0].pInds[0] = 1;
  res_data.polygons[0].pInds[1] = 2;
  res_data.polygons[0].pInds[2] = 5;

  res_data.polygons[1].pInds[0] = 2;
  res_data.polygons[1].pInds[1] = 3;
  res_data.polygons[1].pInds[2] = 4;
  res_data.polygons[1].pInds[3] = 5;

  res_data.polygons[2].pInds[0] = 5;
  res_data.polygons[2].pInds[1] = 6;
  res_data.polygons[2].pInds[2] = 8;
  res_data.polygons[2].pInds[3] = 1;
  res_data.polygons[2].pInds[4] = 2;

  res_data.polygons[3].pInds[0] = 3;
  res_data.polygons[3].pInds[1] = 6;
  res_data.polygons[3].pInds[2] = 7;

  res_data.polygons[4].pInds[0] = 5;
  res_data.polygons[4].pInds[1] = 6;
  res_data.polygons[4].pInds[2] = 7;

  res_data.polygons[5].pInds[0] = 2;
  res_data.polygons[5].pInds[1] = 6;
  res_data.polygons[5].pInds[2] = 7;
  res_data.polygons[5].pInds[3] = 8;
  res_data.polygons[5].pInds[4] = 1;
  res_data.polygons[5].pInds[5] = 11;
  res_data.polygons[5].pInds[6] = 12;
  res_data.polygons[5].pInds[7] = 13;
  res_data.polygons[5].pInds[8] = 14;

  res_data.polygons[6].pInds[0] = 3;
  res_data.polygons[6].pInds[1] = 4;
  res_data.polygons[6].pInds[2] = 8;

  res_data.polygons[7].pInds[0] = 11;
  res_data.polygons[7].pInds[1] = 12;
  res_data.polygons[7].pInds[2] = 15;

  res_data.polygons[8].pInds[0] = 12;
  res_data.polygons[8].pInds[1] = 13;
  res_data.polygons[8].pInds[2] = 14;
  res_data.polygons[8].pInds[3] = 15;

  res_data.polygons[9].pInds[0] = 15;
  res_data.polygons[9].pInds[1] = 16;
  res_data.polygons[9].pInds[2] = 10;
  res_data.polygons[9].pInds[3] = 11;
  res_data.polygons[9].pInds[4] = 12;
  res_data.polygons[9].pInds[5] = 1;
  res_data.polygons[9].pInds[6] = 2;
  res_data.polygons[9].pInds[7] = 3;
  res_data.polygons[9].pInds[8] = 4;
  res_data.polygons[9].pInds[9] = 5;
  res_data.polygons[9].pInds[10] = 6;

  res_data.polygons[10].pInds[0] = 13;
  res_data.polygons[10].pInds[1] = 16;
  res_data.polygons[10].pInds[2] = 12;

  res_data.polygons[11].pInds[0] = 15;
  res_data.polygons[11].pInds[1] = 16;
  res_data.polygons[11].pInds[2] = 13;

  res_data.polygons[12].pInds[0] = 12;
  res_data.polygons[12].pInds[1] = 13;
  res_data.polygons[12].pInds[2] = 15;
  res_data.polygons[12].pInds[3] = 11;

  res_data.polygons[13].pInds[0] = 1;
  res_data.polygons[13].pInds[1] = 2;
  res_data.polygons[13].pInds[2] = 3;
  res_data.polygons[13].pInds[3] = 4;
  res_data.polygons[13].pInds[4] = 5;
  res_data.polygons[13].pInds[5] = 6;
  res_data.polygons[13].pInds[6] = 7;
  res_data.polygons[13].pInds[7] = 8;
  res_data.polygons[13].pInds[8] = 9;
  res_data.polygons[13].pInds[9] = 10;
  res_data.polygons[13].pInds[10] = 11;
  res_data.polygons[13].pInds[11] = 12;
  res_data.polygons[13].pInds[12] = 13;
  res_data.polygons[13].pInds[13] = 14;
  res_data.polygons[13].pInds[14] = 15;

  setNewScale(&res_data);

  data_comparing(drawing_data, res_data);

  free_data(&drawing_data);
  free_data(&res_data);
}
END_TEST

START_TEST(parser_test_empty) {
  char *filename = "test_source/empty.obj";
  data drawing_data;
  ck_assert_int_eq(parser(&drawing_data, filename), PARSER_FALSE);
}
END_TEST

START_TEST(parser_test_wrong_vertcount) {
  char *filename = "test_source/wrong_vert_count.obj";
  data drawing_data;
  ck_assert_int_eq(parser(&drawing_data, filename), PARSER_FALSE);
  free_data(&drawing_data);
}
END_TEST

START_TEST(parser_test_wrong_vf) {
  char *filename = "test_source/wrong_vf.obj";
  data drawing_data;
  ck_assert_int_eq(parser(&drawing_data, filename), PARSER_FALSE);
  free_data(&drawing_data);

  data *scale_ptr = NULL;
  setNewScale(scale_ptr);

  data *data_ptr = NULL;
  free_data(data_ptr);

  data *scaling_ptr = NULL;
  setScaling(55, 55, scaling_ptr);
}
END_TEST

START_TEST(parser_test_wrong_index) {
  char *filename = "test_source/wrong_index.obj";
  data drawing_data;
  ck_assert_int_eq(parser(&drawing_data, filename), PARSER_FALSE);
  free_data(&drawing_data);
}
END_TEST

START_TEST(parser_aux_memory_test_1) {
  double **ptr = NULL;
  int error;

  check_mx_allocation(ptr, &error);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(parser_aux_memory_test_2) {
  matrix_t mx;
  int error = 0;
  int index = 2;

  mx.columns = 3;
  mx.rows = 3;

  mx.matrix = (double **)calloc(mx.rows, sizeof(double *));

  if (mx.matrix[0]) mx.matrix[0] = (double *)calloc(3, sizeof(double));
  if (mx.matrix[1]) mx.matrix[1] = (double *)calloc(3, sizeof(double));
  mx.matrix[2] = NULL;

  check_row_allocation(&mx, index, &error);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(parser_is_facet_test) {
  char *buffer = "vertex";

  ck_assert_int_eq(is_facet(buffer), 0);
}

Suite *suite_parser(void) {
  Suite *s = suite_create("suite_parser");
  TCase *tc = tcase_create("case_parser");

  tcase_add_test(tc, parser_test_01);
  tcase_add_test(tc, parser_test_02);
  tcase_add_test(tc, parser_test_empty);
  tcase_add_test(tc, parser_test_wrong_vertcount);
  tcase_add_test(tc, parser_test_wrong_vf);
  tcase_add_test(tc, parser_test_wrong_index);
  tcase_add_test(tc, parser_aux_memory_test_1);
  tcase_add_test(tc, parser_aux_memory_test_2);
  tcase_add_test(tc, parser_is_facet_test);

  suite_add_tcase(s, tc);
  return s;
}