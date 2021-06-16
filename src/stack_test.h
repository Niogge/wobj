#include "aiv-cunit.h"
#include "wobj.h"
#include "wobj_utils.h"
#include "wobj_kdtree.h"
#include "wobj_stack.h"

#define STACK_TESTS test_stack_new,\
   test_stack_push,\
   test_stack_push_with_resize,\
   test_stack_pop,\
   test_stack_peek

CUNIT_TEST(test_stack_new){
    wobj_stack* stack = wobj_stack_new();
    CUNIT_INT_EQ(2, stack->_size);
    CUNIT_INT_EQ(0, stack->_count);
    CUNIT_IS_TRUE(stack->_data);
    wobj_stack_destroy(stack);
}

CUNIT_TEST(test_stack_push)
{
    wobj_stack* stack = wobj_stack_new();
    int a=2;
    wobj_stack_push_back(stack, &a);
    CUNIT_INT_EQ(2, stack->_size);
    CUNIT_INT_EQ(1, stack->_count);
    CUNIT_INT_EQ(2, *((int*)stack->_data[0]))


    wobj_stack_destroy(stack);
}

CUNIT_TEST(test_stack_push_with_resize)
{
    wobj_stack* stack = wobj_stack_new();
    int a=2;
    wobj_stack_push_back(stack, &a);
    CUNIT_INT_EQ(2, stack->_size);
    CUNIT_INT_EQ(1, stack->_count);
    CUNIT_INT_EQ(2, *((int*)stack->_data[0]))
    int b=3;
    wobj_stack_push_back(stack, &b);
    CUNIT_INT_EQ(4, stack->_size);
    CUNIT_INT_EQ(2, stack->_count);
    CUNIT_INT_EQ(2, *((int*)stack->_data[0]))
    CUNIT_INT_EQ(3, *((int*)stack->_data[1]))

    wobj_stack_destroy(stack);
}

CUNIT_TEST(test_stack_pop)
{
    wobj_stack* stack = wobj_stack_new();
    int a=2;
    wobj_stack_push_back(stack, &a);
    int b=3;
    wobj_stack_push_back(stack, &b);

    int val =*( (int*)wobj_stack_pop(stack));
    CUNIT_INT_EQ(3,val);
    CUNIT_INT_EQ(1, stack->_count);

    wobj_stack_destroy(stack);
}

CUNIT_TEST(test_stack_peek)
{
    wobj_stack* stack = wobj_stack_new();
    int a=2;
    wobj_stack_push_back(stack, &a);
    int b=3;
    wobj_stack_push_back(stack, &b);

    int val =*( (int*)wobj_stack_peek(stack));
    CUNIT_INT_EQ(3,val);
    CUNIT_INT_EQ(2, stack->_count);

    wobj_stack_destroy(stack);

}