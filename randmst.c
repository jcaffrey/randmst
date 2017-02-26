#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#define MAX_WT 10.0
#define PARENT(i) i / 2
#define LEFT(i) 2 * i
#define RIGHT(i) 2 * i + 1

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

typedef struct graph
{
    int V;         /*Number of vertices*/
    alist_p alistArr;     /*Adjacency lists' array*/
} graph_t, *graph_p;

// meant to be a set of nodes - used for making an array of vertices
// typedef struct set
// {
//     node_p setV;
// } set_t, *set_p;

typedef struct mst_node
{
    int vertex;
    double wt;
} mst_node_t, *mst_node_p;

typedef struct min_heap
{
    int sz;
    int V;
    int *index;
    mst_node_p * heap_arr;
    //need an array?
} min_heap_t, *min_heap_p;

min_heap_p initMinHeap(int V)
{
    min_heap_p H = (min_heap_p) malloc(sizeof(min_heap_t));
    H->index = (int*) malloc(V * sizeof(int));
    H->sz = 0;
    H->V = V;
    H->heap_arr = (mst_node_p *) malloc(V * sizeof(mst_node_p));
    return H;
}

void swap(mst_node_p* v1, mst_node_p* v2)
{
    mst_node_p tmp = *v1;
    *v1 = *v2;
    *v2 = tmp;
}

void insert(min_heap_p H, mst_node_p v)
{
    H->sz += 1;
    H->heap_arr[H->sz] = v;

    int N = H->sz;
    while(N != 0 && H->heap_arr[PARENT(N)] < H->heap_arr[N])
    {
        swap(&H->heap_arr[PARENT(N)], &H->heap_arr[N]);
        N = PARENT(N);
    }
}


void minHeapify(min_heap_p H, int N)
{
    int l, r, smallest;
    l = LEFT(N);
    r = RIGHT(N);
    if(l < H->sz && H->heap_arr[l]->wt < H->heap_arr[r]->wt)
    {
        smallest = l;
    }
    else
    {
        smallest = N;
    }
    if(r < H->sz && H->heap_arr[r]->wt < H->heap_arr[l]->wt)
    {
        smallest = r;
    }
    if (smallest != N)
    {
        mst_node_p s_node = H->heap_arr[smallest];
        mst_node_p n_node = H->heap_arr[N];

        H->index[s_node->vertex] = N;
        H->index[n_node->vertex] = smallest;

        swap(&H->heap_arr[smallest], &H->heap_arr[N]);

        minHeapify(H, smallest);
    }
}

mst_node_p deleteMin(min_heap_p H)
{
    if (H->sz == 0)
    {
        printf("deleteMin called on empty! \n");
        return NULL;
    }
    mst_node_p min = H->heap_arr[0];

    mst_node_p leaf = H->heap_arr[H->sz - 1];

    // bring leaf to top
    H->heap_arr[0] = leaf;

    H->index[min->vertex] = H->sz - 1;
    H->index[leaf->vertex] = 0; // hold invariant that root is 0

    H->sz -= H->sz;
    minHeapify(H, 0);

    return min;
}


mst_node_p createMSTNode(int v, double wt)
{
    mst_node_p newMSTNode = (mst_node_p) malloc(sizeof(mst_node_t));
    newMSTNode->vertex = v;
    newMSTNode->wt = wt;
    return newMSTNode;
}

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
    printf("NUM VERTICES IS: %i\n", graph->V);

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



// set_p prim(graph_p g); - W/ BELOW WILL JUST RETURN THE WEIGHT OF THE TREE
double prim(graph_p g)
{
    double mstWeight;
    mstWeight = 0;
    // initialize prev, prev arrays of size V
    double dist[g->V];
    //mst_node_p prev[g->V]; // will this work for large n?
    int inMst[g->V];

    mst_node_p mst[g->V];

    int i;
    for(i = 0; i < g->V; i++)
    {
        dist[i] = MAX_WT;
        inMst[i] = 0;
    }


    dist[0] = 0.0;
    // int n = g->V;
    int u;

    // graph is complete so can do each node one at a time and never have to look behind us once
    // we add a node
    for(u = 0; u < g->V; u++)
    {
        double best_wt;
        int best_i;
        best_wt = MAX_WT;
        node_p alistPtr = g->alistArr[u].head;
        while(alistPtr)// && u != alistPtr->vertex)// && inMst[alistPtr->vertex] == 0)// && inMst[u] == 0) //&& alistPtr->vertex != inMst[u])
        {
            if (u == alistPtr->vertex)
                printf("u = alistPtr->vertx: \n" );
            // printf("u = %i \n", u);
            // printf("Vertex = %i\n", alistPtr->vertex);
            // printf("weight = %f\n\n", alistPtr->wt);

            // find lowest weight edge leaving u
            // repeatedly update
            //if(dist[alistPtr->vertex] > alistPtr->wt)
            if(best_wt > alistPtr->wt && inMst[alistPtr->vertex] == 0)
            {
                best_wt = alistPtr->wt;
                best_i = alistPtr->vertex;
                //dist[alistPtr->vertex] = alistPtr->wt;
                //inMst[alistPtr->vertex] = 1; // this should only be set on final update?
                //printf("V %i ADDED W/ WEIGHT: %f = %f at LOOP: %i\n", alistPtr->vertex, dist[alistPtr->vertex], alistPtr->wt, u);
            }

            alistPtr = alistPtr->next;

        }
        printf("node selected to mst: %i\n", u);
        printf("best index = %i\n", best_i);
        printf("best weigth = %f\n\n", best_wt);

        inMst[best_i] = 1;
        dist[best_i] = best_wt;
        //inMst[0] = 1;
    }

    return 0.0;
}

int main(int argc, char* argv[])
{
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
    n = 4;  // 10k took 8 seconds
    graph_p g = createGraph(n);
    populateGraph(g, n, dimension);
    displayGraph(g);

    prim(g);

//    printf("%f\n", g->alistArr[2].head->wt);
    destroyGraph(g);

    return 0;
}
