#pragma once
#include <stdlib.h>
#define START_SIZE 2

typedef struct{
    void** _data;
    unsigned int _count;
    unsigned int _size; 
} wobj_stack;

static wobj_stack* wobj_stack_new()
{
    wobj_stack* stack = (wobj_stack*)malloc(sizeof(wobj_stack));
    stack->_size = START_SIZE;
    stack->_count = 0;
    stack->_data = (void**)malloc(sizeof(void*)*START_SIZE);
    return stack;
}
static void __stack_auto_resize(wobj_stack* stack){
    stack->_size = 2*stack->_size;
    stack->_data= (void**) realloc(stack->_data, stack->_size*sizeof(void*));
}
static void wobj_stack_push_back(wobj_stack* stack, void* element)
{
    if(element==NULL) return;
    if(stack->_count+1 >= stack->_size) __stack_auto_resize(stack);
    stack->_data[stack->_count] = element;
    stack->_count++;
}
static void* wobj_stack_pop(wobj_stack* stack)
{
    if(stack->_count==0) return NULL;
    stack->_count--;
    void* elem_to_return = stack->_data[stack->_count];
    stack->_data[stack->_count] = NULL;
    return elem_to_return;
}
static void* wobj_stack_peek(wobj_stack* stack)
{
    if(stack->_count == 0)return NULL;
    return stack->_data[stack->_count-1];
}
static void wobj_stack_destroy(wobj_stack * stack)
{
    free(stack->_data);
    free(stack);
}