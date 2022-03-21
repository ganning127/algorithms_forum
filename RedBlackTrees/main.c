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

void rb_insert(int key);                  // insert a node with key value
void rb_print(NodePtr bst);               // print the tree sideways
void rb_insert_fix(NodePtr z);            // fix the tree after insertion
void rb_rotate_right(NodePtr x);          // rotate the tree to the right on x
void rb_rotate_left(NodePtr x);           // rotate the tree to the left on x
void rb_setup();                          // setup the tree
void rb_delete(int key);                  // delete a node with key value
void rb_delete_fix(NodePtr x);            // fix the tree after deletion
NodePtr rb_search(NodePtr root, int key); // search for a node with key value
void destroy_bst(NodePtr *bstPtr);        // destroy the tree
int main(void)
{
    rb_setup(); // setup the red-black tree

    // inserting nodes
    rb_insert(10);
    rb_insert(20);
    rb_insert(30);
    rb_insert(40);
    rb_insert(50);

    // deleting nodes
    rb_delete(20);
    rb_delete(10);

    rb_print(ROOT);     // print the tree sideways
    destroy_bst(&ROOT); // destroy the tree;

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
            if (bst->parent->parent->right != NILL && bst->parent->parent->right->color == RED)
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
            if (bst->parent->parent->left != NILL && bst->parent->parent->left->color == RED)
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

void rb_replace(NodePtr x, NodePtr y)
{
    // switch the locations of x and y
    if (x->parent == NILL)
    {
        // if x is the root, y becomes the new root
        ROOT = y;
    }
    else if (x == x->parent->left)
    {
        // if x is x's parent's left child, y becomes x's parent's left child
        x->parent->left = y;
    }
    else
    {
        // if x is x's parent's right child, y becomes x's parent's right child
        x->parent->right = y;
    }

    // assign new parents
    y->parent = x->parent;
}

NodePtr rb_successor(NodePtr bst)
{
    // find the leftmost node (which is the minimum value node)
    while (bst->left != NILL)
    {
        bst = bst->left; // reassignment
    }
    return bst;
}

void rb_delete(int key)
{
    NodePtr y, x;
    int yOriginalColor; // original color of the y node

    NodePtr bst = rb_search(ROOT, key); // search for the node to be deleted
    y = bst;                            // temporarily store y node as bst root
    yOriginalColor = y->color;          // assign color of y node

    if (bst->left == NILL) // if there is no left child
    {
        x = bst->right;              // x is bst's right child
        rb_replace(bst, bst->right); // replace bst with bst's right child
    }
    else if (bst->right == NILL) // if there is no right child
    {
        x = bst->left;              // x is bst's left child
        rb_replace(bst, bst->left); // replace bst with bst's left child
    }
    else
    {
        y = rb_successor(bst->right); // y is the successor of bst
        yOriginalColor = y->color;    // assign color of y node
        x = y->right;                 // x is y's right child
        if (y->parent == bst)         // if y is bst's right child
            x->parent = y;            // x's parent becomes y
        else
        {
            rb_replace(y, y->right); // replace y with y's right child
            y->right = bst->right;   // y's right child becomes bst's right child
            y->right->parent = y;    // set the parent of y's right child to y
        }
        rb_replace(bst, y);    // replace bst with y
        y->left = bst->left;   // y's left child becomes bst's left child
        y->left->parent = y;   // set the parent of y's left child to y
        y->color = bst->color; // y's color becomes bst's color
    }

    if (yOriginalColor == BLACK)
        rb_delete_fix(x); // fix the tree
}

void rb_delete_fix(NodePtr x)
{
    NodePtr w;

    while (x != ROOT && x->color == BLACK) // while x is not the root and x is a black node
    {
        if (x == x->parent->left) // if x is left child of parent
        {
            w = x->parent->right; // w is x's parent's right child
            if (w->color == RED)  // if w is red
            {
                w->color = BLACK;          // w becomes black
                x->parent->color = RED;    // x's parent becomes red
                rb_rotate_left(x->parent); // rotate left on x's parent
                w = x->parent->right;      // reassign w
            }
            if (w->left->color == BLACK && w->right->color == BLACK) // if w's children are all black
            {
                w->color = RED; // w becomes red
                x = x->parent;  // reassign x
            }
            else
            {
                if (w->right->color == BLACK) // if w's right child is black
                {
                    w->left->color = BLACK; // w's left child becomes black
                    w->color = RED;         // w becomes red
                    rb_rotate_right(w);     // rotate right on w
                    w = x->parent->right;   // reassign w
                }
                w->color = x->parent->color; // w's color becomes x's parent's color
                x->parent->color = BLACK;    // x's parent becomes black
                w->right->color = BLACK;     // w's right child becomes black
                rb_rotate_left(x->parent);   // rotate left on x's parent
                x = ROOT;                    // reassign x to be the new root
            }
        }
        else
        {
            w = x->parent->left; // w is x's parent's left child
            if (w->color == RED) // if w is red
            {
                w->color = BLACK;           // w becomes black
                x->parent->color = RED;     // x's parent becomes red
                rb_rotate_right(x->parent); // rotate right on x's parent
                w = x->parent->left;        // reassign w
            }
            if (w->right->color == BLACK && w->left->color == BLACK) // if w's children are all black
            {
                w->color = RED; // w becomes red
                x = x->parent;  // reassign x to be x's parent
            }
            else
            {
                if (w->left->color == BLACK) // if w's left child is black
                {
                    w->right->color = BLACK; // w's right child becomes black
                    w->color = RED;          // w becomes red
                    rb_rotate_left(w);       // rotate left on w
                    w = x->parent->left;     // reassign w
                }
                w->color = x->parent->color; // w's color becomes x's parent's color
                x->parent->color = BLACK;    // x's parent becomes black
                w->left->color = BLACK;      // w's left child becomes black
                rb_rotate_right(x->parent);  // rotate right on x's parent
                x = ROOT;                    // reassign x to be the new root
            }
        }
    }
    x->color = BLACK;
}

void destroy_bst(NodePtr *bstPtr)
{
    // destroy the BST and free memory
    NodePtr bst = *bstPtr;
    if (bst != NILL)
    {
        destroy_bst(&(bst->left));
        destroy_bst(&(bst->right));
        free(bst);
        *bstPtr = NILL;
    }
}

NodePtr rb_search(NodePtr root, int key)
{
    // return the node that contains the key
    if (root == NILL)
        return NILL;
    else if (root->key == key)
        return root;
    else if (key < root->key)
        return rb_search(root->left, key);
    else
        return rb_search(root->right, key);
}