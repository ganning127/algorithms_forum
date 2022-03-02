#include <stdio.h>
#include <stdlib.h>
#define BLACK 0
#define RED 1

typedef struct node
{
    int key; // the integer value stored
    struct node *parent;
    struct node *left;
    struct node *right;
    int color; // RED or BLACK (RED = 1, BLACK = 0)
} Node;
typedef Node *NodePtr;
NodePtr ROOT; // root of the tree, accessed by almost all the functions in the program, so we set it as a constant
NodePtr NILL; // NILL node, accessed by almost all the functions in the program, we set it as a constant

void rb_insert(int key);       // insert a node with key value
void rb_print(NodePtr bst);    // print the tree sideways
void rb_insert_fix(NodePtr z); // fix the tree after insertion
void rb_rotate_right(NodePtr x);
void rb_rotate_left(NodePtr x);
void rb_setup();

int main(void)
{
    rb_setup(); // setup the red-black tree

    for (int i = 0; i < 100; i++)
    {
        rb_insert(i);
    }

    rb_print(ROOT); // print the tree sideways
    return 0;
}

void rb_setup()
{
    // this function should be run before inserting any node into the tree
    NILL = malloc(sizeof(struct node)); // create NILL node
    NILL->color = BLACK;                // color of NILL node is black
    ROOT = NILL;                        // at first, the root is NILL
}

void rb_insert(int key)
{
    NodePtr nu = (NodePtr)malloc(sizeof(Node)); // alloc memory for a new node
    nu->key = key;                              // set the key value
    nu->left = nu->right = NILL;                // set the left and right child of the new node to NILL
    nu->color = RED;                            // assume new node is red

    NodePtr tmp = ROOT;        // start from the root
    NodePtr tmp_parent = NILL; // temporary parent of the new node

    while (tmp != NILL)
    {
        // find where to place the new node
        tmp_parent = tmp;
        if (key <= tmp->key)
            tmp = tmp->left; // go left if the key is smaller than the current node
        else
            tmp = tmp->right; // go right if the key is larger than the current node
    }

    if (tmp_parent == NILL)
        ROOT = nu; // if the tree is empty, the new node becomes the root
    else if (nu->key <= tmp_parent->key)
        tmp_parent->left = nu;
    else
        tmp_parent->right = nu;

    nu->parent = tmp_parent;

    rb_insert_fix(nu); // fix the red black tree after insertion
}

void rb_insert_fix(NodePtr bst)
{
    while (bst->parent->color == RED)
    {
        //  node's parent is left child of node's grand parent
        if (bst->parent == bst->parent->parent->left)
        {
            // node's grand parent's right child is RED
            if (bst->parent->parent->right->color == RED)
            {
                bst->parent->color = BLACK;                // set parent's color to black
                bst->parent->parent->right->color = BLACK; // set grandparent's right child's color to black
                bst->parent->parent->color = RED;          // set grandparent's color to red
                bst = bst->parent->parent;                 // move up the tree
            }

            // node's grand parent's right child is not RED
            else
            {
                /* z is z's parent's right child */
                if (bst == bst->parent->right)
                {
                    bst = bst->parent;   // move up the tree
                    rb_rotate_left(bst); // left rotation on node
                }

                bst->parent->color = BLACK;           // set parent's color to black
                bst->parent->parent->color = RED;     // set grandparent's color to red
                rb_rotate_right(bst->parent->parent); // right rotation on grandparent
            }
        }

        /* z's parent is z's grand parent's right child */
        else
        {
            //  z's left uncle or z's grand parent's left child is also RED
            if (bst->parent->parent->left->color == RED)
            {
                // the grandparent's left child is RED
                bst->parent->color = BLACK;               // set parent's color to black
                bst->parent->parent->left->color = BLACK; // set grandparent's left child's color to black
                bst->parent->parent->color = RED;         // set grandparent's color to red
                bst = bst->parent->parent;                // move up the tree
            }

            else
            {
                //  z's left uncle is BLACK
                if (bst == bst->parent->left)
                {
                    // z is z's parents left child
                    bst = bst->parent;    // move up the tree
                    rb_rotate_right(bst); // right rotation on node
                }
                bst->parent->color = BLACK;          // set parent's color to black
                bst->parent->parent->color = RED;    // set grandparent's color to red
                rb_rotate_left(bst->parent->parent); // left rotation on grandparent
            }
        }
    }

    ROOT->color = BLACK;
}

void rb_rotate_left(NodePtr bst)
{
    // right rotation on bst node
    NodePtr y;

    y = bst->right;       // y is bst's right child
    bst->right = y->left; // bst's right child becomes y's left child
    if (y->left != NILL)
        y->left->parent = bst; // set the parent of y's left child to bst

    y->parent = bst->parent; // y's parent becomes bst's parent
    if (y->parent == NILL)   // if bst is the root
        ROOT = y;
    else if (bst == bst->parent->left) // if bst is bst's parent's left child
        bst->parent->left = y;
    else
        bst->parent->right = y; // if bst is bst's parent's right child

    y->left = bst;   // y's left child becomes bst
    bst->parent = y; // bst's parent becomes y
}

void rb_rotate_right(NodePtr bst)
{
    NodePtr y;

    y = bst->left;        // y is bst's left child
    bst->left = y->right; // bst's left child becomes y's right child
    if (y->right != NILL)
        y->right->parent = bst; // set the parent of y's right child to bst

    y->parent = bst->parent; // y's parent becomes bst's parent
    if (y->parent == NILL)
        ROOT = y;                      // if bst is the root
    else if (bst == bst->parent->left) // if bst is bst's parent's left child
        bst->parent->left = y;         // bst's parent's left child becomes y
    else
        bst->parent->right = y; // bst's parent's right child becomes y

    y->right = bst;  // y's right child becomes bst
    bst->parent = y; // bst's parent becomes y
}

void rb_print(NodePtr bst)
{
    static unsigned int depth = 0; // depth of the current node, static variable
    if (bst == NILL)               // if the current node is NILL
        return;
    ++depth;              // increase the depth
    rb_print(bst->right); // recursive call on right nodes
    --depth;              // reduce depth

    for (unsigned int i = 0; i < depth; ++i)
        printf("  "); // print spaces

    printf("%d\n", bst->key); // print the key of the current node
    ++depth;                  // increase depth
    rb_print(bst->left);      // recursive call on left nodes
    --depth;                  // reduce depth
}