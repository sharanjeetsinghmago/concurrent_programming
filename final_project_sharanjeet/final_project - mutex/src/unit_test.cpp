#include <iostream>
#include "trees_lock.h"

using namespace std;

extern Node* groot;

void test_put()
{

  cout<<"\n Inserting keys from 1 to 10";
  for(int i=1;i<=10;i++)
  {
    put(groot, i, 100);
  }
}

bool test_get()
{

  cout<<"\n Getting nodes for keys from 1 to 10";
  for(int i=1;i<=10;i++)
  {
    // get should be able to find the nodes with these keys
    if(get(groot,i) == NULL)
    {
      return 0;
    }
  }

  cout<<"\n Getting nodes for keys from 11 to 20";
  for(int i=11;i<=20;i++)
  {
    // get should not be able to find the nodes with these keys
    if(get(groot,i) != NULL)
    {
      return 0;
    }
  }

  return 1;
}

int main()
{
  srand(time(NULL));

  groot = NULL;

  cout<<"\n Testing put and get functions!!!";

  test_put();
  if(test_get())
  {
    cout<<"\n\n <<<Put and Get tests passed>>>";
  }
  else
  {
    cout<<"\n Put and Get tests failed!!!";
  }

  cout<<endl;

  return 0;
}
