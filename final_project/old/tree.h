#include <iostream>
#include <time.h>

#ifndef TREE_H
#define TREE_H

class Node
{
  public:

  int key;
  int value;
  Node* left;
  Node* right;
};

Node* getNewNode(int key, int value);
Node* findMin(Node* root);
void inOrder(Node *root);
Node* insert(Node* root, int key, int value);
Node* delete_key(Node* root, int key);
Node* getNode(Node* root, int key);

#endif
