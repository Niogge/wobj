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
	float split_distance;
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

typedef struct{
	wobj_triangle * tris;
	float t_hit;
} wobj_hit_result;

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
	aabb->split_distance = 0;

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
static void split_box(wobj_float3 min_box, wobj_float3 max_box, unsigned int split_axis , wobj_float3* out_new_upper_min, wobj_float3* out_new_lower_max, float* split_distance)
{
	*out_new_upper_min = min_box;
	*out_new_lower_max = max_box;
	*split_distance = (wobj_float3_get(split_axis, &min_box) + wobj_float3_get(split_axis, &max_box)) / 2.f;
	wobj_float3_set(split_axis, out_new_upper_min, *split_distance);
	wobj_float3_set(split_axis, out_new_lower_max, *split_distance);

	
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
static void swap_f(float* x, float* y){
	float temp = *x;
	*x = *y;
	*y= temp; 
}
static int rayTriangle_intersection(wobj_float3 * ray_origin, wobj_float3* ray_direction,wobj_triangle* tris, float* t_result)
{
	wobj_float3 e1, e2, h, s, q;
	e1 = wobj_float3_sub(&tris->v2.position, &tris->v1.position);
	e2 = wobj_float3_sub(&tris->v3.position, &tris->v1.position);
	h = wobj_float3_cross(ray_direction, &e2);
	float det = wobj_float3_dot(&h, &e1);

	if (det < EPSILON)
	{

		return 0;
	}
	float f = 1 / det;
	
	s= wobj_float3_sub(ray_origin, &tris->v1.position);
	float u = f * wobj_float3_dot(&s, &h);
	if (u < 0.0 || u>1.0)
	{
		return 0;
	}

	q = wobj_float3_cross(&s, &e1);
	float v = f * wobj_float3_dot(ray_direction, &q);

	if (v < 0.0 || u + v>1.0)
	{
		return 0;
	}
	*t_result = f * wobj_float3_dot(&e2, &q);
	return 1;
}
static void __intersect_r(wobj_float3* ray_origin, 
					wobj_float3* ray_direction,
					wobj_aabb_node* node, 
					wobj_kdtree *kd, 
					float t_start, 
					float t_end, 
					wobj_hit_result* hit_res)
{
	int splitAxis = node->split_axis;
	if (node->is_leaf)
	{
		
		for (int i = 0; i < node->triangles_count; i++)
		{
			wobj_triangle* tri =&kd->model->triangles[node->triangles_index[i]];

			// INTERSECT and populate out_hit
			if(rayTriangle_intersection(ray_origin,ray_direction,tri,&hit_res->t_hit)==1 ){
				hit_res->tris = tri;
				break;
			}
		}
		return;
	}
	float t_split = (node->split_distance - wobj_float3_get(node->split_axis,ray_origin)) / wobj_float3_get(node->split_axis,ray_direction);

	wobj_aabb_node* near = node->r_child;
	wobj_aabb_node* far = node->l_child;
	if ( wobj_float3_get(node->split_axis,ray_origin) < node->split_distance)
	{
		near = node->l_child;
		far = node->r_child;
	}
	
	if (t_split > t_end || t_split < 0)
	{
		__intersect_r(ray_origin,ray_direction,near,kd,t_start,t_end,hit_res);
	}
	else if (t_split < t_start)
	{
		__intersect_r(ray_origin,ray_direction,far,kd,t_start,t_end,hit_res);
	}
	else
	{
		 __intersect_r(ray_origin,ray_direction,near,kd,t_start,t_split,hit_res);
		if (hit_res->t_hit <= t_split && hit_res->t_hit>0)
		{
			return;
		}
		__intersect_r(ray_origin,ray_direction,far,kd,t_split,t_end,hit_res);
	}
}
static int ray_box_intersection(wobj_float3 ray_origin, wobj_float3 ray_direction, wobj_kdtree* kd, wobj_hit_result* t_hit){
	t_hit->t_hit = -1000.0f;
	float t_min, t_0Y, t_0Z;
	float t_max, t_1Y, t_1Z;

	t_min = (kd->root_node->min_bounds.x - ray_origin.x) / ray_direction.x;
	t_max = (kd->root_node->max_bounds.x - ray_origin.x) / ray_direction.x;
	if (t_min > t_max)swap_f(&t_min, &t_max);


	t_0Y = (kd->root_node->min_bounds.y - ray_origin.y) / ray_direction.y;
	t_1Y = (kd->root_node->max_bounds.y - ray_origin.y) / ray_direction.y;
	if (t_0Y > t_1Y)swap_f(&t_0Y, &t_1Y);

	if ((t_min > t_1Y) || (t_0Y > t_max))
	{
		return 0;
	}
	if (t_0Y > t_min) t_min = t_0Y;
	if (t_1Y < t_max) t_max = t_1Y;

	t_0Z = (kd->root_node->min_bounds.z - ray_origin.z) / ray_direction.z;
	t_1Z = (kd->root_node->max_bounds.z - ray_origin.z) / ray_direction.z;
	
	if (t_0Z > t_1Z)swap_f(&t_0Z, &t_1Z);

	if ((t_min > t_1Z) || (t_0Z > t_max))
	{
		return 0;
	}
	if (t_0Z > t_min) t_min = t_0Z;
	if (t_1Z < t_max) t_max = t_1Z;

	__intersect_r(&ray_origin,&ray_direction,kd->root_node,kd,0,10000.0f, t_hit);

	if(t_hit->t_hit>0){
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
	split_box(parent->min_bounds, parent->max_bounds, parent->split_axis, &upper_min, &lower_max, &parent->split_distance);

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
	parent->l_child->triangles_index = (int*)malloc(sizeof(int)*lchild_count);
	parent->r_child->triangles_count = rchild_count;
	parent->r_child->triangles_index = (int*)malloc(sizeof(int)*rchild_count);


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