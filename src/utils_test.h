#include "aiv-cunit.h"
#include "wobj.h"
#include "wobj_utils.h"
#include "wobj_kdtree.h"
#include "wobj_stack.h"

#define UTILS_TESTS    test_util_get,\
   test_util_set

CUNIT_TEST(test_util_get)
{
    wobj_float3 f3;
    f3.x = 1.f;
    f3.y = 2.f;
    f3.z = 3.f;
    CUNIT_FLOAT_EQ(1.f, wobj_float3_get(0, &f3))
    CUNIT_FLOAT_EQ(2.f, wobj_float3_get(1, &f3))
    CUNIT_FLOAT_EQ(3.f, wobj_float3_get(2, &f3))
}
CUNIT_TEST(test_util_set)
{
    wobj_float3 f3;
    f3.x = 1.f;
    f3.y = 2.f;
    f3.z = 3.f;
    CUNIT_FLOAT_EQ(4.f, wobj_float3_set(0,&f3,4.f));
    CUNIT_FLOAT_EQ(4.f, wobj_float3_get(0, &f3));

    CUNIT_FLOAT_EQ(5.f, wobj_float3_set(1,&f3,5.f));
    CUNIT_FLOAT_EQ(5.f, wobj_float3_get(1, &f3))
        
    CUNIT_FLOAT_EQ(6.f, wobj_float3_set(2,&f3,6.f));
    CUNIT_FLOAT_EQ(6.f, wobj_float3_get(2, &f3))
}