#include "aiv-cunit.h"
#include "quad_test.h"

CUNIT_RUNNER(
   test_quad_vertices,
   test_quad_uvs,
   test_quad_normals,
   test_quad_faces,

   test_util_get,
   test_util_set,

   test_kdtree_compute_bounds,
   test_kdtree_aabb_build,
   test_kdtree_split_aabb,
   test_kdtree_build,
   test_kdtree_is_triangle_in_box
)