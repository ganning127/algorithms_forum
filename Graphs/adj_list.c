// Adjacency list representation of graphs in C
// Author: Ganning Xu, NCSSM
/* DESCRIPTION:
 * An adjacency list representation of graphs in C. By using an array of linked lists, each index in the array represents a vertex and each element is a linked list of nodes that represent nodes that are connected to the vertex through an edge.
 * For example:
 *
 * Time Complexity of each function is located in the definition of the function.
 * The space complexity of each function is also located in the definition of the function.
 * Room for improvement:
 *     - removing edges is not efficient, as we need to loop through BOTH the source and the destination nodes to remove the edge fully
 *     - edges are repeated between source and destination nodes, which may cause some space to be wasted. for example, if there was an edge between node 0 and node 1, then there will be an edge between node 1 and node 0 as well stored in both linked lists.
 *     - adding an edge is also not efficient, as we need to add the same structure to both the source and destination nodes' linked lists.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node
{
    int data;          // value of the node
    struct node *next; // pointer to the next node, as adjacent nodes are linked together in a linked list
} Node;
typedef Node *NodePtr;

typedef struct graph
{
    int numnodes;      // number of nodes
    NodePtr *adjlists; // lsit of NodePtrs, each NodePtr points to a linked list of adjacent nodes
} Graph;
typedef Graph *GraphPtr;

void add_edge(GraphPtr g, int from_node, int to_node);    // add an edge between two nodes
NodePtr create_node(int val);                             // create a node
GraphPtr create_graph(int numnodes);                      // create a graph
void print_graph(GraphPtr g);                             // print the graph
void destroy_graph(GraphPtr *g);                          // destroy the graph
void remove_edge(GraphPtr g, int from_node, int to_node); // remove an edge between two nodes
void add_vertex(GraphPtr *g);                             // add a vertex
bool has_edge(GraphPtr g, int from_node, int to_node);    // check if there is an edge between two nodes

int main(void)
{
    GraphPtr g = create_graph(8); // create a graph with 8 nodes

    // adding edges between nodes
    add_edge(g, 0, 1);
    add_edge(g, 0, 2);
    add_edge(g, 1, 2);
    add_edge(g, 1, 3);
    add_edge(g, 2, 3);
    add_edge(g, 3, 4);
    add_edge(g, 4, 5);
    add_edge(g, 5, 6);
    add_edge(g, 6, 7);

    // add a new vertex
    add_vertex(&g);

    remove_edge(g, 0, 1); // remove some edges
    remove_edge(g, 0, 2);

    // check if edge between different nodes exist
    printf("Is there an edge between %d and %d: %s\n", 0, 5, has_edge(g, 0, 5) ? "true" : "false");
    printf("Is there an edge between %d and %d: %s\n", 1, 3, has_edge(g, 1, 3) ? "true" : "false");

    print_graph(g); // print the graph

    destroy_graph(&g); // destroy the graph, free the memory
    return 0;
}

GraphPtr create_graph(int numnodes)
{
    /*
        Time Complexity: O(1), as there is no looping involved.
        Space Complexity: O(n), as you'll need to allocate an array of NodePtrs to represent the adjacency list.
    */
    GraphPtr nu = malloc(sizeof(Graph));                  // allocate memory for the graph
    nu->numnodes = numnodes;                              // initialize the number of nodes
    nu->adjlists = calloc(sizeof(NodePtr), nu->numnodes); // allocate memory for the adjacency list
    return nu;                                            // return the graph
}

NodePtr create_node(int val)
{
    /*
        Time Complexity: O(1), as there is no looping involved.
        Space Complexity: O(n), as if you want more nodes, you'll need to allocate more memory.
    */
    NodePtr nu = malloc(sizeof(Node)); // allocate memory for the node
    nu->data = val;                    // initialize the value of the node
    nu->next = NULL;                   // initialize the next node to NULL, as we don't know what the next node is yet
    return nu;                         // return the node
}

void add_edge(GraphPtr g, int from_node, int to_node)
{
    /*
        Time Complexity: O(1), as there is no looping involved.
        Space Complexity: O(n), as you're creating a node and adding a pointer to the next node.
    */

    NodePtr nu = create_node(to_node); // allocate space for the new node

    // create an edge from the from_node to the new node
    nu->next = g->adjlists[from_node]; // set the next node of the new node to the original first node in the index list
    g->adjlists[from_node] = nu;       // set the first node of the index to the newly created node

    // create an edge from the to_node to the from_node
    nu = create_node(from_node);     // allocate space for the new node
    nu->next = g->adjlists[to_node]; // set the next node of the new node to the original first node in the index list
    g->adjlists[to_node] = nu;       // set the first node of the index to the newly created node
}

void print_graph(GraphPtr g)
{
    /*
        Time Complexity: O(n^2) as you need to loop through each node AND each of its edges
        Space Complexity: O(1), as you're just printing the graph, not allocating any more memory.
    */
    NodePtr nu; // create a node pointer
    for (int i = 0; i < g->numnodes; i++)
    {
        nu = g->adjlists[i];       // set the node pointer to the first node in the index
        printf("Vertex %d | ", i); // print the vertex number
        while (nu != NULL)
        {
            printf("%d -> ", nu->data); // print the value of the node
            nu = nu->next;              // set the node pointer to the next node
        }

        printf("NULL \n"); // print NULL, as the end of the list
    }
}

void destroy_graph(GraphPtr *g)
{
    /*
        Time Complexity: O(n^2) as you need to loop through each node AND each of its edges
        Space Complexity: O(1), as you're just freeing the memory, not allocating any more memory.
    */
    NodePtr nu, temp; // create a node pointer and a temporary node pointer
    for (int i = 0; i < (*g)->numnodes; i++)
    {
        nu = (*g)->adjlists[i]; // set the node pointer to the first node in the index
        while (nu != NULL)
        {
            temp = nu;     // set the temporary node pointer to the current node
            nu = nu->next; // set the node pointer to the next node
            free(temp);    // free the memory of the temporary node
        }
    }
    free((*g)->adjlists); // free the memory of the adjacency list
    free(*g);             // free the memory of the graph
}

void remove_edge(GraphPtr g, int from_node, int to_node)
{
    /*
        Time Complexity: O(n), as you need to loop through each node.
        Space Complexity: O(1), as you're just freeing the memory, not allocating any more memory.
    */
    NodePtr nu = g->adjlists[from_node]; // set the node pointer to the first node in the index
    NodePtr prev = NULL;                 // set the previous node pointer to NULL
    while (nu != NULL)
    {
        if (nu->data == to_node)
        {
            // next node is the node we want to remove
            if (prev == NULL)
                g->adjlists[from_node] = nu->next; // set the first node of the index to the next node
            else
                prev->next = nu->next; // set the next node of the previous node to the next node
            free(nu);                  // free the memory of the node
            break;                     // break out of the loop
        }
        prev = nu;     // set the previous node to the current node
        nu = nu->next; // set the node pointer to the next node, allowing us to loop through the list
    }

    // REPEAT SAME STEPS AS ABOVE, BUT WITH THE SECOND INDEX
    nu = g->adjlists[to_node];
    prev = NULL;
    while (nu != NULL)
    {
        if (nu->data == from_node)
        {
            if (prev == NULL)
                g->adjlists[to_node] = nu->next;
            else
                prev->next = nu->next;
            free(nu);
            break;
        }
        prev = nu;
        nu = nu->next;
    }
}

void add_vertex(GraphPtr *g)
{
    /*
        Time Complexity: O(n), as you need to loop through each node.
        Space Complexity: O(n), as you're allocating more memory for each new node added.
    */
    NodePtr *new_adjlists;                                      // create a new array of NodePtrs
    new_adjlists = calloc(sizeof(NodePtr), (*g)->numnodes + 1); // allocate memory for the new adjacency list
    for (int i = 0; i < (*g)->numnodes; i++)
    {
        new_adjlists[i] = (*g)->adjlists[i]; // copy the old adjacency list to the new adjacency list
    }
    free((*g)->adjlists);          // free the memory of the old list
    (*g)->adjlists = new_adjlists; // set the old list to the new list
    (*g)->numnodes++;              // increment the number of nodes
}

bool has_edge(GraphPtr g, int from_node, int to_node)
{
    /*
        Time Complexity: O(n), as you need to loop through each node.
        Space Complexity: O(1), as you're just checking if the edge exists, not allocating any more memory.
    */
    NodePtr nu = g->adjlists[from_node]; // set the node pointer to the first node in the index
    while (nu != NULL)
    {
        if (nu->data == to_node)
            return true; // edge exists
        nu = nu->next;
    }
    return false; // edge doesn't exist
}