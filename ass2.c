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
	double xloc, yloc, radius;
	int litres;

	// Stage 2
	int num_conflicts;
	char conflicts[MAX_TREES - 1];

	//Stage 3 & 4
	int catchment_cells;
	int is_alive;
} Tree;

typedef Tree Forest[MAX_TREES];

typedef struct {
	int num_trees, total_water;
	Forest forest;
} Data;

// Tree functions
void initialise_tree(Tree *tree);
void print_tree_data(Tree *tree);
void find_conflicting_trees(Data *data);
double distance_between_trees(Tree *tree, Tree *other);

// Input handlers
void read_data(Data *data);
void remove_headers();
int mygetchar();

// Output functions
void stage1_output(Data *data);
void stage2_output(Data *data);

int
main(int argc, char *argv[]) {
	// Initialise data
	Data data = {
		.num_trees = 0,
		.total_water = 0,
	};

	remove_headers();
	read_data(&data);
	stage1_output(&data);

	find_conflicting_trees(&data);
	stage2_output(&data);

	return 0;
}

// Tree functions
void initialise_tree(Tree *tree) {
	// Initialise particular members to their default value
	tree->num_conflicts = 0;
	tree->catchment_cells = 0;
	tree->is_alive = TRUE;
}

void print_tree_data(Tree *tree) {
	printf("==== Tree at %p\n", tree, sizeof(Tree));
	printf("tree->label  = %c\n", tree->label);
	printf("tree->xloc   = %f\n", tree->xloc);
	printf("tree->yloc   = %f\n", tree->yloc);
	printf("tree->litres = %d\n", tree->litres);
	printf("tree->radius = %f\n\n", tree->radius);
}

void find_conflicting_trees(Data *data) {
	int processed, compared, num_trees = data->num_trees;
	double distance, tree_rad, other_rad;

	// Declare pointer to first tree in forest
	Tree *tree, *other;
	tree = data->forest;
	other = NULL;

	// Iterate through trees in forest to find conflicts for each
	for (processed = 0; processed < num_trees; processed++, tree++) {
		// Set comparison tree, iterate through other trees
		other = data->forest;
		for (compared = 0; compared < num_trees - 1; compared++, other++) {
			// If other tree is the same, skip to next tree
			if (tree->label == other->label) {
				continue;
			}
			// Compare distance between trees to their cumulative root radii
			tree_rad = tree->radius, other_rad = other->radius;
			distance = distance_between_trees(tree, other);
			if (distance <= (tree_rad + other_rad)) {
				// Add label of conflicting tree to conflicts list within tree
				tree->conflicts[tree->num_conflicts] = other->label;
				tree->num_conflicts++;
			}
		}
	}

}

double distance_between_trees(Tree* tree, Tree* other) {
	// Compute euclidean distance with Pythagoras theorem (hypotenuse length)
	double x1, y1, x2, y2, a2, b2;
	x1 = tree->xloc, y1 = tree->yloc;
	x2 = other->xloc, y2 = other->yloc;
	a2 = (x2 - x1) * (x2 - x1);
	b2 = (y2 - y1) * (y2 - y1);
	// Calculate dist = c, c2 = a2 + b2
	return sqrt(a2 + b2);
}

// Output functions
void stage1_output(Data *data) {
	double megalitres = (double) (data->total_water) / 1e6;
	printf("S1: total data lines   = %5d trees\n", data->num_trees);
	printf("S1: total water needed = %5.3f megalitres\n\n", megalitres);
}

void stage2_output(Data *data) {
	int processed, con_tree, conflicts, num_trees = data->num_trees;
	Tree *tree;
	tree = data->forest;

	// Process all trees stored in data
	for (processed = 0; processed < num_trees; tree++, processed++) {
		// Print all labels stored in tree's array of conflicts
		conflicts = tree->num_conflicts;
		printf("S2: tree %c is in conflict with ", tree->label);
		for(con_tree = 0; con_tree < conflicts; con_tree++) {
			printf("%c ", tree->conflicts[con_tree]);
		}
		printf("\n");
	}
	printf("\n");
}

// Input handlers
void read_data(Data *data) {
	// Input variables for table data
	char label;
	double xloc, yloc, rootrad;
	int litres;

	// Declare pointer to first tree in forest
	Tree* tree;
	tree = data->forest;

	while (scanf("%c%lf%lf%d%lf\n",
				 &label, &xloc, &yloc, &litres, &rootrad) == 5) {
					// Read input info into tree structure
					tree->label = label;
					tree->xloc = xloc;
					tree->yloc = yloc;
					tree->litres = litres;
					tree->radius = rootrad;
					//print_tree_data(tree);

					// Assign all other tree attributes to their defaults
					initialise_tree(tree);

					// Update data info about entire forest, and point to next tree
					data->total_water += litres;
					data->num_trees++;
					tree++;
	 }
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
