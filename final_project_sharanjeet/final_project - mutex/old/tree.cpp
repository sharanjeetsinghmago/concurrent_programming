#include "tree.h"

using namespace std;

Node* getNewNode(int key, int value)
{
  Node* newNode = new Node();
  newNode->key = key;
  newNode->value = value;
  newNode->left = NULL;
  newNode->right = NULL;

  return newNode;
}

Node* findMin(Node* root)
{
  while(root->left != NULL)
  {
    root = root->left;
  }
  return root;
}

void inOrder(Node *root)
{
  if(root != NULL)
  {
    inOrder(root->left);
    cout << " " << root->key << ":" << root->value << " ";
    inOrder(root->right);
  }
}

Node* insert(Node* root, int key, int value)
{
  if(root == NULL)
  {
    root = getNewNode(key,value);
  }
  else if(key < root->key)
  {
    root->left = insert(root->left, key, value);
  }
  else if(key > root->key)
  {
    root->right = insert(root->right, key, value);
  }
  else
  {
    cout<<"\n Entry with same key already exists!!!";
  }

  return root;
}


Node* delete_key(Node* root, int key)
{
  if(root == NULL)
    return root;
  else if( key < root->key)
    root->left = delete_key( root->left, key);
  else if( key > root->key)
    root->right = delete_key( root->right, key);
  else
  {
    if(root->left == NULL && root->right == NULL)
    {
      // CASE 1: No Children
      delete root;
      root = NULL;
      return root;
    }
    else if(root->left == NULL)
    {
      // CASE 2: 1 Child
      Node* temp = root;
      root = root->right;
      delete temp;
      return root;
    }
    else if(root->right == NULL)
    {
      // CASE 2: 1 Child
      Node* temp = root;
      root = root->left;
      delete temp;
      return root;
    }
    else
    {
      // CASE 3: 2 Children
      Node *temp = findMin(root->right);
      root->key = temp->key;
      root->value = temp->value;
      root->right = delete_key(root->right, temp->key);
    }
  }
  return root;
}


Node* getNode(Node* root, int key)
{
  if(root == NULL)
  {
    return NULL;
  }
  else if(key == root->key)
  {
    return root;
  }
  else if(key < root->key)
  {
    return getNode(root->left, key);
  }
  else if(key > root->key)
  {
    return getNode(root->right, key);
  }

}
