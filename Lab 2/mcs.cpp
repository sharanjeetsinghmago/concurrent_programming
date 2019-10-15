#include <iostream>
#include <atomic>

using namespace std;

class Node
{
  atomic<Node*> next;
  atomic<bool> wait;
};

class MCSLock
{
  atomic<Node*> tail;
  tail = NULL;
};
//
// MCSLock::acquire(Node* myNode)
// {
//   Node* oldTail = tail.load();
//   myNode->next.store(NULL,RELAXED);
//   while(!tail.CAS(oldTail,myNode))
//   {
//     oldTail = tail.load();
//   }
//
//   // if oldTail== NULL, we’ve
//   // acquired the lock
//   // otherwise, wait for it
//
//   if(oldTail != NULL)
//   {
//     myNode->wait.store(true,RELAXED);
//     oldTail->next.store(myNode);
//     while( myNode->wait.load()){}
//   }
// }
//
// MCSLock::release(Node* myNode)
// {
//   if(tail.CAS(myNode, NULL))
//   {
//     // no one is waiting, and we just
//     // freed the lock
//   }
//   else
//   {
//     // hand lock to next waiting thread
//     while(myNode->next.load() == NULL) {}
//     myNode->next->wait.store(false);
//   }
// }


int main()
{
  cout<<"Hello World"<<endl;
  return 0;
}
