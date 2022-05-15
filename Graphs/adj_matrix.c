// Adjacency matrix representation of graphs in C
// Author: Ganning Xu, NCSSM
/* DESCRIPTION:
 * An adjacency matrix is a 2D array that represents a graph. Each row represents a node and each column represents a node. The value in the matrix represents whether there is an edge between the two nodes. For example, if there was an edge between node 0 and node 1, then the value in matrix[0][1] would be 1. If there was no edge between the two nodes, then the value in matrix[0][1] would be 0.
 *
 *
 * Time Complexity of each function is located in the definition of the function.
 * The space complexity of each function is also located in the definition of the function.
 * Room for improvement:
 *     - adding vertices is not efficient, since we need to reallocate the adjacency matrix
 *     - for large graphs with not many vertices, a lot of space is wasted because of the adjacency matrix.
 *     - regardless of the number of edges, the amount of space allocated is the same, so it's inefficient if there are not many edges
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct mygraph
{
    int numnodes; // number of nodes
    bool **edges; // 2D matrix of edges
} graph;
typedef graph *GraphPtr;

GraphPtr create_graph(int numnodes);                   // create a graph
void destroy_graph(GraphPtr g);                        // destroy a graph
void print_graph(GraphPtr g);                          // print the graph
void add_edge(GraphPtr g, int from_node, int to_node); // add an edge between two nodes
void add_vertex(GraphPtr *g);                          // add a vertex
bool has_edge(GraphPtr g, int from_node, int to_node); // check if there is an edge between two nodes
void remove_edge(GraphPtr g, int from_node, int to_node);

int main()
{
    GraphPtr g = create_graph(8); // create a graph with 8 nodes

    // adding edges between nodes
    add_edge(g, 0, 1); // add an edge between node 0 and node 1
    add_edge(g, 0, 2);
    add_edge(g, 1, 2);
    add_edge(g, 1, 3);
    add_edge(g, 2, 3);
    add_edge(g, 3, 4);
    remove_edge(g, 3, 4);

    // check if edge between different nodes exist
    printf("Is there an edge between %d and %d: %s\n", 0, 5, has_edge(g, 0, 5) ? "true" : "false");
    printf("Is there an edge between %d and %d: %s\n", 0, 1, has_edge(g, 0, 1) ? "true" : "false");

    add_vertex(&g); // add a vertex

    print_graph(g); // print the graph

    destroy_graph(g); // destroy the graph
}

GraphPtr create_graph(int numnodes)
{
    /*
        Time Complexity: O(n), as you'll need to loop through each node within the graph and a row that represents each node in the adjacency matrix.
        Space Complexity: O(n^2), as you'll need to allocate a 2D array of booleans to represent the adjacency matrix.
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
        Time Complexity: O(n), as you'll be looping through each row in the adjacency matrix to free it.
        Space Complexity: O(1). No memory is allocated in this function.
    */

    if (g->edges == NULL)
        return; // just in case there are no edges to free

    for (int i = 0; i < g->numnodes; i++)
    {
        // free the row
        if (g->edges[i] != NULL)
            free(g->edges[i]);
    }

    free(g->edges); // free the 2D matrix
    free(g);        // free the graph
}

void print_graph(GraphPtr g)
{
    /*
        Time Complexity: O(n^2), as you'll be looping through each row and column in the adjacency matrix.
        Space Complexity: O(1) as no memory is allocated in this function.
    */
    printf("   "); // print the first column
    for (int c = 0; c < g->numnodes; c++)
    {
        printf("%.1d ", c); // print the header row
    }
    printf("\n");
    for (int c = 0; c < g->numnodes; c++)
    {
        printf("---"); // print the header row separator
    }
    printf("\n");
    for (int r = 0; r < g->numnodes; r++)
    {
        printf("%.1d| ", r); // print each row number
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
        Time Complexity: O(1), as you'll be setting the value of the edge to 1.
        Space Complexity: O(1), as no memory is allocated in this function. While an edge is being added, it's only a bool that was already present that is manipulated. Thus, no additional memory is allocated.
    */

    // safety checks
    assert(g != NULL);
    assert(from_node >= 0 && from_node < g->numnodes);
    assert(to_node >= 0 && to_node < g->numnodes);

    if (g->edges[from_node][to_node])
    {
        return; // edge already exists
    }

    g->edges[from_node][to_node] = true; // set the edge to 1, indicating an edge exists
}

bool has_edge(GraphPtr g, int from_node, int to_node)
{
    /*
        Time Complexity: O(1), as you'll be returning the value of the edge.
        Space Complexity: O(1), as no memory is allocated in this function.
    */

    // safety checks
    assert(g != NULL);
    assert(from_node >= 0 && from_node < g->numnodes);
    assert(to_node >= 0 && to_node < g->numnodes);

    return g->edges[from_node][to_node]; // return the value of the edge (0 or 1, representing true/false)
}

void add_vertex(GraphPtr *gPtr)
{
    /*
        Time Complexity: O(n^2), as you'll be looping through each row and column in the adjacency matrix to create a new node.
        Space Complexity: O(n^2), as you'll be creating a new row and column in the adjacency matrix, and since everything is stored as an array, a new matrix needs to be created, resulting in the same space complexity as create_graph().
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

void remove_edge(GraphPtr g, int from_node, int to_node)
{
    /*
        Time Complexity: O(1), as you'll be setting the value of the edge to 0.
        Space Complexity: O(1), as no memory is allocated in this function. While an edge is being removed, it's only a bool that was already present that is manipulated. Thus, no additional memory is allocated.
    */

    // safety checks
    assert(g != NULL);
    assert(from_node >= 0 && from_node < g->numnodes);
    assert(to_node >= 0 && to_node < g->numnodes);

    if (!g->edges[from_node][to_node])
    {
        return; // edge doesn't exist
    }

    g->edges[from_node][to_node] = false; // set the edge to 0, indicating no edge exists
}