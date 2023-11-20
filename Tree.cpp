#include <stdio.h>
#include <assert.h>
//#include <stdlib.h>
//#include <strings.h>

#include "Tree.h"

Node* node_ctor(int type, int data, Node* left, Node* right) {

    Node* node = (Node*)calloc(1, sizeof(Node));
    assert(node && "It's impossible to create new node");

    node->type = type;
    node->data = data;
    node->left = left;
    node->right = right;

    return node;
}

int node_dtor(Node* node) {

    if (node->left != 0 && node->right != 0) {
        node_dtor(node->left);
        node_dtor(node->right);
    }

    node->left = NULL;
    node->right = NULL;
    node->data = DATA_POISON;
    node->type = TYPE_POISON;

    free(node);

    return 0;
}

Tree* tree_ctor(Node* root, size_t size) {

    assert(root);

    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    assert(tree && "It's impossible to create new node");

    tree->root = root;
    tree->size = size;

    return tree;
}


int tree_dtor(Tree* tree) {

    node_dtor(tree->root);
    tree->root = 0;
    tree->size = 0;

    free(tree);

    return 0;
}










