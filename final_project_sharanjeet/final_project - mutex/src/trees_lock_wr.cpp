#include "trees_lock.h"

using namespace std;

int num_iterations = 10;

pthread_rwlock_t tree_lock;

Node *groot;

ofstream out_file;

int MAX_KEY = 100000;

int MAX_VALUE = 1000000;

int r_min = 0;

int r_max = MAX_KEY;

Node* getNewNode(int key, int value)
{
  Node* newNode = new Node();

  newNode->key = key;
  newNode->value = value;
  newNode->left = NULL;
  newNode->right = NULL;
  pthread_rwlock_init(&newNode->lock,NULL);

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

void put(Node* root, int key, int value)
{
  if(root == groot)
  {
    pthread_rwlock_wrlock(&tree_lock);
    if(groot == NULL)
    {
      groot = getNewNode(key,value);
      pthread_rwlock_unlock(&tree_lock);
      return;
    }
    else
    {
      pthread_rwlock_wrlock(&groot->lock);
      root = groot;
      pthread_rwlock_unlock(&tree_lock);
    }
  }


  if(key < root->key)
  {
    if(root->left == NULL)
    {
      root->left = getNewNode(key, value);
      pthread_rwlock_unlock(&root->lock);
    }
    else
    {
      pthread_rwlock_wrlock(&root->left->lock);
      pthread_rwlock_unlock(&root->lock);
      put(root->left, key, value);
    }
  }
  else if(key > root->key)
  {
    if(root->right == NULL)
    {
      root->right = getNewNode(key, value);
      pthread_rwlock_unlock(&root->lock);
    }
    else
    {
      pthread_rwlock_wrlock(&root->right->lock);
      pthread_rwlock_unlock(&root->lock);
      put(root->right, key, value);
    }

  }
  else
  {
    //cout<<"\n Entry with same key already exists!!!";
    pthread_rwlock_unlock(&root->lock);
  }

}


void get(Node* root, int key)
{
  if(root == groot)
  {
    pthread_rwlock_rdlock(&tree_lock);
    if(groot == NULL)
    {
      cout<<"\n Search failed for Key:"<<key<<" !!!";
      pthread_rwlock_unlock(&tree_lock);
      return;
    }
    else
    {
      pthread_rwlock_rdlock(&groot->lock);
      root = groot;
      pthread_rwlock_unlock(&tree_lock);
    }
  }


  if(key < root->key)
  {
    if(root->left == NULL)
    {
      cout<<"\n Search failed for Key:"<<key<<" !!!";
      pthread_rwlock_unlock(&root->lock);
      return;
    }
    else
    {
      pthread_rwlock_rdlock(&root->left->lock);
      pthread_rwlock_unlock(&root->lock);
      get(root->left, key);
    }
  }
  else if(key > root->key)
  {
    if(root->right == NULL)
    {
      cout<<"\n Search failed for Key:"<<key<<" !!!";
      pthread_rwlock_unlock(&root->lock);
      return;
    }
    else
    {
      pthread_rwlock_rdlock(&root->right->lock);
      pthread_rwlock_unlock(&root->lock);
      get(root->right, key);
    }
  }
  else
  {
    pthread_rwlock_unlock(&root->lock);
  }
}

void range(Node* root, int start, int end)
{
  if(root == groot)
  {
    pthread_rwlock_rdlock(&tree_lock);
    if(groot == NULL)
    {
      pthread_rwlock_unlock(&tree_lock);
      return;
    }
    else
    {
      pthread_rwlock_rdlock(&groot->lock);
      root = groot;
      pthread_rwlock_unlock(&tree_lock);
    }
  }

  if(start < root->key)
  {
    if(root->left == NULL)
    {
      //blank
    }
    else
    {
      pthread_rwlock_rdlock(&root->left->lock);
      pthread_rwlock_unlock(&root->lock);
      range(root->left, start, end);
      pthread_rwlock_rdlock(&root->lock);
    }
  }

  if( (root->key >=start) && (root->key <= end) )
  {
    out_file << endl << " " << setw(5) << root->key << " : " << root->value << " ";

    // cout << " " << root->key << ":" << root->value << " ";

    pthread_rwlock_unlock(&root->lock);
  }

  if(end > root->key)
  {
    if(root->right == NULL)
    {
      //blank
    }
    else
    {
      pthread_rwlock_rdlock(&root->right->lock);
      pthread_rwlock_unlock(&root->lock);
      range(root->right, start, end);
      pthread_rwlock_rdlock(&root->lock);
    }
  }
}


void* thread_func(void*)
{



  for(int i=0;i<num_iterations;i++)
  {
    int a = rand()%MAX_KEY;
    int b = rand()%MAX_VALUE;

    put(groot, a, b);

    get(groot, a);

  }

}
