#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>

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

/* Adjacency list */
typedef struct alist
{
    int E;           /*number of members in the list (for future use)*/
    node_t *head;      /*head of the adjacency linked list*/
} alist_t, *alist_p;

/* Graph structure. A graph is an array of adjacency lists.
   Size of array will be number of vertices in graph*/
typedef struct graph
{
    int V;         /*Number of vertices*/
    alist_p alistArr;     /*Adjacency lists' array*/
} graph_t, *graph_p;


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
graph_p createGraph(int n)
{
    int i;
    graph_p graph = (graph_p)malloc(sizeof(graph_t));
    if(!graph)
        printf("bad\n" );
    graph->V = n;

    graph->alistArr = (alist_p)malloc(n * sizeof(alist_t));
    if(!graph->alistArr)
        printf("bad\n" );

    for(i = 0; i < n; i++)
    {
        graph->alistArr[i].head = NULL;
        graph->alistArr[i].E = 0;
    }

    return graph;
}


double calcEuclidian(node_p src, node_p dest, int dimension) {
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

void addEdge(graph_t *graph, int src, int dest, int dimension)
{
    double dist;

    node_p n1 = createNode(src, dimension);
    node_p n2 = createNode(dest, dimension);
    if(dimension != 0)
    {
        dist = calcEuclidian(n1, n2, dimension);
        n2->wt = dist;
        n1->wt = dist;
    }

    n2->next = graph->alistArr[src].head;
    graph->alistArr[src].head = n2;
    graph->alistArr[src].E++;

    n1->next = graph->alistArr[dest].head;
    graph->alistArr[dest].head = n1;
    graph->alistArr[dest].E++;
}

void displayGraph(graph_p graph)
{
    int i;
    for (i = 0; i < graph->V; i++)
    {
        node_p alistPtr = graph->alistArr[i].head;
        printf("\n%d: ", i);
        while (alistPtr)
        {
            printf("%d->", alistPtr->vertex);
            alistPtr = alistPtr->next;
        }
        printf("NULL\n");
    }
    for (i = 0; i < graph->V; i++)
    {
        node_p alistPtr = graph->alistArr[i].head;
        printf("\n%d: ", i);
        while (alistPtr)
        {
            printf("%f->", alistPtr->wt);
            alistPtr = alistPtr->next;
        }
        printf("NULL\n");
    }
}

void populateGraph(graph_p g, int n, int dimension)
{
    int i, j;
    for(i = 0; i < n; i++)
    {
        for(j=0; j<i; j++)
        {
            if(i!=j)
            {
                addEdge(g, i, j, dimension);
            }
        }
    }

}

void destroyGraph(graph_p g)
{
    if(g)
    {
        if(g->alistArr)
        {
            int v;
            for (v = 0; v < g->V; v++)
            {
                node_p alistPtr = g->alistArr[v].head;
                while (alistPtr)
                {
                    node_p tmp = alistPtr;
                    alistPtr = alistPtr->next;
                    free(tmp);
                }
            }
            free(g->alistArr);
        }
        free(g);
    }
}

int main(int argc, char* argv[]) {
	srand((unsigned) time(NULL));
    rand();

	if (argc != 5) {
		printf("usage: ./randmst 0 numpoints numtrials dimension\n");
		return 1;
	}

	int dimension = atoi(argv[4]);
    if (dimension != 0 && dimension != 2 && dimension != 3 && dimension != 4)
    {
        printf("Please enter valid dimensions (ie. 0, 2, 3, 4)\n");
        return 1;
    }

    int n;
    n = 10;  // 10k took 8 seconds
    graph_p g = createGraph(n);
    populateGraph(g, n, dimension);
    displayGraph(g);

    destroyGraph(g);

    return 0;
}
