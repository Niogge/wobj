#pragma once
#include "wobj.h"
#include "wobj_utils.h"
struct _wobj_aabb_node
{
	wobj_float3 min_bounds;
	wobj_float3 max_bounds;
	unsigned int is_leaf;
	int* triangles_index;
	int triangles_count;
	unsigned int box_depth;
	unsigned int split_axis;
	struct _wobj_aabb_node* l_child;
	struct _wobj_aabb_node* r_child;
};
typedef struct _wobj_aabb_node wobj_aabb_node;
typedef struct
{
	wobj* model;
	wobj_aabb_node* root_node;
	unsigned int depth;
} wobj_kdtree;


static wobj_aabb_node* __aabb_build(wobj_float3 minb, wobj_float3 maxb, unsigned int depth)
{
	wobj_aabb_node* aabb = (wobj_aabb_node*)malloc(sizeof(wobj_aabb_node));
	aabb->max_bounds = maxb;
	aabb->min_bounds = minb;
	aabb->box_depth = depth;
	aabb->is_leaf = 0;
	aabb->l_child = NULL;
	aabb->r_child = NULL;
	aabb->triangles_index = NULL;

	return aabb;
}
static void wobj_compute_bounds(wobj* model, wobj_float3* out_max_bounds, wobj_float3* out_min_bounds )
{
	float max_x = -100000.f;
	float max_y = -100000.f;
	float max_z = -1000.f;

	float min_x = 100000.f;
	float min_y = 100000.f;
	float min_z = 100000.f;

	for (int i = 0; i < model->vertices_size; i=i+3)
	{
		float curr_x = model->vertices[i + 0];
		float curr_y = model->vertices[i + 1];
		float curr_z = model->vertices[i + 2];

		if (curr_x > max_x) max_x = curr_x;
		if (curr_x < min_x) min_x = curr_x;

		if (curr_y > max_y) max_y = curr_y;
		if (curr_y < min_y) min_y = curr_y;

		if (curr_z > max_z) max_z = curr_z;
		if (curr_z < min_z) min_z = curr_z;
	}
	out_max_bounds->x = max_x;
	out_max_bounds->y = max_y;
	out_max_bounds->z = max_z;

	out_min_bounds->x = min_x;
	out_min_bounds->y = min_y;
	out_min_bounds->z = min_z;
}
static void split_box(wobj_float3 min_box, wobj_float3 max_box, unsigned int split_axis , wobj_float3* out_new_upper_min, wobj_float3* out_new_lower_max)
{
	*out_new_upper_min = min_box;
	*out_new_lower_max = max_box;
	float middle = (wobj_float3_get(split_axis, &min_box) + wobj_float3_get(split_axis, &max_box)) / 2.f;
	wobj_float3_set(split_axis, out_new_upper_min, middle);
	wobj_float3_set(split_axis, out_new_lower_max, middle);

	
}
static unsigned int is_triangle_in_box(wobj_aabb_node* box,wobj_triangle* tris, unsigned int axis )
{
	float min = wobj_float3_get(axis, &box->min_bounds);
	float max = wobj_float3_get(axis, &box->max_bounds);
	float v1_ = wobj_float3_get(axis,&tris->v1.position);
	float v2_ = wobj_float3_get(axis,&tris->v2.position);
	float v3_ = wobj_float3_get(axis,&tris->v3.position);
	if(v1_<= max && v1_>= min){
		//is inside
		return 1;
	}
	if(v2_<= max && v2_>= min){
		//is inside
		return 1;
	}
	if(v3_<= max && v3_>= min){
		//is inside
		return 1;
	}
	return 0;
}
static void _recurse_aabb(wobj_aabb_node* parent, wobj_kdtree* kd)
{
	if (parent->box_depth >= kd->depth)
	{
		parent->is_leaf = 1;
		return;
	}

	//pick split, split in half for now
	wobj_float3 lower_max, upper_min;
	split_box(parent->min_bounds, parent->max_bounds, parent->split_axis, &upper_min, &lower_max);

	parent->l_child = __aabb_build(parent->min_bounds, lower_max, parent->box_depth + 1);
	parent->r_child = __aabb_build(upper_min, parent->max_bounds, parent->box_depth + 1);

	//count triangles inside boxes
	unsigned int lchild_count = 0;
	unsigned int rchild_count = 0;
	for (int i = 0; i < parent->triangles_count; i++)
	{
		//get triangle; 
		wobj_triangle triangle = kd->model->triangles[parent->triangles_index[i]];
		if(is_triangle_in_box(parent->l_child,&triangle, parent->split_axis))
			lchild_count++;
		if(is_triangle_in_box(parent->r_child, &triangle, parent->split_axis))
			rchild_count++;
	}
	parent->l_child->triangles_count = lchild_count;
	parent->l_child->triangles_index = malloc(sizeof(int)*lchild_count);
	parent->r_child->triangles_count = rchild_count;
	parent->r_child->triangles_index = malloc(sizeof(int)*rchild_count);


	lchild_count = 0;
	rchild_count = 0;
	//sort triangle in boxes
	for (int i = 0; i < parent->triangles_count; i++)
	{
		//get triangle; 
		int triangle_index = parent->triangles_index[i];
		wobj_triangle triangle = kd->model->triangles[triangle_index];
		if(is_triangle_in_box(parent->l_child,&triangle, parent->split_axis))
		{
			parent->l_child->triangles_index[lchild_count] = triangle_index;			
			lchild_count++;
		}
		if(is_triangle_in_box(parent->r_child, &triangle, parent->split_axis))
		{
			parent->r_child->triangles_index[rchild_count] = triangle_index;			
			rchild_count++;
		}
	}

	//prepare for next step 
	parent->l_child->split_axis = (parent->split_axis+1)%3;
	parent->r_child->split_axis = (parent->split_axis+1)%3;

	if(parent->l_child->triangles_count >0){
		_recurse_aabb(parent->l_child, kd);
	}
	else{
		parent->l_child->is_leaf =1;
	}

	if(parent->r_child->triangles_count >0){
		_recurse_aabb(parent->r_child, kd);
	}
	else{
		parent->r_child->is_leaf =1;
	}
}

static wobj_kdtree* wobj_kdtree_from_file(const char* file_path, unsigned int max_depth)
{
	wobj_kdtree* wkd = (wobj_kdtree*)malloc(sizeof(wobj_kdtree));
	wkd->model = wobj_from_file(file_path);
	wobj_float3 min_b, max_b;
	wobj_compute_bounds(wkd->model, &max_b, &min_b);
	wkd->root_node = __aabb_build(min_b, max_b, 0);
	wkd->root_node->triangles_index = (int*)malloc(sizeof(int) * wkd->model->triangles_count);
	for (int i = 0; i < wkd->model->triangles_count; i++)
	{
		wkd->root_node->triangles_index[i] = i; //I know, I know.
	}
	wkd->root_node->triangles_count = wkd->model->triangles_count;
	wkd->root_node->split_axis = 0;
	wkd->depth = max_depth;
	_recurse_aabb(wkd->root_node, wkd );
	return wkd;

}
static void __aabb_destroy(wobj_aabb_node* node)
{
	if (node->l_child != NULL)
	{
		__aabb_destroy(node->l_child);
	}
	if (node->r_child != NULL)
	{
		__aabb_destroy(node->r_child);
	}
	if(node->triangles_index){
		free(node->triangles_index);
	}
	free(node);
}

static void wobj_kdtree_destroy(wobj_kdtree* wkd)
{
	wobj_destroy(wkd->model);
	__aabb_destroy(wkd->root_node);
	free(wkd);
}