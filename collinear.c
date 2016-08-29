#include "collinear.h"


float find_collinearity(struct point_3d *points_3d, planes_t plane_to_drop)
{
    point_2d* points_2d = malloc(NUM_POINTS * sizeof(*points_2d));
    convert_3d_to_2d_array(points_3d, points_2d, plane_to_drop);

    return calc_collinearity(points_2d);
}

void convert_3d_to_2d_array(struct point_3d *points_3d, struct point_2d *points_2d, planes_t plane_to_drop)
{
    for (int i=0; i<NUM_POINTS; i++)
    {
        struct point_2d point_2d;
        convert_3d_to_2d_point(&points_3d[i], &point_2d, plane_to_drop);
        points_2d[i] = point_2d;
    }
}

float calc_collinearity(struct point_2d *points)
{
    struct point_2d min_point;
    struct point_2d max_point;
    
    float max_distance = find_max_distance(points, &min_point, &max_point);
    float sum_point_distance = sum_distance_of_points(points, &min_point, &max_point);
    float sqr_root_sum_point_distance = sqrtf(sum_point_distance);

    printf("sum_point_distance: %.2f\n", sum_point_distance);
    printf("sqr_root_sum_point_distance: %.2f\n", sqr_root_sum_point_distance);

    float collinearity = 1 - (sqr_root_sum_point_distance / (max_distance * max_distance));
    if (collinearity > 1)
        collinearity = 1;

    if (collinearity < 0)
        collinearity = 0;

    return collinearity;
}

void convert_3d_to_2d_point(struct point_3d *point3d, struct point_2d *point2d, planes_t plane_to_drop)
{
    switch (plane_to_drop)
    {
        case X:
            point2d->x = point3d->y;
            point2d->y = point3d->z;
            break;
        case Y:
            point2d->x = point3d->x;
            point2d->y = point3d->z;
            break;
        default:
            point2d->x = point3d->x;
            point2d->y = point3d->y;
            break;
    }
}

void load_3d_points(struct point_3d *points)
{
    int i = 0;

    // my test data
    // collinearity for all 3: .751
    // x y
    /*
    add_3d_point(points, 2, 2, -9.31, i++);
    add_3d_point(points, 3, 3, 3.85, i++);
    add_3d_point(points, 4, 4, 6.90, i++);
    add_3d_point(points, 5, 2, 6.90, i++);
    
    // y z
    add_3d_point(points, -22, 2, 2, i++);
    add_3d_point(points, -23, 3, 3, i++);
    add_3d_point(points, -33, 4, 4, i++);
    add_3d_point(points, -35, 5, 2, i++);
    
    // x z
    add_3d_point(points, 2, -42, 2, i++);
    add_3d_point(points, 3, -33, 3, i++);
    add_3d_point(points, 4, -54, 4, i++);
    add_3d_point(points, 5, -22, 2, i++);
    */
    
    // LU test data
    
    add_3d_point(points, 1.1, 2.05, 13.40, i++);
    add_3d_point(points, 2.1, 3.17, 9.19, i++);
    add_3d_point(points, 4.9, 6.02, -9.59, i++);
    add_3d_point(points, -3.2, -2.15, -13.10, i++);
    add_3d_point(points, 14.1, 14.95, 3.79, i++);
    add_3d_point(points, -12.08, -10.9, 5.60, i++);
    add_3d_point(points, 6, 12, 12.53, i++);
    add_3d_point(points, 0, 0, -0.27, i++);
    add_3d_point(points, -11, -10.1, -3.48, i++);
    add_3d_point(points, 6, 12, 7.64, i++);
    
}

void add_3d_point(point_3d *points, float x, float y, float z, int counter)
{
    struct point_3d temp_point;
    temp_point.x = x;
    temp_point.y = y;
    temp_point.z = z;

    points[counter] = temp_point;
}

float find_max_distance(struct point_2d *points, struct point_2d *min_point, struct point_2d *max_point)
{
    float temp_distance, max_distance = 0;

    for (int i=0; i<NUM_POINTS; i++)
    {
        for (int j=NUM_POINTS-1; j>i; j--)
        {
            temp_distance = find_distance_between_points(&points[i], &points[j]);

            // >= to make sure the min and max points will be set even if all the points are the same
            if (temp_distance >= max_distance)
            {
                max_distance = temp_distance;

                // how do i do this directly?
                min_point->x = points[i].x;
                min_point->y = points[i].y;

                max_point->x = points[j].x;
                max_point->y = points[j].y;                    
            }
        }
    }

    printf("max_distance: %.2f\n", max_distance);
    return max_distance;
}

float find_distance_between_points(struct point_2d *point1, struct point_2d *point2)
{
    float dx = point2->x - point1->x;
    float dy = point2->y - point1->y;

    return sqrt(dx*dx + dy*dy);
}

boolean_t is_same_point(struct point_2d *first_point, struct point_2d *second_point)
{
    return first_point->x == second_point->x &&
           first_point->y == second_point->y;
}

float sum_distance_of_points(struct point_2d *points, struct point_2d *min_point, struct point_2d *max_point)
{
    float sum_distance = 0;
    for (int i=0; i<NUM_POINTS; i++)
    {
        boolean_t is_min_point = is_same_point(min_point, &points[i]);
        boolean_t is_max_point = is_same_point(max_point, &points[i]);

        if (!is_min_point && !is_max_point)
        {
            float temp_dist = find_distance_from_line(min_point, max_point, &points[i]);
            sum_distance += (temp_dist * temp_dist);
        }
    }

    return sum_distance;
}

float find_distance_from_line(struct point_2d *min_point, struct point_2d *max_point, struct point_2d *this_point)
{
    float x0 = this_point->x;
    float y0 = this_point->y;

    float x1 = min_point->x;
    float y1 = min_point->y;

    float x2 = max_point->x;
    float y2 = max_point->y;

    float numerator = fabsf((y2-y1)*x0 - (x2-x1)*y0 + x2*y1 - y2*x1);
    float denom = sqrtf((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1));

    float distance = numerator / denom;
    
    return distance;
}