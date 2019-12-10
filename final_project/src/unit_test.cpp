#include <iostream>
#include "trees_lock.h"

using namespace std;

extern Node* groot;

void test_put()
{
  cout<<"\n Testing put function";
  put(groot, 50, 100);
}

void test_get()
{
  cout<<"\n Testing get function";
  get(groot,25);
  get(groot,50);
}

int main()
{
  srand(time(NULL));

  groot = NULL;

  test_put();
  test_get();

  cout<<endl;

  return 0;
}
