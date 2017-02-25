
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>

struct graph {
    int n;
    int dimension; // 0, 2, 3, 4
    struct adjList {
        int vertexId;
        int numNbrs;
        int len;        /* number of slots in array */
        double x;
        double y;
        double z;
        double w;
        struct edge {
            int nbrId;
            int weightFromP;
            // int sink;           /* other end of edge */
            // int weight;         /* used for shortest-path algorithms */
        } edges[1];    /* actual list of successors */
    } *vertices[1];
};

typedef struct graph *Graph;

//void populateGraph0(int n);  // implemented as adjacency list with dynamically resizing array to store edges
Graph generateGraph(int n, int dimension);
Graph populateGraph(Graph g, int n, int dimension);
void printGraph(Graph g);

void addEdge(Graph g, int src, int dest, int dimension);
double calculateNewEdgeWeight(Graph g, int src, int dimension);



int main(int argc, char* argv[]) {
	srand((unsigned) time(NULL));
    rand();

	if (argc != 5) {
		printf("usage: ./randmst 0 numpoints numtrials dimension\n");
		return 1;
	}

	int dimension = atoi(argv[4]);
    Graph g = generateGraph(10, dimension);
    populateGraph(g, 10, dimension);
    printGraph(g);

}

// create vertices of graph
Graph generateGraph(int n, int dimension)
{
    Graph g;
    int i;

    g = malloc(sizeof(struct graph) + sizeof(struct adjList *) * n - 1);
    assert(g);

    g->n = n;

    for(i = 0; i < n; i++) {
        g->vertices[i] = malloc(sizeof(struct adjList));
        assert(g->vertices[i]);

        g->vertices[i]->vertexId = i;
        g->vertices[i]->numNbrs = 0;
        g->vertices[i]->len = 1;
        // keep this sorted?
    }
    return g;
}

// add n-1 edges for each vertex in graph
Graph populateGraph(Graph g, int n, int dimension)
{
    int i, j;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < i; j++)
        {
            if(i != j)
                addEdge(g, i, j, dimension);
        }
    }
    return g;
}

void addEdge(Graph g, int src, int dest, int dimension)
{
    // while(g->vertices[src]->numNbrs >= g->vertices[src]->len)
    // {
    //     g->vertices[src]->len *= 2;
    //     g->vertices[src] = realloc(g->vertices[src], sizeof(struct edge) * (g->vertices[src]->len - 1) + sizeof(struct adjList));
    // }

    if (dimension == 2)
    {
        double vertX;
        double vertY;
        vertX = (double) rand() / (double) RAND_MAX;
        vertY = (double) rand() / (double) RAND_MAX;
        g->vertices[src]->x = vertX;
        g->vertices[src]->y = vertY;
    }

    g->vertices[src]->edges[g->vertices[src]->numNbrs].weightFromP = calculateNewEdgeWeight(g, src, dimension);
    g->vertices[src]->edges[g->vertices[src]->numNbrs].nbrId = dest;
    g->vertices[src]->numNbrs++;
    // mark as not sorted?
    //g->edges[src]->numNbrs += 1;
    // g's edge count++?
    return;
}

double calculateNewEdgeWeight(Graph g, int src, int dimension) {
    if (dimension == 2) {
        double newX;
        double newY;
        double dist;
        newX = (double) rand() / (double) RAND_MAX;
        newY = (double) rand() / (double) RAND_MAX;
        dist = sqrt(pow(g->vertices[src]->x - newX, 2) + pow(g->vertices[src]->y - newY, 2));
        return dist;
    }
    return 0;
}

void printGraph(Graph g)
{
    int i, j;
    for(i = 0; i < g->n; i++)
    {
        for(j = 0; j < g->n; j++)
        {
            printf("VERTEX IS: %i\n", g->vertices[i]->vertexId);
            printf("EDGE IS: %i\n", g->vertices[i]->edges[j].nbrId);
            //printf("VERTEX COORDINATES (X, Y): %i\n", g->vertices[i]->vertexId);

        }
    }
    return;
    // for(i = 0; i < g->n; i++)

}
