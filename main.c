#include "collinear.h"

int main (int argc, char **argv)
{   
    point_3d* points = malloc(NUM_POINTS * sizeof(*points));
    
    load_3d_points(points);
    
    float collinearityX = find_collinearity(points, X);
    float collinearityY = find_collinearity(points, Y);
    float collinearityZ = find_collinearity(points, Z);
    	
    printf("collinearity without X: %f\n", collinearityX);
    printf("collinearity without Y: %f\n", collinearityY);
    printf("collinearity without Z: %f\n", collinearityZ);
    
	return 0;
}