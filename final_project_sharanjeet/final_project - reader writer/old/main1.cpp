#include "tree.h"

using namespace std;

int main()
{

  srand(time(NULL));

  Node *root = NULL;

  root = insert(root, 50, 10);

  root = insert(root, 90, 122);

  root = insert(root, 20, 2131);

  for(int i=0;i<10;i++)
  {
    root = insert(root, rand()%20, rand()%100);
  }

  cout<<"\n In order traversal of tree => ";

  inOrder(root);

  cout<<endl;

  int x;

  cin>>x;

  // root = delete_key(root,x);
  //
  // cout<<"\n In order traversal of tree => ";
  //
  // inOrder(root);
  //
  // cout<<endl;

  Node* g = getNode(root,x);

  if(g == NULL)
    cout<<"\n Node with key "<<x<<" not found!!!";
  else
    cout << "\n Key: " << g->key << " Value: " << g->value;

  cout<<endl;

  return 0;
}
