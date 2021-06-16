#include "aiv-cunit.h"
#include "wobj.h"
#include "wobj_utils.h"
#include "wobj_kdtree.h"
#include "wobj_stack.h"
#define QUAD_TESTS  test_quad_vertices,\
   test_quad_uvs,\
   test_quad_normals,\
   test_quad_faces


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


