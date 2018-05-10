/*
COMP20005 2018 S1
Assignment 2
Jack Zezula
04/05/2018 - ????
*/
// programming is fun

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
	double xloc, yloc, litres, radius;

	// Stage 2
	int num_conflicts;
	char conflicts[MAX_TREES - 1];

	//Stage 3 & 4
	int catchment_cells;
	int is_alive;
} Tree;


/*
A comment about the manipulation of the Trees in Forest:
	In this program, I have chosen to assign pointers to trees of interest.
	This is because I think that:
		tree->label
	looks much nicer than:
		forest[tree_index].label
*/
typedef Tree Forest[MAX_TREES];


// Tree functions
void initialise_tree(Tree *tree);
void print_tree_data(Tree *tree);
void find_conflicting_trees(Forest forest, int num_trees);
double distance_between_trees(Tree *tree, Tree *other);

// Input handlers
void read_data(Forest forest, int *num_trees, double *total_water);
void remove_headers();
int mygetchar();

// Output functions
void stage1_output(Forest forest, int num_trees, int total_water);
void stage2_output(Forest forest, int num_trees);

int
main(int argc, char *argv[]) {
	// Initialise data
	Forest forest;
	int num_trees = 0;
	double total_water = 0;

	remove_headers();
	read_data(forest, &num_trees, &total_water);
	stage1_output(forest, num_trees, total_water);

	find_conflicting_trees(forest, num_trees);
	stage2_output(forest, num_trees);

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
	printf("==== Tree at %p (%d bytes)\n", tree, sizeof(*tree));
	printf("tree->label  = %c\n", tree->label);
	printf("tree->xloc   = %f\n", tree->xloc);
	printf("tree->yloc   = %f\n", tree->yloc);
	printf("tree->litres = %f\n", tree->litres);
	printf("tree->radius = %f\n\n", tree->radius);
}

void find_conflicting_trees(Forest forest, int num_trees) {
	int processed, compared;
	double distance, tree_rad, other_rad;

	// Declare pointers to trees in forest
	Tree *tree = forest, *other;

	// Iterate through trees in forest to find conflicts for each
	for (processed = 0; processed < num_trees; processed++, tree++) {
		// For each processed tree, compare every other tree for distance
		for (compared = 0; compared < num_trees; compared++, other++) {
			other = &forest[compared];
			// If compared trees are the same, skip to next tree
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
void stage1_output(Forest forest, int num_trees, int total_water) {
	double megalitres = (double) (total_water) / 1e6;
	printf("S1: total data lines   = %5d trees\n", num_trees);
	printf("S1: total water needed = %5.3f megalitres\n\n", megalitres);
}

void stage2_output(Forest forest, int num_trees) {
	// Counter variable and pointer declaration
	int processed, conf_tree, conflicts;
	Tree *tree;

	// Process all trees stored in data
	for (processed = 0; processed < num_trees; tree++, processed++) {
		// Use pointer to tree for easy assignment of tree properties
		tree = &forest[processed];
		// Print all labels stored in tree's array of conflicts
		conflicts = tree->num_conflicts;
		printf("S2: tree %c is in conflict with ", tree->label);
		for(conf_tree = 0; conf_tree < conflicts; conf_tree++) {
			printf("%c ", tree->conflicts[conf_tree]);
		}
		printf("\n");
	}
	printf("\n");
}

// Input handlers
void read_data(Forest forest, int *num_trees, double *total_water) {
	// Input variables for table data
	char label;
	double xloc, yloc, litres, rootrad;

	// Use pointer to tree for easier assigment of properties during input read
	Tree *tree = forest;
	while (scanf(" %c %lf %lf %lf %lf\n",
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

					// Update total water and tree count, and point to next tree
					*total_water += litres;
					*num_trees += 1;
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
