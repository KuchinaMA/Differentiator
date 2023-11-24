#include <stdio.h>
#include <stdlib.h>

#include "Tree.h"
#include "Diff.h"


/*Node* derivative(Node* node) {

    switch (node->type) {

        case T_NUM: {
            new_node = node_ctor(T_NUM, 0, NULL, NULL);
            return new_node;
        }

        case T_VAR: {
            new_node = node_ctor(T_NUM, 1, NULL, NULL);
            return new_node;
        }

        case T_OP: {

            switch(node->data) {

                case ADD: {

                    d_left = derivative(node->left);
                    d_right = derivative(node->right);
                    new_node = node_ctor(T_OP, ADD, d_left, d_right);
                    return new_node;
                }

                case SUB: {

                    d_left = derivative(node->left);
                    d_right = derivative(node->right);
                    new_node = node_ctor(T_OP, SUB, d_left, d_right);
                    return new_node;
                }


            }


    }

}*/
