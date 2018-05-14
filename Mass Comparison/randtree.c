/* This program generates "random" test file in tsv for ass2.
Author: Tsz Kiu Pang
Date:   13 May 2018
*/

#include <stdio.h>
#include <stdlib.h>

#define XLIM 70
#define YLIM 60

#define MAXLIT 70000
#define MINLIT 0

#define MAXRAD 20
#define MINRAD 2

#define MAXTREES 25

#define MAXALPHA 52

#define NONALPHA 7

int
main(int argc, char *argv[]){
    int i, j, seed, ntrees;
    char tree;
    double xloc, yloc;
    int liters;
    double rootrad;
    int duplicate;

    char prev[MAXTREES];

    if(argc!=2){
        printf("Usage  : %s <seed here>\n", argv[0]);
        printf("Example: %s 1293879\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    seed = atoi(argv[1]);
    srand(seed);
    ntrees = rand()%MAXTREES;

    printf("tree\txloc\tyloc\tliters\trootrad\n");

    for(i=0; i<ntrees; i++){

    	/* make sure no label is duplicated */
    	duplicate = 1;
        while(duplicate){
            tree = 'A' + rand()%(MAXALPHA-1);
            if(tree>'Z')
                tree += NONALPHA;
            duplicate = 0;
            for(j=0; j<i; j++){
                if(tree==prev[j])
                    duplicate = 1;
            }
        }
        prev[i] = tree;

        /* make sure there is room for the root*/
        xloc = rand()%((XLIM-MINRAD*2)*10)/10.0 + MINRAD;
        yloc = rand()%((YLIM-MINRAD*2)*10)/10.0 + MINRAD;

        liters = rand()%(MAXLIT-MINLIT)+MINLIT;
        rootrad = rand()%((MAXRAD-MINRAD)*10)/10.0+MINRAD;

        /* make sure the root does not go out of the graph */
        while(xloc+rootrad>XLIM
                || xloc-rootrad<0
                || yloc+rootrad>YLIM
                || yloc-rootrad<0){
            rootrad = rand()%((MAXRAD-MINRAD)*10)/10.0+MINRAD;
        }

        printf("%c\t%.1f\t%.1f\t%d\t%.1f\n", tree, xloc, yloc, liters, rootrad);
    }
    return 0;
}
