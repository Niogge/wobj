#include "aiv-cunit.h"
#include "wobj.h"
#include "wobj_utils.h"
#include "wobj_kdtree.h"

CUNIT_TEST(test_quad_vertices)
{
    wobj *quad = wobj_from_file("resources/quad.obj");

    CUNIT_INT_EQ(12, quad->vertices_size);
    CUNIT_INT_EQ(4, quad->vertices_count);

    CUNIT_FLOAT_EQ(-1.f, quad->vertices[0]);
    CUNIT_FLOAT_EQ(1.f, quad->vertices[1]);
    CUNIT_FLOAT_EQ(0.f, quad->vertices[2]);

    CUNIT_FLOAT_EQ(1.f, quad->vertices[3]);
    CUNIT_FLOAT_EQ(1.f, quad->vertices[4]);
    CUNIT_FLOAT_EQ(0.f, quad->vertices[5]);

    CUNIT_FLOAT_EQ(-1.f, quad->vertices[6]);
    CUNIT_FLOAT_EQ(-1.f, quad->vertices[7]);
    CUNIT_FLOAT_EQ(0.f, quad->vertices[8]);

    CUNIT_FLOAT_EQ(1.f, quad->vertices[9]);
    CUNIT_FLOAT_EQ(-1.f, quad->vertices[10]);
    CUNIT_FLOAT_EQ(0.f, quad->vertices[11]);
    wobj_destroy(quad);
}

CUNIT_TEST(test_quad_uvs)
{
    wobj *quad = wobj_from_file("resources/quad.obj");

    CUNIT_INT_EQ(8, quad->uvs_size);
    CUNIT_INT_EQ(4, quad->uvs_count);

    CUNIT_FLOAT_EQ(1.f, quad->uvs[0]);
    CUNIT_FLOAT_EQ(1.f, quad->uvs[1]);

    CUNIT_FLOAT_EQ(0.f, quad->uvs[2]);
    CUNIT_FLOAT_EQ(1.f, quad->uvs[3]);

    CUNIT_FLOAT_EQ(0.f, quad->uvs[4]);
    CUNIT_FLOAT_EQ(0.f, quad->uvs[5]);

    CUNIT_FLOAT_EQ(1.f, quad->uvs[6]);
    CUNIT_FLOAT_EQ(0.f, quad->uvs[7]);
    wobj_destroy(quad);

}


CUNIT_TEST(test_quad_normals)
{
    wobj *quad = wobj_from_file("resources/quad.obj");

    CUNIT_INT_EQ(3, quad->normals_size);
    CUNIT_INT_EQ(1, quad->normals_count);

    CUNIT_FLOAT_EQ(0.f, quad->normals[0]);
    CUNIT_FLOAT_EQ(0.f, quad->normals[1]);
    CUNIT_FLOAT_EQ(1.f, quad->normals[2]);

    wobj_destroy(quad);
   
}

CUNIT_TEST(test_quad_faces)
{
    wobj *quad = wobj_from_file("resources/quad.obj");

    CUNIT_INT_EQ(2, quad->faces_count);
    CUNIT_INT_EQ(18, quad->faces_size);

    CUNIT_INT_EQ(4, quad->faces[0]);
    CUNIT_INT_EQ(4, quad->faces[1]);
    CUNIT_INT_EQ(1, quad->faces[2]);

    CUNIT_INT_EQ(3, quad->faces[3]);
    CUNIT_INT_EQ(3, quad->faces[4]);
    CUNIT_INT_EQ(1, quad->faces[5]);
    
    CUNIT_INT_EQ(1, quad->faces[6]);
    CUNIT_INT_EQ(2, quad->faces[7]);
    CUNIT_INT_EQ(1, quad->faces[8]);
    
    CUNIT_INT_EQ(2, quad->faces[9]);
    CUNIT_INT_EQ(1, quad->faces[10]);
    CUNIT_INT_EQ(1, quad->faces[11]);

    CUNIT_INT_EQ(4, quad->faces[12]);
    CUNIT_INT_EQ(4, quad->faces[13]);
    CUNIT_INT_EQ(1, quad->faces[14]);
    
    CUNIT_INT_EQ(1, quad->faces[15]);
    CUNIT_INT_EQ(2, quad->faces[16]);
    CUNIT_INT_EQ(1, quad->faces[17]);
    
    wobj_destroy(quad);

}

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

    wobj_float3 upper_min_b_xsplit, lower_max_b_xsplit;
    split_box(min_b, max_b, 0,&upper_min_b_xsplit,&lower_max_b_xsplit);
    CUNIT_FLOAT_EQ(0.0f, upper_min_b_xsplit.x)
    CUNIT_FLOAT_EQ(-1.0f, upper_min_b_xsplit.y)
    CUNIT_FLOAT_EQ(0.0f, upper_min_b_xsplit.z)
    
    CUNIT_FLOAT_EQ(0.0f, lower_max_b_xsplit.x)
    CUNIT_FLOAT_EQ(1.0f, lower_max_b_xsplit.y)
    CUNIT_FLOAT_EQ(0.0f, lower_max_b_xsplit.z)

    wobj_float3 upper_min_b_ysplit, lower_max_b_ysplit;
    split_box(min_b, max_b, 1,&upper_min_b_ysplit,&lower_max_b_ysplit);
    CUNIT_FLOAT_EQ(-1.0f, upper_min_b_ysplit.x)
    CUNIT_FLOAT_EQ(0.0f, upper_min_b_ysplit.y)
    CUNIT_FLOAT_EQ(0.0f, upper_min_b_ysplit.z)
    
    CUNIT_FLOAT_EQ(1.0f, lower_max_b_ysplit.x)
    CUNIT_FLOAT_EQ(0.0f, lower_max_b_ysplit.y)
    CUNIT_FLOAT_EQ(0.0f, lower_max_b_ysplit.z)

    wobj_float3 upper_min_b_zsplit, lower_max_b_zsplit;
    split_box(min_b, max_b, 2,&upper_min_b_zsplit,&lower_max_b_zsplit);
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