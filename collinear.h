#ifndef COLINEAR_H_
#define COLINEAR_H_

#define NUM_POINTS 10

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct point_3d
{
	float x;
	float y;
    float z;
} point_3d;

typedef struct point_2d
{
	float x;
	float y;
} point_2d;

typedef enum
{
	X,
	Y,
	Z
} planes_t;

typedef enum
{
	TRUE = 1,
	FALSE = 0
} boolean_t;

void convert_3d_to_2d_point(struct point_3d *, struct point_2d *, planes_t);

void convert_3d_to_2d_array(struct point_3d *points_3d, struct point_2d *points_2d, planes_t plane_to_drop);

float calc_collinearity(struct point_2d *);

float find_collinearity(struct point_3d *, planes_t);

float find_max_distance(struct point_2d *, struct point_2d *, struct point_2d *);

void load_3d_points(struct point_3d *);

void add_3d_point(struct point_3d *, float, float, float, int);

float find_distance_between_points(struct point_2d *, struct point_2d *);

boolean_t is_same_point(struct point_2d *, struct point_2d *);

float sum_distance_of_points(struct point_2d *, struct point_2d *, struct point_2d *);

float find_distance_from_line(struct point_2d *, struct point_2d *, struct point_2d *);

#endif