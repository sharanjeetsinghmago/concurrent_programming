#include "trees_lock.h"

using namespace std;

extern int num_threads;

extern int num_iterations;

extern pthread_mutex_t tree_lock;

extern Node *groot;

extern ofstream out_file;

int r_max = 50;

int r_min = 41;

Node* getNewNode(int key, int value)
{
  Node* newNode = new Node();

  newNode->key = key;
  newNode->value = value;
  newNode->left = NULL;
  newNode->right = NULL;
  pthread_mutex_init(&newNode->lock,NULL);

  return newNode;
}

void inOrder(Node *root)
{
  if(root != NULL)
  {
    inOrder(root->left);

    if( (root->key >= r_min) && (root->key <= r_max) )
    {
    out_file << endl << " " << setw(5) << root->key << " : " << root->value << " ";

    // cout << " " << root->key << ":" << root->value << " ";
    }
    inOrder(root->right);
  }
}

void put(Node* root, int key, int value, int thread_num)
{
  if(root == groot)
  {
    pthread_mutex_lock(&tree_lock);
    if(groot == NULL)
    {
      groot = getNewNode(key,value);
      pthread_mutex_unlock(&tree_lock);
      return;
    }
    else
    {
      pthread_mutex_lock(&groot->lock);
      root = groot;
      pthread_mutex_unlock(&tree_lock);
    }
  }


  if(key < root->key)
  {
    if(root->left == NULL)
    {
      root->left = getNewNode(key, value);
      pthread_mutex_unlock(&root->lock);
    }
    else
    {
      pthread_mutex_lock(&root->left->lock);
      pthread_mutex_unlock(&root->lock);
      put(root->left, key, value, thread_num);
    }
  }
  else if(key > root->key)
  {
    if(root->right == NULL)
    {
      root->right = getNewNode(key, value);
      pthread_mutex_unlock(&root->lock);
    }
    else
    {
      pthread_mutex_lock(&root->right->lock);
      pthread_mutex_unlock(&root->lock);
      put(root->right, key, value, thread_num);
    }

  }
  else
  {
    //cout<<"\n Entry with same key already exists!!!";
    pthread_mutex_unlock(&root->lock);
  }

}


void get(Node* root, int key)
{
  if(root == groot)
  {
    pthread_mutex_lock(&tree_lock);
    if(groot == NULL)
    {
      cout<<"\n Search failed for Key:"<<key<<" !!!";
      pthread_mutex_unlock(&tree_lock);
      return;
    }
    else
    {
      pthread_mutex_lock(&groot->lock);
      root = groot;
      pthread_mutex_unlock(&tree_lock);
    }
  }


  if(key < root->key)
  {
    if(root->left == NULL)
    {
      cout<<"\n Search failed for Key:"<<key<<" !!!";
      pthread_mutex_unlock(&root->lock);
      return;
    }
    else
    {
      pthread_mutex_lock(&root->left->lock);
      pthread_mutex_unlock(&root->lock);
      get(root->left, key);
    }
  }
  else if(key > root->key)
  {
    if(root->right == NULL)
    {
      cout<<"\n Search failed for Key:"<<key<<" !!!";
      pthread_mutex_unlock(&root->lock);
      return;
    }
    else
    {
      pthread_mutex_lock(&root->right->lock);
      pthread_mutex_unlock(&root->lock);
      get(root->right, key);
    }
  }
  else
  {
    pthread_mutex_unlock(&root->lock);
  }
}

void* thread_func(void*)
{

  for(int i=0;i<num_iterations;i++)
  {
    int a = rand()%MAX_KEY;
    int b = rand()%MAX_VALUE;

    //cout<<"\n\n puting Key:"<<a<<" Value:"<<b<<"\n\n";

    put(groot, a, b, 1);

    get(groot,a);

  }

}
