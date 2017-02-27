#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_WT 1000.0

typedef struct node
{
    int vertex;
    struct node *next;
    double wt;
    double x;
    double y;
    double z;
    double f;
} node_t, *node_p;


// stores wt or coordinates for node and assigns vertex ID
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

        vertices[i] = newNode;
    }

    return vertices;
}


// only call on dimension 2, 3, 4
double calcEuclidian(node_p* vertices, int idx_1, int idx_2, int dimension)
{
    node_p node_1 = vertices[idx_1];
    node_p node_2 = vertices[idx_2];
    double sumDiffSquare;
    sumDiffSquare = 0.0;
    if (dimension == 0)
    {
        return fabs(node_1->wt - node_2->wt);
    }
    if (dimension >= 2)
    {
        sumDiffSquare = pow(node_1->x - node_2->x, 2) + pow(node_1->y - node_2->y, 2);
    }
    if (dimension >= 3)
    {
        sumDiffSquare += pow(node_1->z - node_2->z, 2);
    }
    if (dimension == 4)
    {
        sumDiffSquare += pow(node_1->f - node_2->f, 2);
    }
    return sqrt(sumDiffSquare);
}

double mstWtOnRand(int V)
{
    double dist_keys[V];
    double dist_wts[V];
    bool in_mst[V];
    int i, u, v, min_key;
    double tmp_wt, tot_wt, min_wt;

    for(i = 0; i < V; i++)
    {
        dist_wts[i] = INT_MAX;
        in_mst[i] = false;
    }

    dist_wts[0] = 0;
    dist_keys[0] = 0;

    min_key = 0;
    for(i = 0; i < V - 1; i++)
    {
        min_wt = INT_MAX;
        for(u = 0; u < V; u++)
        {
            if (in_mst[u] == false && dist_wts[u] < min_wt)
            {
                min_wt = dist_wts[u];
                min_key = u;
            }
        }
        in_mst[min_key] = true;
        for(v = 0; v < V; v++)
        {
            // tmp_wt = calcEuclidian(vertices, min_key, v, dimension);
            if(in_mst[v] == false && tmp_wt < dist_wts[v])
            {
                dist_keys[v] = min_key;
                dist_wts[v] = tmp_wt;
            }
        }
        // ...going for one loop...
        // for (u = 0; u < V; u++)
        // {
        //     if(in_mst[u] == false)
        // }
    }
    tot_wt = 0.0;
    for(i = 0; i < V; i++)
    {
        tot_wt += dist_wts[i];
    }
    return tot_wt;

}

double mstWtOnGrid(node_p * vertices, int V, int dimension)
{
    double dist_keys[V];
    double dist_wts[V];
    bool in_mst[V];
    int i, u, v, min_key;
    double tmp_wt, tot_wt, min_wt;

    for(i = 0; i < V; i++)
    {
        dist_wts[i] = INT_MAX;
        in_mst[i] = false;
    }

    dist_wts[0] = 0;
    dist_keys[0] = 0;

    min_key = 0;
    for(i = 0; i < V - 1; i++)
    {
        min_wt = INT_MAX;
        for(u = 0; u < V; u++)
        {
            if (in_mst[u] == false && dist_wts[u] < min_wt)
            {
                min_wt = dist_wts[u];
                min_key = u;
            }
        }
        in_mst[min_key] = true;
        for(v = 0; v < V; v++)
        {
            tmp_wt = calcEuclidian(vertices, min_key, v, dimension);
            if(in_mst[v] == false && tmp_wt < dist_wts[v])
            {
                dist_keys[v] = min_key;
                dist_wts[v] = tmp_wt;
            }
        }
        // ...going for one loop...
        // for (u = 0; u < V; u++)
        // {
        //     if(in_mst[u] == false)
        // }
    }
    tot_wt = 0.0;
    for(i = 0; i < V; i++)
    {
        tot_wt += dist_wts[i];
    }
    return tot_wt;

}

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
    if (numpts > 131073)
    {
        printf("values of this magnitude have not been tested. proceed at own peril\n");
        return 1;
    }

    int i;
    double totWt;
    totWt = 0.0;

    for(i = 0; i < numtrials; i++)
    {
        if (dimension == 0)
        {
            totWt += mstWtOnRand(numpts);
        }
        else
        {
            node_p * vertices = createArrOfNodes(numpts, dimension);

            totWt += mstWtOnGrid(vertices, numpts, dimension);
        }

    }
    printf("AVG WT: %f\n", totWt / (double) numtrials);

    return 0;
}

// TODO: MAKE PRIMS WORK FOR THE ZERO DIMENSION
