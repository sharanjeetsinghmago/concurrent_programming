#include <iostream>
#include <fstream>
#include <iomanip>
#include <pthread.h>

#ifndef TREES_LOCK_H
#define TREES_LOCK_H

class Node
{
  public:

  int key;
  int value;
  Node* left;
  Node* right;
  pthread_mutex_t lock;
};

void put(Node* root, int key, int value);

Node* get(Node* root, int key);

void range(Node* root, int start, int end);

void inOrder(Node *root);

void* thread_func(void*);

#endif
