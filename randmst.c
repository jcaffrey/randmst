#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>


/* Adjacency list node*/
typedef struct adjlist_node
{
    int vertex;                /*Index to adjacency list array*/
    struct adjlist_node *next; /*Pointer to the next node*/
    double wt;
    double x;
    double y;
    // double z;
    // double f;
} adjlist_node_t, *adjlist_node_p;

/* Adjacency list */
typedef struct adjlist
{
    int num_members;           /*number of members in the list (for future use)*/
    adjlist_node_t *head;      /*head of the adjacency linked list*/
} adjlist_t, *adjlist_p;

/* Graph structure. A graph is an array of adjacency lists.
   Size of array will be number of vertices in graph*/
typedef struct graph
{
    int num_vertices;         /*Number of vertices*/
    adjlist_p adjListArr;     /*Adjacency lists' array*/
} graph_t, *graph_p;


adjlist_node_p createNode(int v, int dimension)
{
    adjlist_node_p newNode = (adjlist_node_p)malloc(sizeof(adjlist_node_t));
    if(!newNode)
        printf("bad\n" );
    if(dimension == 0)
    {
        newNode->wt = (double) rand() / (double) RAND_MAX;
    }

    if (dimension == 2)
    {
        double x;
        double y;
        x = (double) rand() / (double) RAND_MAX;
        y = (double) rand() / (double) RAND_MAX;
        // g->adjListArr[v]->x = vertX;
        // g->adjListArr[v]->y = vertY;
        newNode->x = x;
        newNode->y = y;
    }
    newNode->vertex = v;
    newNode->next = NULL;

    return newNode;
}

/* Function to create a graph with n vertices */
graph_p createGraph(int n)
{
    int i;
    graph_p graph = (graph_p)malloc(sizeof(graph_t));
    if(!graph)
        printf("bad\n" );
    graph->num_vertices = n;


    /* Create an array of adjacency lists*/
    graph->adjListArr = (adjlist_p)malloc(n * sizeof(adjlist_t));
    if(!graph->adjListArr)
        printf("bad\n" );

    for(i = 0; i < n; i++)
    {
        graph->adjListArr[i].head = NULL;
        graph->adjListArr[i].num_members = 0;
    }

    return graph;
}

/*Destroys the graph*/
void destroyGraph(graph_p graph)
{
    if(graph)
    {
        if(graph->adjListArr)
        {
            int v;
            /*Free up the nodes*/
            for (v = 0; v < graph->num_vertices; v++)
            {
                adjlist_node_p adjListPtr = graph->adjListArr[v].head;
                while (adjListPtr)
                {
                    adjlist_node_p tmp = adjListPtr;
                    adjListPtr = adjListPtr->next;
                    free(tmp);
                }
            }
            /*Free the adjacency list array*/
            free(graph->adjListArr);
        }
        /*Free the graph*/
        free(graph);
    }
}

double calcNewEdgeWt(adjlist_node_p src, adjlist_node_p dest, int dimension) {
    double dist;
    if (dimension == 2) {
        printf("%i\n", src->vertex);
        dist = sqrt(pow(src->x - dest->x, 2) + pow(src->y - dest->y, 2));
        return dist;
    }
    return 0;
}

/* Adds an edge to a graph*/
void addEdge(graph_t *graph, int src, int dest, int dimension)
{
    double dist;
    /* Add an edge from src to dst in the adjacency list*/
    adjlist_node_p n1 = createNode(src, dimension);
    printf("N1 V:  %i\n", n1->vertex);

    adjlist_node_p n2 = createNode(dest, dimension);
    printf("N2 V:  %i\n", n2->vertex);
    if(dimension == 2)
    {
        dist = calcNewEdgeWt(n1, n2, dimension);
        n2->wt = dist;
        n1->wt = dist;
    }

    n2->next = graph->adjListArr[src].head;
    graph->adjListArr[src].head = n2;
    graph->adjListArr[src].num_members++;
    //graph->adjListArr[src].wt = dist;

    /* Add an edge from dest to src also*/
    n1->next = graph->adjListArr[dest].head;
    graph->adjListArr[dest].head = n1;
    graph->adjListArr[dest].num_members++;
    //graph->adjListArr[dest].wt = dist;
}

/* Function to print the adjacency list of graph*/
void displayGraph(graph_p graph)
{
    int i;
    // for (i = 0; i < graph->num_vertices; i++)
    // {
    //     adjlist_node_p adjListPtr = graph->adjListArr[i].head;
    //     printf("\n%d: ", i);
    //     while (adjListPtr)
    //     {
    //         printf("%d->", adjListPtr->vertex);
    //         adjListPtr = adjListPtr->next;
    //     }
    //     printf("NULL\n");
    // }
    for (i = 0; i < graph->num_vertices; i++)
    {
        adjlist_node_p adjListPtr = graph->adjListArr[i].head;
        printf("\n%d: ", i);
        while (adjListPtr)
        {
            printf("%f->", adjListPtr->wt);
            adjListPtr = adjListPtr->next;
        }
        printf("NULL\n");
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

    int n;
    n = 10;
    graph_p g = createGraph(n);
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

    displayGraph(g);

    return 0;
}
