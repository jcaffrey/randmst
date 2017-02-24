
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
        int x;
        int y;
        int z;
        int w;
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
void addEdge(Graph g, int src, int dest, int dimension);
double calculateEdgeWeight(Graph g, int src, int dimension);



int main(int argc, char* argv[]) {
	time_t t; // to pass to srand to seed..
	srand((unsigned) time(&t));
	if (argc != 5) {
		printf("usage: ./randmst 0 numpoints numtrials dimension\n");
		return 1;
	}
	for (int i = 0; i < argc; i++) {
		printf(" argv of %i is %s\n", i, argv[i]);
	}
	int dimension = atoi(argv[4]);
    printf("%i\n", dimension);
    Graph e = generateGraph(10, dimension);
    addEdge(e, 1, 2, dimension);
    // graph_create(10);
	// if(dimension == 0)
	// 	populateGraph0(512);
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

void addEdge(Graph g, int src, int dest, int dimension)
{
    if (dimension == 0)
        printf("dimesnio is zero! : %i\n", dimension);

    while(g->vertices[src]->numNbrs >= g->vertices[src]->len)
    {
        g->vertices[src]->len *= 2;
        g->vertices[src] = realloc(g->vertices[src], sizeof(struct edge) * (g->vertices[src]->len - 1) + sizeof(struct adjList));
    }

    g->vertices[src]->edges[g->vertices[src]->numNbrs].weightFromP = calculateEdgeWeight(g, src, dimension);
    g->vertices[src]->edges[g->vertices[src]->numNbrs].nbrId = dest;
    g->vertices[src]->numNbrs++;
    // mark as not sorted?
    //g->edges[src]->numNbrs += 1;
    // g's edge count++?
    return;
}

double calculateEdgeWeight(Graph g, int src, int dimension) {
    if (dimension == 0) {
        double newX;
        double newY;
        double test;
        int r;
        int newR;
        rand();
        newR = rand();
        printf("RAND() IS : %i\n", r);
        printf("newR IS : %i\n", newR);

        newX = (double) newR / (double) RAND_MAX;
        // newY = (double) rand() % 1.0;
        // newY = rand();
        // test = rand();

        printf("newX IS : %f\n", newX);
        // printf("newY: %f\n", newY);
        // printf("test: %f\n", test);

    }
    return 0.5;
}
