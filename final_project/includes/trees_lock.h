#include <iostream>
#include <fstream>
#include <iomanip>
#include <pthread.h>

#ifndef TREES_LOCK_H
#define TREES_LOCK_H

#define MAX_KEY 100
#define MAX_VALUE 1000000

class Node
{
  public:

  int key;
  int value;
  Node* left;
  Node* right;
  pthread_mutex_t lock;
};

void range(Node* root, int start, int end);

void inOrder(Node *root);

void* thread_func(void*);

#endif
