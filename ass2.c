/*
COMP20005 2018 S1
Assignment 2
Jack Zezula
04/05/2018 - 10/05/2018
*/
// programming is fun

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_TREES 52
#define HEADER_LINES 1
#define TRUE 1
#define FALSE 0
#define MEGA 1.0e6

#define REG_HEIGHT 60
#define GRID_HEIGHT REG_HEIGHT/CELL_HEIGHT
#define GRID_WIDTH 70
#define CELL_HEIGHT 2
#define CELL_WIDTH 1
#define CELL_AREA CELL_HEIGHT * CELL_WIDTH

typedef struct {
	// Stage 1 vars
	char label;
	double xloc, yloc, litres, radius;

	// Stage 2
	int num_conflicts;
	char conflicts[MAX_TREES - 1];

	//Stages 3 & 4
	int catchment_cells;
	int is_alive;
} Tree;

/*
A comment about the manipulation of the Trees in Forest:
	In this program, I frequently assign pointers to trees of interest.
	This is because I think that:
		tree->label
	looks much nicer than:
		forest[tree_index].label
*/
typedef Tree Forest[MAX_TREES];

typedef char Grid[GRID_WIDTH + 1][GRID_HEIGHT + 1];

// Input handlers
void read_data(Forest forest, int *num_trees, double *total_water);
void remove_headers();
int mygetchar();

// Tree functions
void initialise_tree(Tree *tree);
void find_conflicting_trees(Forest forest, int num_trees);
double distance(double x1, double y1, double x2, double y2);
double distance_between_trees(Tree *tree, Tree *other);
double find_stress_factor(Tree *tree, double rainfall);

// Grid functions
void initialise_grid(Grid grid);
void print_grid(Grid grid, int stage);
void print_xaxis();
void calculate_grid(Grid grid, Forest forest, int num_trees);
char cell_tree(double x_cent, double y_cent, Forest forest, int num_trees);

// Output functions
void stage1_output(Forest forest, int num_trees, int total_water);
void stage2_output(Forest forest, int num_trees);
void stage3_output(Grid grid, Forest forest, int num_trees);
void stage4_output(Grid grid, Forest forest, int num_trees, double rainfall);

// Main function
int
main(int argc, char *argv[]) {
	int num_trees = 0;
	double total_water = 0;
	double rainfall = atof(argv[1]);
	Forest forest;
	Grid grid;

	remove_headers();
	read_data(forest, &num_trees, &total_water);
	stage1_output(forest, num_trees, total_water);

	find_conflicting_trees(forest, num_trees);
	stage2_output(forest, num_trees);

	initialise_grid(grid);
	stage3_output(grid, forest, num_trees);

	stage4_output(grid, forest, num_trees, rainfall);

	return 0;
}

// Input handlers
void read_data(Forest forest, int *num_trees, double *total_water) {
	char label;
	double xloc, yloc, litres, rootrad;
	Tree *tree = NULL;

	while (scanf("%c%lf%lf%lf%lf",
	&label, &xloc, &yloc, &litres, &rootrad) == 5) {
		// Use pointer to tree for easier assigment of properties during input read
		tree = &forest[*num_trees];
		tree->label = label;
		tree->xloc = xloc;
		tree->yloc = yloc;
		tree->litres = litres;
		tree->radius = rootrad;

		// Assign all other tree attributes to their defaults
		initialise_tree(tree);

		// Update total water and tree count, and point to next tree
		*total_water += litres;
		*num_trees += 1;
		tree++;

		// Clear line remainder to prime buffer for scanning the next tree
		remove_headers();
	}
}

/* remove_headers by Jack Zezula (ME), taken from:
/storage3/beta/students/j/jzezula/COMP20005/Projects/Ass1/ass1.c
*/
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

/* mygetchar by Alistair Moffat, taken from:
https://people.eng.unimelb.edu.au/ammoffat/teaching/20005/ass2/
*/
int	mygetchar() {
	int c;
	while ((c=getchar())=='\r');
	return c;
}

// Tree functions
void initialise_tree(Tree *tree) {
	// Initialise tree properties to default values
	tree->num_conflicts = 0;
	tree->catchment_cells = 0;
	tree->is_alive = TRUE;
}

void find_conflicting_trees(Forest forest, int num_trees) {
	int processed, compared;
	double distance, tree_rad, other_rad;

	// Declare pointers to trees in forest
	Tree *tree = forest, *other = NULL;

	// Iterate through trees in forest to find conflicts for each
	for (processed = 0; processed < num_trees; processed++) {
		tree = &forest[processed];
		// For each processed tree, compare every other tree for distance
		for (compared = 0; compared < num_trees; compared++) {
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

double distance(double x1, double y1, double x2, double y2) {
	double a2, b2;
	// Compute euclidean distance with Pythagoras theorem (hypotenuse length)
	a2 = (x2 - x1) * (x2 - x1);
	b2 = (y2 - y1) * (y2 - y1);
	// Calculate dist = c, c2 = a2 + b2
	return sqrt(a2 + b2);
}

double distance_between_trees(Tree* tree, Tree* other) {
	double x1, y1, x2, y2;
	x1 = tree->xloc, y1 = tree->yloc;
	x2 = other->xloc, y2 = other->yloc;
	return distance(x1, y1, x2, y2);
}

double find_stress_factor(Tree *tree, double rainfall) {
	double stress_factor, water_needed, water_available;

	// Find stress factor and reset tree catchment cells to zero
	water_needed = tree->litres;
	water_available = CELL_AREA * rainfall * tree->catchment_cells;
	stress_factor = water_needed / water_available;
	tree->catchment_cells = 0;

	return stress_factor;
}

// Grid functions
void initialise_grid(Grid grid) {
	int row, col;
	// Set top and right boundary of grid to blank (will never contain trees)
	for (row = 0; row <= GRID_HEIGHT; row++) {
		grid[GRID_WIDTH][row] = ' ';
	}
	for (col = 0; col <= GRID_WIDTH; col++) {
		grid[col][GRID_HEIGHT] = ' ';
	}
}

void calculate_grid(Grid grid, Forest forest, int num_trees) {
	int row, col;
	double x, y, x_cent, y_cent;

	// Iterate through grid, exclude final row/col points
	for (row = 0; row < GRID_HEIGHT; row++) {
		for (col = 0; col < GRID_WIDTH; col++) {
			// Assign cell centre coords, divide by 2.0 to find midpoint
			x = (double) col;
			y = (double) row*CELL_HEIGHT;
			x_cent = x + CELL_WIDTH/2.0;
			y_cent = y + CELL_HEIGHT/2.0;

			// Find tree with catchment over point centre
			grid[col][row] = cell_tree(x_cent, y_cent, forest, num_trees);
		}
	}
}

char cell_tree(double x_cent, double y_cent, Forest forest, int num_trees) {
	double min_dist, dist_to_tree;
	int processed;
	char cell_label = ' ';
	Tree *tree = NULL, *cell_tree = NULL;

	// Search forest for closest tree with catchment over cell
	// Default distance is max distance possible within grid
	min_dist = distance(0, 0, GRID_WIDTH, REG_HEIGHT);
	for (processed = 0; processed < num_trees; processed++) {
		tree = &forest[processed];
		dist_to_tree = distance(x_cent, y_cent, tree->xloc, tree->yloc);

		// Update cell label if tree has catchment, is closest yet, and still alive
		if (dist_to_tree <= tree->radius && dist_to_tree < min_dist
			&& tree->is_alive) {
				min_dist = dist_to_tree;
				cell_label = tree->label;
				cell_tree = tree;
			}
		}
		// Add to tree catchment cell count, return label
		if (cell_tree) {
			cell_tree->catchment_cells++;
		}
		return cell_label;
	}

void print_grid(Grid grid, int stage) {
		int row, col;

		for (row = GRID_HEIGHT; row >= 0; row--) {
			// Print axis every 10 units (depends on cell height)
			if (row % (10 / CELL_HEIGHT) == 0) {
				printf("S%d:%3d +", stage, CELL_HEIGHT*row);
			} else {
				printf("S%d:    |", stage);
			}

			// Print grid values
			for (col = 0; col <= GRID_WIDTH; col++) {
				printf("%c", grid[col][row]);
			}
			printf("\n");
		}

		// Print bottom axis
		print_xaxis(stage);
		printf("\n\n");
	}

void print_xaxis(int stage) {
		// In this function, assume x-axis will always be marked + every 10 points
		int col;

		// Axis symbols
		printf("S%d:     ", stage);
		for (col = 0; col <= GRID_WIDTH; col++) {
			if (col % 10 == 0) {
				printf("+");
			} else {
				printf("-");
			}
		}

		// Axis numbers
		printf("\n"
		"S%d:     0", stage);
		for (col = 10; col <= GRID_WIDTH; col+=10) {
			printf("%10d", col);
		}
}

// Output functions
void stage1_output(Forest forest, int num_trees, int total_water) {
	double megalitres = (double) (total_water) / MEGA;
	printf("\n");
	printf("S1: total data lines   = %5d trees\n", num_trees);
	printf("S1: total water needed = %5.3f megaliters per year\n\n", megalitres);
}

void stage2_output(Forest forest, int num_trees) {
	int processed, conf_tree, conflicts;
	Tree *tree = NULL;

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

void stage3_output(Grid grid, Forest forest, int num_trees) {
	calculate_grid(grid, forest, num_trees);
	print_grid(grid, 3);
	printf("\n");
}

void stage4_output(Grid grid, Forest forest, int num_trees, double rainfall) {
	int processed, tree_has_died = TRUE;
	double stress_factor, max_stress_factor;
	Tree* tree = NULL, *dead_tree = NULL;

	printf("S4: rainfall amount = %.1f\n", rainfall);
	while (tree_has_died && rainfall) {
		// Set default conditions
		tree_has_died = FALSE;
		dead_tree = NULL;
		max_stress_factor = 1.0;
		for (processed = 0; processed < num_trees; processed++) {
			tree = &forest[processed];
			// Find stress factor for tree, and reset cells for next calculation
			stress_factor = find_stress_factor(tree, rainfall);

			// Record tree with highest stress factor
			if (stress_factor > max_stress_factor && tree->is_alive) {
				tree_has_died = TRUE;
				max_stress_factor = stress_factor;
				dead_tree = tree;
			}
		}
		if (tree_has_died) {
			// Announce dead tree, recalculate survivors' catchment cells
			dead_tree->is_alive = 0;
			printf("S4: tree %c has stress factor %.2f and dies next\n",
						 dead_tree->label, max_stress_factor);
 		  calculate_grid(grid, forest, num_trees);
		}
	}
	// Print final map when no further trees will die
	printf("\n");
	print_grid(grid, 4);
}
