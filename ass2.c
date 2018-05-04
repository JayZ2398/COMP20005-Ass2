/*
COMP20005 2018 S1
Assignment 2
Jack Zasezula
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

	// Stage 2
	int num_conflicts;
	//char conflicts[];

	int catchment_cells;
	int is_alive;
} Tree;

typedef Tree Forest[MAX_TREES];

typedef struct {
	int num_trees, total_water;
	Forest forest;
} Data;

void read_data(Data *data);
void initialise_tree(Tree *tree);
void remove_headers();
int mygetchar();

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

	Forest* forest;
	forest = &(data->forest);
	Tree *tree;
	int num_trees = data->num_trees;

	while (scanf("%c%lf%lf%d%lf\n",
				 &label, &xloc, &yloc, &litres, &rootrad) == 5) {

					 tree = forest[num_trees];

					 // Read into tree and initialise values to default
					 tree->label = label;
					 tree->xloc = xloc;
					 tree->yloc = yloc;
					 tree->litres = litres;
					 tree->rootrad = rootrad;

					 printf("BEFORE INIT\nnum_trees = %d\n", num_trees);
					 printf("data->num_trees = %d\n", data->num_trees);
					 initialise_tree(tree);
					 printf("AFTER INIT\nnum_trees = %d\n", num_trees);
					 printf("data->num_trees = %d\n\n", data->num_trees);

					 // Check read worked
					 printf("####READ: tree %c at (%f, %f) with radius %f\n",
					 label, xloc, yloc, rootrad);
					 printf("####SAVE: tree %c at (%f, %f) with radius %f\n",
					 tree->label, tree->xloc, tree->yloc, tree->rootrad);


					 // Maintain forest array counter and its water consumption
					 data->total_water += litres;
					 //printf("total_water = %d\n", data->total_water);

					 printf("BEFORE ++\nnum_trees = %d\n", num_trees);
					 printf("data->num_trees = %d\n", data->num_trees);
					 num_trees++;
					 data->num_trees = num_trees;
					 printf("AFTER ++\nnum_trees = %d\n", num_trees);
					 printf("data->num_trees = %d\n\n", data->num_trees);

					 //printf("Last label read: %c\n", forest[data->num_trees]->label);
					 scanf("%*[ \n\t]");
	 }
}

void initialise_tree(Tree *tree) {
	// Initialise particular members to their default value
	tree->num_conflicts = 0;
	tree->catchment_cells = 0;
	tree->is_alive = TRUE;
}

void remove_headers() {
	/*Consume all characters until header lines have been consumed*/
	int c;
	int newlines = 0;
	while ( newlines < HEADER_LINES && (c = mygetchar()) ) {
		printf("%c ", c);
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
