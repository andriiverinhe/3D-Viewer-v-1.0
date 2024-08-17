#ifndef TEST_H
#define TEST_H

#include <check.h>

#include "../parser/parser.h"
#include "../transform/transform.h"

#define COORD_NUM 3
#define SCALE_NUM 6

Suite *suite_parser(void);
Suite *suite_transform(void);

void run_test(void);
void run_testcase(Suite *testcase);

#endif