#include "aiv-cunit.h"
#include "quad_test.h"
#include "kdtree_test.h"
#include "utils_test.h"
#include "stack_test.h"

CUNIT_RUNNER(
   QUAD_TESTS,
   UTILS_TESTS,
   KDTREE_TESTS,
   STACK_TESTS
)