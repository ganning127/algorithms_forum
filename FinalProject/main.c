#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#ifndef GRAPH_ADJ_MAT_H_
#define GRAPH_ADJ_MAT_H_

typedef struct mygraph
{
    int numnodes;
    bool **edges; // 2D matrix of edges
} graph;

typedef graph *GraphPtr;

GraphPtr create_graph(int numnodes);
void destroy_graph(GraphPtr g);
void print_graph(GraphPtr g);
void add_edge(GraphPtr g, int from_node, int to_node);
void add_vertex(GraphPtr *g);
bool has_edge(GraphPtr g, int from_node, int to_node);

#endif

int main()
{
    GraphPtr g = create_graph(8);

    add_edge(g, 0, 1);
    add_edge(g, 0, 2);
    add_edge(g, 1, 2);
    add_edge(g, 1, 3);
    add_edge(g, 2, 3);
    add_edge(g, 3, 4);
    add_vertex(&g);

    print_graph(g);

    destroy_graph(g);
}

GraphPtr create_graph(int numnodes)
{
    /*
        Time Complexity: O(n), as you'll need to loop through each node within the graph and a row that represents each node in the adjacency matrix.
    */

    GraphPtr g = malloc(sizeof(graph)); // allocate memory for the graph

    if (g == NULL)
        return NULL; // just in case malloc failed

    // initialize our object
    g->numnodes = numnodes;
    g->edges = calloc(sizeof(bool *), g->numnodes); // initialize the 2D matrix to zeros

    if (g->edges == NULL) // if the 2D matrix failed to allocate
    {
        free(g);     // free the graph
        return NULL; // return NULL
    }

    // initialize the edges
    for (int i = 0; i < g->numnodes; i++)
    {
        g->edges[i] = calloc(sizeof(bool), g->numnodes); // initialize the row to zeros
        if (g->edges[i] == NULL)
        {
            // check if calloc failed
            destroy_graph(g);
            return NULL;
        }
    }

    return g; // return the graph
}

void destroy_graph(GraphPtr g)
{
    /*
        O(n) time complexity, as you'll be looping through each row in the adjacency matrix to free it.
    */

    if (g->edges == NULL)
        return; // just in case there are no edges to free

    for (int i = 0; i < g->numnodes; i++)
    {
        if (g->edges[i] != NULL)
            free(g->edges[i]);
    }

    free(g->edges);
    free(g);
}

void print_graph(GraphPtr g)
{
    /*
        O(n^2) time complexity, as you'll be looping through each row and column in the adjacency matrix.
    */

    for (int c = 0; c < g->numnodes; c++)
    {
        printf("%.1d ", c + 1); // print the header row
    }
    printf("\n");
    for (int c = 0; c < g->numnodes; c++)
    {
        printf("---"); // print the header row separator
    }
    printf("\n");
    for (int r = 0; r < g->numnodes; r++)
    {
        printf("%.1d| ", r + 1); // print each row number
        for (int c = 0; c < g->numnodes; c++)
        {
            printf("%.1d ", g->edges[r][c]); // print the value of the edge
        }
        printf("\n");
    }
}

void add_edge(GraphPtr g, int from_node, int to_node)
{
    /*
        O(1) time complexity, as you'll be setting the value of the edge to 1.
    */
    assert(g != NULL);
    assert(from_node >= 0 && from_node < g->numnodes);
    assert(to_node >= 0 && to_node < g->numnodes);

    if (g->edges[from_node][to_node])
    {
        return; // edge already exists
    }

    g->edges[from_node][to_node] = true;
}

bool has_edge(GraphPtr g, int from_node, int to_node)
{
    /*
        O(1) time complexity, as you'll be returning the value of the edge.
    */
    assert(g != NULL);
    assert(from_node >= 0 && from_node < g->numnodes);
    assert(to_node >= 0 && to_node < g->numnodes);

    return g->edges[from_node][to_node];
}

void add_vertex(GraphPtr *gPtr)
{
    /*
        O(n^2) time complexity, as you'll be looping through each row and column in the adjacency matrix to create a new node.
    */
    GraphPtr g = *gPtr;                       // get the graph pointer
    int new_numnodes = g->numnodes + 1;       // get the new number of nodes
    GraphPtr nu = create_graph(new_numnodes); // create a new graph with the new number of nodes
    for (int i = 0; i < g->numnodes; i++)
    {
        for (int j = 0; j < g->numnodes; j++)
        {
            nu->edges[i][j] = g->edges[i][j]; // copy the edges from the old graph to the new graph
        }
    }

    destroy_graph(g); // destroy the old graph

    *gPtr = nu; // set the new graph pointer
}