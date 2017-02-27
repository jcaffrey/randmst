#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <limits.h>

#define MAX_WT 10.0
#define PARENT(i) (i - 1) / 2
#define LEFT(i) 2 * i + 1
#define RIGHT(i) 2 * i + 2

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
    struct mst_node* next;
} mst_node_t, *mst_node_p;

typedef struct min_heap
{
    int sz;
    int cap;
    int *index;
    mst_node_p *heap_arr;
    //need an array?
} min_heap_t, *min_heap_p;

min_heap_p initMinHeap(int v)
{
    min_heap_p H = (min_heap_p) malloc(sizeof(min_heap_t));
    H->index = (int*) malloc(v * sizeof(int));
    H->sz = 0;
    H->cap = v;
    H->heap_arr = (mst_node_p *) malloc(v * sizeof(mst_node_p));
    return H;
}

void swap(mst_node_p* v1, mst_node_p* v2)
{
    mst_node_p tmp = *v1;
    *v1 = *v2;
    *v2 = tmp;
}

// void insert(min_heap_p H, mst_node_p v)
// {
//     H->sz += 1;
//     H->heap_arr = (mst_node_p*) realloc(H->heap_arr, 16 * sizeof(mst_node_p));
//     H->heap_arr[H->sz] = v;
//
//     int N = H->sz;
//     printf("%zu\n", sizeof(H->heap_arr));
//     while(N != 0 && H->heap_arr[PARENT(N)] < H->heap_arr[N])
//     {
//
//         swap(&H->heap_arr[PARENT(N)], &H->heap_arr[N]);
//         N = PARENT(N);
//     }
//     return;
// }


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
    H->sz--;
    minHeapify(H, 0);

    return min;
}

void decreaseKey(min_heap_p H, int v, int key)
{
    int i;
    i = H->index[v];

    H->heap_arr[i]->wt = key;

    while(i && H->heap_arr[i]->wt < H->heap_arr[PARENT(i)]->wt)
    {
        H->index[H->heap_arr[i]->vertex] = PARENT(i);
        H->index[H->heap_arr[PARENT(i)]->vertex] = i;
        swap(&H->heap_arr[i], &H->heap_arr[PARENT(i)]);

        i = PARENT(i);
    }
}

mst_node_p createMSTNode(int v, double wt)
{
    mst_node_p newMSTNode = (mst_node_p) malloc(sizeof(mst_node_t));
    newMSTNode->vertex = v;
    newMSTNode->wt = wt;
    //newMSTNode->next = NULL;
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
        graph->alistArr[i].E = 0; // prob don't need this
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
// double prim(graph_p g)
// {
//     double mstWeight;
//     mstWeight = 0;
//     // initialize prev, prev arrays of size V
//     double dist[g->V];
//     //mst_node_p prev[g->V]; // will this work for large n?
//     int inMst[g->V];
//
//     mst_node_p mst[g->V];
//
//     int i;
//     for(i = 0; i < g->V; i++)
//     {
//         dist[i] = MAX_WT;
//         inMst[i] = 0;
//     }
//
//     dist[0] = 0.0;
//     // int n = g->V;
//     int u;
//
//     // graph is complete so can do each node one at a time and never have to look behind us once
//     // we add a node
//     inMst[0] = 1;
//     node_p alistPtr = g->alistArr[0].head;
//     int best_i;
//     best_i = 0;
//     double best_wt;
//     best_wt = MAX_WT;
//
//     for(u = 0; u < g->V; u++)
//     {
//         printf("OUTER FOR and u is: %i\n", u);
//
//         alistPtr = g->alistArr[u].head;
//
//         while(alistPtr  && inMst[alistPtr->vertex] == 0 && (inMst[u] == 1)) // || inMst[best_i] == 1))// && u != alistPtr->vertex)// && inMst[alistPtr->vertex] == 0)// && inMst[u] == 0) //&& alistPtr->vertex != inMst[u])
//         {
//
//             // printf("u = %i \n", u);
//             // printf("Vertex = %i\n", alistPtr->vertex);
//             // printf("weight = %f\n\n", alistPtr->wt);
//
//             if(best_wt > alistPtr->wt)// || dist[alistPtr->vertex] < alistPtr->wt)// && inMst[alistPtr->vertex] == 0)
//             {
//                 best_wt = alistPtr->wt;
//                 best_i = alistPtr->vertex;
//             }
//             if(dist[alistPtr->vertex] < alistPtr->wt)
//             {
//                 dist[alistPtr->vertex] = alistPtr->wt;
//             }
//
//             alistPtr = alistPtr->next;
//             printf("LOOPING AGAIN\n");
//
//         }
//         // printf("node selected to mst: %i\n", u);
//         if(best_wt <= dist[best_i])
//         {
//             printf("best index = %i\n", best_i);
//             printf("best weigth = %f\n\n", best_wt);
//             inMst[best_i] = 1;
//             dist[best_i] = best_wt;
//
//             //alistPtr = g->alistArr[best_i].head;
//             best_wt = MAX_WT;
//         }
//         // else
//         // {
//         //     alistPtr = g->alistArr[u].head;
//         //     //alistPtr = g->alistArr[u].head;
//         //     //best_wt = MAX_WT;
//         // }
//         // printf("best_i %i was added inMst[best_i] %i\n", best_i, inMst[best_i]);
//         // printf("u is: %i and inMst[u] is : %i\n\n", u, inMst[u]);
//
//         // if(best_i  > u)
//         //     u = best_i;
//     }
//
//     for(i = 0; i < g->V; i++)
//     {
//         printf("DIST[%i] = %f\n", i, dist[i]);
//         printf("inMst[%i] = %i\n", i, inMst[i]);
//
//     }
//
//     return 0.0;
// }

double minHeapPrim(graph_p g)
{
    double totWt;
    totWt = 0.0;
    int parent[g->V];
    double key[g->V];
//    printf("%f\n", g->alistArr[2].head->wt);
    min_heap_p H = initMinHeap(g->V);
    int n;
    for(n = 0; n < g->V; n++)
    {
        parent[n] = -1.0;
        key[n] = INT_MAX;
        H->heap_arr[n] = createMSTNode(n, key[n]);
        H->index[n] = n;
    }
    key[0] = 0;
    H->heap_arr[0] = createMSTNode(0, key[0]);
    H->index[0] = 0;

    H->sz = g->V;

    while(H->sz != 0)
    {
        int u;
        mst_node_p u_node = deleteMin(H);
        u = u_node->vertex;

        node_p alistPtr;
        alistPtr = g->alistArr[u].head;

        while(alistPtr != NULL)
        {
            int v;
            v = alistPtr->vertex;

            if(H->index[v] < H->sz && alistPtr->wt < key[v])
            {
                key[v] = alistPtr->wt;
                parent[v] = u;
                decreaseKey(H, v, key[v]);
            }
            alistPtr = alistPtr->next;
        }
    }

    for(n = 0; n < g->V; n++)
    {
        //printf("VERTEX: %i and WEIGHT: %f\n", n, key[n]);
        totWt += key[n];
    }
    // mst_node_p low;
    // mst_node_p med;
    // mst_node_p h;
    //
    // low = deleteMin(H);
    // med = deleteMin(H);
    // h = deleteMin(H);
    //
    //
    // printf("low has wt: %f\n", low->wt);
    // printf("med has wt: %f\n", med->wt);
    // printf("h has wt: %f\n", h->wt);
    //


    return totWt;
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

    int i;
    double totWt;
    for(i = 0; i < numtrials; i++)
    {
        graph_p g = createGraph(numpts);
        populateGraph(g, numpts, dimension);
        totWt += minHeapPrim(g);
        destroyGraph(g);
    }
    printf("AVG WT: %f\n", totWt / (double) numtrials);

    // int n;
    // n = 100;  // 10k took 8 seconds
    // graph_p g = createGraph(n);
    // populateGraph(g, n, dimension);
    // //displayGraph(g);
    //
    // minHeapPrim(g);
    //
    // //prim(g);
    //
    //
    // destroyGraph(g);

    return 0;
}
