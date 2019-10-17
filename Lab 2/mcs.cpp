#include <iostream>
#include <atomic>

using namespace std;

class Node
{
public:
  atomic<Node*> next;
  atomic<bool> wait;
};

class MCSLock
{
public:
  atomic<Node*> tail{NULL};

  void acquire(Node*);
  void release(Node*);

};

void MCSLock::acquire(Node* myNode)
{
  Node* oldTail = tail.load();
  myNode->next.store(NULL);
  while(!tail.compare_exchange_strong(oldTail,myNode))
  {
    oldTail = tail.load();
  }

  // if oldTail== NULL, we’ve
  // acquired the lock
  // otherwise, wait for it

  if(oldTail != NULL)
  {
    myNode->wait.store(true);
    oldTail->next.store(myNode);
    while( myNode->wait.load()){}
  }
}

void MCSLock::release(Node* myNode)
{
  if(tail.compare_exchange_strong(myNode, NULL))
  {
    // no one is waiting, and we just
    // freed the lock
  }
  else
  {
    // hand lock to next waiting thread
    while(myNode->next.load() == NULL) {}
    myNode->next.load()->wait.store(false);
  }
}


int main()
{
  cout<<"Hello World"<<endl;
  return 0;
}
