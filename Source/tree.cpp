#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "tree.h"

t_err TreeCtor(t_tree *Tree, t_node *head, const char* name){

    assert(Tree != NULL);

    *Tree = {
        .name = name,
        .head = head, //
        .curr_node = &Tree->head,
        .nnodes = 0,
    };

    return TREE_OK;
}

#define MIN(a,b) ((a) > (b)) ? (b) : (a)

t_err TreeAddNode(t_tree *Tree, void* value, size_t valsize){

    void *node_val = calloc(1, sizeof(t_node) + valsize);

    assert(Tree != NULL);
    assert(value != NULL);
    assert(node_val != NULL);

    t_node node = {
        .value = (char*) node_val + sizeof(t_node),
        .valsize = valsize,
        .left = NULL,
        .right = NULL,
    };

    memcpy(node_val, &node, sizeof(t_node));
    memcpy(node.value, value, valsize);

    t_node* curr_node_ptr = Tree->head;
    t_node* curr_node_parent = NULL;

    if (!curr_node_ptr){
        Tree->head = (t_node*) node_val;
        Tree->curr_node = &Tree->head;
    }

    // printf("Value: %d\n", *((int*)((t_node*)node_val)->value));
    bool is_right = true;

    while (curr_node_ptr != NULL){
        if(memcmp(curr_node_ptr->value, value, MIN(valsize, curr_node_ptr->valsize)) >= 0){
            curr_node_parent = curr_node_ptr;
            curr_node_ptr = curr_node_ptr->right;
            is_right = true;
        }
        else{
            curr_node_parent = curr_node_ptr;
            curr_node_ptr = curr_node_ptr->left;
            is_right = false;
        }
    }

    if (curr_node_parent){
        if (is_right){
            curr_node_parent->right = (t_node*) node_val;
        }
        else{

            curr_node_parent->left = (t_node*) node_val;
        }
    }

    Tree->nnodes++;
    return TREE_OK;
}
#undef MIN

t_err BuildNode(t_node **Node, void *value, size_t valsize){

    t_node *node_val = (t_node*) calloc(1, sizeof(t_node) + valsize + 1);

    assert(Node != NULL);
    assert(value != NULL);
    assert(node_val != NULL);

    *node_val = {
        .value = (char*) node_val + sizeof(t_node),
        .valsize = valsize,
        .left = NULL,
        .right = NULL,
    };

    memcpy(node_val->value, value, valsize);

    *Node = node_val;

    return TREE_OK;
}

//TODO: Add Rec

t_err PrintTree(t_tree *Tree, const char *format){

    assert(Tree != NULL);
    assert(Tree->curr_node != NULL);

    printf(" ( ");
    if (!strcmp(format,"%s")){
        printf(format, ((char*)NODE->value));
    }
    else {
        printf(format, *((char*)NODE->value));
    }
    t_node *curr_node = *Tree->curr_node;

    if (curr_node->left){

        Tree->curr_node = &curr_node->left;
        PrintTree(Tree, format);
    }

    if (curr_node->right){

        Tree->curr_node = &curr_node->right;
        PrintTree(Tree, format);
    }

    printf(" ) ");

    Tree->curr_node = &Tree->head;
    return TREE_OK;
}

void *Resize(void *src, size_t size, size_t last_n, bool upscale){

    assert(src != NULL);

    src = realloc (src, (int) last_n*size*ResizeScale);
    assert(src != NULL);

    if (upscale)
        memset((char*) src + last_n*size, '\0', (size_t) last_n*size*(ResizeScale - 1));

    return src;
}







