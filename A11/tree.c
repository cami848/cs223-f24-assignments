/*----------------------------------------------
 * Author: Camille Lopez
 * Date:11/15/2024
 * Description: This program implements a
 * binary search tree.
 ---------------------------------------------*/

#include "tree.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>

struct tree_node *find(const char *name, struct tree_node *root) {
  if (root == NULL)
    return NULL;
  if (strcmp(name, root->data.name) < 0) {
    return (find(name, root->left));
  } else if (strcmp(name, root->data.name) > 0) {

    return (find(name, root->right));
  } else {
    return root;
  }
}

struct tree_node *insert(const char *name, struct tree_node *root) {
  if (root == NULL) {
    root = (struct tree_node *)malloc(sizeof(struct tree_node));
    if (root == NULL) {
      perror("Out of space!!!");
      exit(EXIT_FAILURE);
    } else {
      strncpy(root->data.name, name, sizeof(root->data.name));
      root->left = NULL;
      root->right = NULL;
    }
  } else if (strcmp(name, root->data.name) < 0) {
    root->left = insert(name, root->left);
  } else {
    if (strcmp(name, root->data.name) > 0) {
      root->right = insert(name, root->right);
    }
  }
  return root;
}

void clear(struct tree_node *root) {
  if (root == NULL) {
    return;
  }

  // Recursively clear left and right subtrees
  clear(root->left);
  clear(root->right);

  // Free the current node
  free(root);
};
;

void helper_print(struct tree_node *root, int depth) {
  if (root == NULL) {
    return;
  }
  // Print the left child with indentation
  if (root->left) {
    for (int i = 0; i < depth; i++) {
      printf(" ");
    }
    printf(" l:%s\n", root->left->data.name);
    helper_print(root->left, depth + 1); // Recursively print left subtree
  }

  // Print the right child with indentation
  if (root->right) {
    for (int i = 0; i < depth; i++) {
      printf(" ");
    }
    printf(" r:%s\n", root->right->data.name);
    helper_print(root->right, depth + 1); // Recursively print right subtree
  }
}

void print(struct tree_node *root) {
  if (root == NULL) {
    return;
  }

  // Print the current node
  printf("%s\n", root->data.name);

  helper_print(root, 1);
}

void printSorted(struct tree_node *root) {
  if (root == NULL) {
    return;
  }

  // Recursively print left subtree
  printSorted(root->left);

  printf("%s\n", root->data.name);

  // Recursively print right subtree
  printSorted(root->right);
}
