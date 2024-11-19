#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <stdlib.h>

typedef struct TreeNode {
    void* value;
    size_t valsize;
    TreeNode *left;
    TreeNode *right;
} t_node;

typedef struct TreeStruct {
    const char* name;
    TreeNode *head; //is actually a head? but to simplify the algorithm is right
    TreeNode **curr_node; // for recursion implementing
    int nnodes;
} t_tree;

typedef struct TreePath {
    t_node **Nodes;
    int *truth;
    size_t capacity;
    size_t depth;
} t_path;

typedef enum TreeError{
    TREE_OK = 0,

} t_err;


#define NODE (*Tree->curr_node)
#define NAME (char*) NODE->value

t_err TreeCtor(t_tree *Tree,t_node* head, const char* name);
t_err TreeAddNode(t_tree *Tree, void* value, size_t valsize);
t_err BuildNode(t_node **Node, void *value, size_t valsize);
t_err PrintTree(t_tree *Tree, const char *format);

void *Resize(void *src, size_t size, size_t last_n, bool upscale);
const int ResizeScale = 2;
#endif //TREE_H_INCLUDED
