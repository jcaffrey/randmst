#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_WT 1000.0

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
        //printf("VERTEX ID : %i\n", vertices[i]->vertex);

    }

    return vertices;
}


// only call this on dimension 2, 3, 4
double calcEuclidian(node_p* vertices, int idx_1, int idx_2, int dimension)
{
    node_p node_1 = vertices[idx_1];
    node_p node_2 = vertices[idx_2];
    double sumDiffSquare;
    sumDiffSquare = 0.0;
    if (dimension >= 2)
    {
        sumDiffSquare = pow(node_1->x - node_2->x, 2) + pow(node_1->y - node_2->y, 2);
    }
    if (dimension >= 3)
    {
        sumDiffSquare = pow(node_1->z - node_2->z, 2);
    }
    if (dimension == 4)
    {
        sumDiffSquare = pow(node_1->f - node_2->f, 2);
    }
    return sqrt(sumDiffSquare);
}

double minKey(double key[], bool mstSet[], int V)
{
   // Initialize min value
   double min = INT_MAX;
   double min_index;

   for (int v = 0; v < V; v++)
   {
       if (mstSet[v] == false && key[v] < min)
       {
           min = key[v];
           min_index = v;
       }

   }
   return min_index;
}

double primMST(node_p * vertices, int V, int dimension)
{
     double parent[V]; // Array to store constructed MST
     double key[V];   // Key values used to pick minimum weight edge in cut
     bool mstSet[V];  // To represent set of vertices not yet included in MST
     double tmp_wt;
     // Initialize all keys as INFINITE
     int i;
     for (i = 0; i < V; i++)
     {
         key[i] = INT_MAX;
         mstSet[i] = false;
     }

     key[0] = 0;     // Make key 0 so that this vertex is picked as first vertex
     parent[0] = -1; // First node is always root of MST

     for (int count = 0; count < V-1; count++)
     {

        int u = minKey(key, mstSet, V);
    //    printf("MIN KEY%i\n", u);

        mstSet[u] = true;


        for (int v = 0; v < V; v++)
        {
            tmp_wt = calcEuclidian(vertices, u, v, dimension);
            //printf("tmp_wt: %f\n", tmp_wt);
            if (mstSet[v] == false && tmp_wt <  key[v]) // graph[u][v] &&???
            {
                parent[v]  = u;
                key[v] = tmp_wt;
            }
        }

     }
     double totWt;
     totWt = 0.0;
     for(i = 0; i < V; i++)
     {
        //  printf("DISTANCE: %f\n", key[i]);
         totWt += key[i];
     }
     return totWt;
}

// double prim(node_p * vertices, int n, int dimension)
// {
//     printf("NUMPTS: %i\n", n);
//     double dist[n];
//     bool inMst[n];
//     double totWt;
//     totWt = 0.0;
//
//     int i, j;
//     for(i = 0; i < n; i++)
//     {
//         dist[i] = MAX_WT;
//         inMst[0] = false;
//     }
//     inMst[0] = true;
//     dist[0] = 0.0;
//
//
//
//
//     int best_i;
//     double best_wt;
//     double tmp_wt;
//
//     best_i = 0;
//
//     //
//     // for(i = 0; i < n; i++)
//     // {
//     //     inMst[best_i] = true;
//     //     printf("ADDING : %f\n", best_wt);
//     //     totWt += best_wt;
//     //     // best_wt = MAX_WT;
//     //
//     //
//     //     for(j = 0; j < n; j++)
//     //     {
//     //         tmp_wt = calcEuclidian(vertices, j, best_i, dimension);
//     //         printf("TMP_WT: %f < best_wt %f?\n", tmp_wt, best_wt);
//     //         if(inMst[j] == false && tmp_wt < best_wt)
//     //         {
//     //             best_wt = tmp_wt;
//     //             printf("UPDATING BEST_I: %i\n", best_i);
//     //             best_i = j;
//     //
//     //         }
//     //
//     //     }
//     // }
//
//     for(i = 0; i < n; i++)
//     {
//         inMst[best_i] = true;
//         //printf("ADDING : %f\n", dist[best_i]);
//         totWt += dist[best_i];
//
//         for(j = 0; j < n; j++)
//         {
//             tmp_wt = calcEuclidian(vertices, j, best_i, dimension);
//             //printf("TMP_WT: %f < DIST[J] %f?\n", tmp_wt, dist[j]);
//             if(dist[j] > tmp_wt)// && inMst[j] == false)
//             {
//                 dist[j] = tmp_wt;
//                 //printf("UPDATING BEST_I: %i\n", best_i);
//                 best_i = j;
//             }
//         }
//     }
//
//
//
//
//
//
//
//
//
//
//
//     // for(i = 0; i < n; i++)
//     // {
//     //     //totWt += dist[i];
//     //     printf("%f\n", dist[i]);
//     // }
//     return totWt;
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

        // for(ind = 0; ind < numpts; ind++)
        // {
        //     printf("VERTICES passed: %f\n", vertices[ind]->x);
        // }

        totWt += primMST(vertices, numpts, dimension);
        //destroyGraph(g);
    }
    printf("AVG WT: %f\n", totWt / (double) numtrials);

    return 0;
}

// TODO: MAKE PRIMS WORK FOR THE ZERO DIMENSION 
