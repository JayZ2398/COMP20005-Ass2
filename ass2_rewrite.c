/*
COMP20005 2018 S1
Assignment 2
Jack Zezula
04/05/2018 - ????
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_TREES 52
#define HEADER_LINES 1
#define TRUE 1
#define FALSE 0

typedef struct {
	// Stage 1 vars
	char label;
	double xloc, yloc, rootrad;
	int litres;

	/*
	// Stage 2
	int num_conflicts;
	char conflicts[MAX_TREES - 1];

	int catchment_cells;
	int is_alive;
	*/
} Tree;

typedef Tree Forest[MAX_TREES];

typedef struct {
	int num_trees, total_water;
	Forest forest;
} Data;

void read_data(Data *data);
void remove_headers();
int mygetchar();
void print_tree_data(Tree* tree_ptr);

int
main(int argc, char *argv[]) {
	// Initialise data
	Data data = {
		.num_trees = 0,
		.total_water = 0,
	};

	remove_headers();
	read_data(&data);

  return 0;
}

void read_data(Data *data) {
	char label;
	double xloc, yloc, rootrad;
	int litres;

	Tree* tree_ptr;
	tree_ptr = data->forest;

	printf("####In read_data:\n\n");

	while (scanf("%c%lf%lf%d%lf\n",
				 &label, &xloc, &yloc, &litres, &rootrad) == 5) {

					tree_ptr->label = label;
					tree_ptr->xloc = xloc;
					tree_ptr->yloc = yloc;
					tree_ptr->litres = litres;
					tree_ptr->rootrad = rootrad;
					print_tree_data(tree_ptr);

					data->total_water += litres;
					//printf("data->total_water = %d\n", data->total_water);

					tree_ptr++;
	 }

}

void print_tree_data(Tree* tree_ptr) {
	printf("==== Tree at %p (%I64d bytes)\n", tree_ptr, sizeof(Tree));
	printf("tree_ptr->label  = %c\n", tree_ptr->label);
	printf("tree_ptr->xloc   = %f\n", tree_ptr->xloc);
	printf("tree_ptr->yloc   = %f\n", tree_ptr->yloc);
	printf("tree_ptr->litres = %d\n", tree_ptr->litres);
	printf("tree_ptr->radius = %f\n\n", tree_ptr->rootrad);
}

void remove_headers() {
	/*Consume all characters until header lines have been consumed*/
	int c;
	int newlines = 0;
	while ( newlines < HEADER_LINES && (c = mygetchar()) ) {
		if (c == '\n') {
			newlines++;
		}
	}
}

int	mygetchar() {
	int c;
	while ((c=getchar())=='\r');
	return c;
}
