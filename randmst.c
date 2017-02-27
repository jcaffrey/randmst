#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <limits.h>

#define MAX_WT 10.0

/* Adjacency list node*/
typedef struct node
{
    int vertex;                /*Index to adjacency list array*/
    struct node *next; /*Pointer to the next node*/
    double wt;
    double x;
    double y;
    double z;
    double f;
} node_t, *node_p;


node_p createNode(int v, int dimension)
{
    node_p newNode = (node_p)malloc(sizeof(node_t));
    if(!newNode)
        printf("bad\n" );
    if(dimension == 0)
    {
        newNode->wt = (double) rand() / (double) RAND_MAX;
    }

    if (dimension >= 2)
    {
        newNode->x = (double) rand() / (double) RAND_MAX;
        newNode->y = (double) rand() / (double) RAND_MAX;
    }
    if (dimension >= 3)
    {
        newNode->z = (double) rand() / (double) RAND_MAX;
    }
    if (dimension == 4)
    {
        newNode->f = (double) rand() / (double) RAND_MAX;
    }

    newNode->vertex = v;
    newNode->next = NULL;

    return newNode;
}

// instantiate vertices
node_p* createArrOfNodes(int n, int dimension)
{
    node_p * vertices;
    vertices = malloc(n * sizeof(node_p));

    int i, idx;
    node_p newNode;
    for(i = 0; i < n; i++)
    {
        newNode = createNode(i, dimension);
        //printf("%f\n", newNode->x);
        vertices[i] = newNode;
        // idx += sizeof(node_p);
        // vertices[i] = newNode;
        printf("VERTEX ID : %i\n", vertices[i]->vertex);

    }

    return vertices;
}



double calcEuclidian(node_p src, node_p dest, int dimension)
{
    double sumDiffSquare;
    if (dimension >= 2)
    {
        sumDiffSquare = pow(src->x - dest->x, 2) + pow(src->y - dest->y, 2);
    }
    if (dimension >= 3)
    {
        sumDiffSquare = pow(src->z - dest->z, 2);
    }
    if (dimension == 4)
    {
        sumDiffSquare = pow(src->f - dest->f, 2);
    }
    return sqrt(sumDiffSquare);
}



// void destroyGraph(graph_p g)
// {
//     if(g)
//     {
//         if(g->alistArr)
//         {
//             int v;
//             for (v = 0; v < g->V; v++)
//             {
//                 node_p alistPtr = g->alistArr[v].head;
//                 while (alistPtr)
//                 {
//                     node_p tmp = alistPtr;
//                     alistPtr = alistPtr->next;
//                     free(tmp);
//                 }
//             }
//             free(g->alistArr);
//         }
//         free(g);
//     }
// }



int main(int argc, char* argv[])
{
	srand((unsigned) time(NULL));
    rand();

	if (argc != 5) {
		printf("usage: ./randmst 0 numpoints numtrials dimension\n");
		return 1;
	}
    int numpts, numtrials, dimension;
    numpts = atoi(argv[2]);
    numtrials = atoi(argv[3]);
	dimension = atoi(argv[4]);
    if (dimension != 0 && dimension != 2 && dimension != 3 && dimension != 4)
    {
        printf("Please enter valid dimensions (ie. 0, 2, 3, 4)\n");
        return 1;
    }

    int i, ind;
    double totWt;
    totWt = 0.0;
    //node_p vertices[numpts];
    for(i = 0; i < numtrials; i++)
    {
        node_p * vertices = createArrOfNodes(numpts, dimension);
        for(ind = 0; ind < numpts; ind++)
        {
            printf("VERTICES passed: %f\n", vertices[ind]->x);
        }
        printf("vertices[0]->x: %f\n", vertices[0]->x);
        //displayGraph(g);
        //totWt += prim(g, dimension);
        //destroyGraph(g);
    }
    printf("AVG WT: %f\n", totWt / (double) numtrials);

    return 0;
}
