#include "aiv-cunit.h"
#include "wobj.h"
#include "wobj_utils.h"
#include "wobj_kdtree.h"
#include "wobj_stack.h"

#define KDTREE_TESTS   test_kdtree_compute_bounds,\
   test_kdtree_aabb_build,\
   test_kdtree_split_aabb,\
   test_kdtree_build,\
   test_kdtree_is_triangle_in_box



CUNIT_TEST(test_kdtree_compute_bounds)
{
    wobj *quad = wobj_from_file("resources/quad.obj");

    wobj_float3 min_b, max_b;
    wobj_compute_bounds(quad, &max_b, &min_b);
    CUNIT_FLOAT_EQ(-1.0f, min_b.x);
    CUNIT_FLOAT_EQ(-1.0f, min_b.y);
    CUNIT_FLOAT_EQ(0.f, min_b.z);

    CUNIT_FLOAT_EQ(1.0f, max_b.x);
    CUNIT_FLOAT_EQ(1.0f, max_b.y);
    CUNIT_FLOAT_EQ(0.f, max_b.z);
    wobj_destroy(quad);
   
}
CUNIT_TEST(test_kdtree_aabb_build){
    wobj *quad = wobj_from_file("resources/quad.obj");

    wobj_float3 min_b, max_b;
    wobj_compute_bounds(quad, &max_b, &min_b);
    wobj_aabb_node* node = __aabb_build(min_b, max_b,5);

    CUNIT_FLOAT_EQ(-1.0f, node->min_bounds.x)
    CUNIT_FLOAT_EQ(-1.0f, node->min_bounds.y)
    CUNIT_FLOAT_EQ(0.0f, node->min_bounds.z)
    
    CUNIT_FLOAT_EQ(1.0f, node->max_bounds.x)
    CUNIT_FLOAT_EQ(1.0f, node->max_bounds.y)
    CUNIT_FLOAT_EQ(0.0f, node->max_bounds.z)

    CUNIT_INT_EQ(5, node->box_depth);
    CUNIT_IS_TRUE(!node->is_leaf)
    __aabb_destroy(node);
    wobj_destroy(quad);
}

CUNIT_TEST(test_kdtree_split_aabb)
{
    wobj *quad = wobj_from_file("resources/quad.obj");

    wobj_float3 min_b, max_b;
    wobj_compute_bounds(quad, &max_b, &min_b);
    wobj_aabb_node* node = __aabb_build(min_b, max_b,5);

    float split_dist_x;
    wobj_float3 upper_min_b_xsplit, lower_max_b_xsplit;
    split_box(min_b, max_b, 0,&upper_min_b_xsplit,&lower_max_b_xsplit,&split_dist_x);
    CUNIT_FLOAT_EQ(0.0f, upper_min_b_xsplit.x)
    CUNIT_FLOAT_EQ(-1.0f, upper_min_b_xsplit.y)
    CUNIT_FLOAT_EQ(0.0f, upper_min_b_xsplit.z)
    
    CUNIT_FLOAT_EQ(0.0f, lower_max_b_xsplit.x)
    CUNIT_FLOAT_EQ(1.0f, lower_max_b_xsplit.y)
    CUNIT_FLOAT_EQ(0.0f, lower_max_b_xsplit.z)

    float split_dist_y;
    wobj_float3 upper_min_b_ysplit, lower_max_b_ysplit;
    split_box(min_b, max_b, 1,&upper_min_b_ysplit,&lower_max_b_ysplit,&split_dist_y);
    CUNIT_FLOAT_EQ(-1.0f, upper_min_b_ysplit.x)
    CUNIT_FLOAT_EQ(0.0f, upper_min_b_ysplit.y)
    CUNIT_FLOAT_EQ(0.0f, upper_min_b_ysplit.z)
    
    CUNIT_FLOAT_EQ(1.0f, lower_max_b_ysplit.x)
    CUNIT_FLOAT_EQ(0.0f, lower_max_b_ysplit.y)
    CUNIT_FLOAT_EQ(0.0f, lower_max_b_ysplit.z)


    float split_dist_z;
    wobj_float3 upper_min_b_zsplit, lower_max_b_zsplit;
    split_box(min_b, max_b, 2,&upper_min_b_zsplit,&lower_max_b_zsplit,&split_dist_z);
    CUNIT_FLOAT_EQ(-1.0f, upper_min_b_zsplit.x)
    CUNIT_FLOAT_EQ(-1.0f, upper_min_b_zsplit.y)
    CUNIT_FLOAT_EQ(0.0f, upper_min_b_zsplit.z)
    
    CUNIT_FLOAT_EQ(1.0f, lower_max_b_zsplit.x)
    CUNIT_FLOAT_EQ(1.0f, lower_max_b_zsplit.y)
    CUNIT_FLOAT_EQ(0.0f, lower_max_b_zsplit.z)


    __aabb_destroy(node);
    wobj_destroy(quad);
}

CUNIT_TEST(test_kdtree_build)
{
    wobj_kdtree * wkd = wobj_kdtree_from_file("resources/quad.obj", 1);
    CUNIT_INT_EQ(12, wkd->model->vertices_size);
    CUNIT_INT_EQ(4, wkd->model->vertices_count);

    CUNIT_FLOAT_EQ(-1.f, wkd->model->vertices[0]);
    CUNIT_FLOAT_EQ(1.f, wkd->model->vertices[1]);
    CUNIT_FLOAT_EQ(0.f, wkd->model->vertices[2]);

    CUNIT_FLOAT_EQ(1.f, wkd->model->vertices[3]);
    CUNIT_FLOAT_EQ(1.f, wkd->model->vertices[4]);
    CUNIT_FLOAT_EQ(0.f, wkd->model->vertices[5]);

    CUNIT_FLOAT_EQ(-1.f, wkd->model->vertices[6]);
    CUNIT_FLOAT_EQ(-1.f, wkd->model->vertices[7]);
    CUNIT_FLOAT_EQ(0.f, wkd->model->vertices[8]);

    CUNIT_FLOAT_EQ(1.f, wkd->model->vertices[9]);
    CUNIT_FLOAT_EQ(-1.f, wkd->model->vertices[10]);
    CUNIT_FLOAT_EQ(0.f, wkd->model->vertices[11]);
    
    //root node
    CUNIT_FLOAT_EQ(-1.0f, wkd->root_node->min_bounds.x);
    CUNIT_FLOAT_EQ(-1.0f, wkd->root_node->min_bounds.y);
    CUNIT_FLOAT_EQ(0.f, wkd->root_node->min_bounds.z);

    CUNIT_FLOAT_EQ(1.0f, wkd->root_node->max_bounds.x);
    CUNIT_FLOAT_EQ(1.0f, wkd->root_node->max_bounds.y);
    CUNIT_FLOAT_EQ(0.f, wkd->root_node->max_bounds.z);

    //SPLIT IS ON X (0)
    //first l_child
    CUNIT_FLOAT_EQ(-1.0f, wkd->root_node->l_child->min_bounds.x);
    CUNIT_FLOAT_EQ(-1.0f, wkd->root_node->l_child->min_bounds.y);
    CUNIT_FLOAT_EQ(0.f, wkd->root_node->l_child->min_bounds.z);

    CUNIT_FLOAT_EQ(0.0f, wkd->root_node->l_child->max_bounds.x); 
    CUNIT_FLOAT_EQ(1.0f, wkd->root_node->l_child->max_bounds.y);
    CUNIT_FLOAT_EQ(0.f, wkd->root_node->l_child->max_bounds.z);

    //first r_child
    CUNIT_FLOAT_EQ(0.0f, wkd->root_node->r_child->min_bounds.x); 
    CUNIT_FLOAT_EQ(-1.0f, wkd->root_node->r_child->min_bounds.y);
    CUNIT_FLOAT_EQ(0.f, wkd->root_node->r_child->min_bounds.z);

    CUNIT_FLOAT_EQ(1.0f, wkd->root_node->r_child->max_bounds.x);
    CUNIT_FLOAT_EQ(1.0f, wkd->root_node->r_child->max_bounds.y);
    CUNIT_FLOAT_EQ(0.f, wkd->root_node->r_child->max_bounds.z);
    
    //they should contain 2 tris each (yeah, quad is no fun.)
    CUNIT_INT_EQ(2, wkd->root_node->l_child->triangles_count);
    CUNIT_INT_EQ(2, wkd->root_node->r_child->triangles_count);

    //next split should be on y (1)
    CUNIT_INT_EQ(1, wkd->root_node->l_child->split_axis);
    CUNIT_INT_EQ(1, wkd->root_node->r_child->split_axis);

    //and then should stop recursion after first level;
    CUNIT_IS_TRUE(!wkd->root_node->l_child->l_child);
    CUNIT_IS_TRUE(!wkd->root_node->l_child->r_child);
    CUNIT_IS_TRUE(!wkd->root_node->r_child->l_child);
    CUNIT_IS_TRUE(!wkd->root_node->r_child->r_child);
    
    CUNIT_INT_EQ(0, wkd->root_node->box_depth);
    CUNIT_IS_TRUE(!wkd->root_node->is_leaf);
    
    CUNIT_INT_EQ(2, wkd->root_node->triangles_count);


    CUNIT_INT_EQ(1, wkd->depth);

    wobj_kdtree_destroy(wkd);
}

CUNIT_TEST(test_kdtree_is_triangle_in_box)
{
    wobj_kdtree * wkd = wobj_kdtree_from_file("resources/quad.obj", 5);

    CUNIT_IS_TRUE(is_triangle_in_box(wkd->root_node,&wkd->model->triangles[0],0));
    CUNIT_IS_TRUE(is_triangle_in_box(wkd->root_node,&wkd->model->triangles[0],1));
    CUNIT_IS_TRUE(is_triangle_in_box(wkd->root_node,&wkd->model->triangles[0],2));
    
    wobj_float3 min;
    min.x = -10.f;
    min.y = -10.f;
    min.z = -10.f;
    wobj_float3 max;
    max.x = -8.f;
    max.y = -8.f;
    max.z = -8.f;

    wobj_aabb_node* node_out = __aabb_build(min, max, 0);

    CUNIT_IS_TRUE(!is_triangle_in_box(node_out,&wkd->model->triangles[0],0));
    CUNIT_IS_TRUE(!is_triangle_in_box(node_out,&wkd->model->triangles[0],1));
    CUNIT_IS_TRUE(!is_triangle_in_box(node_out,&wkd->model->triangles[0],2));

    __aabb_destroy(node_out);
    wobj_kdtree_destroy(wkd);
}
