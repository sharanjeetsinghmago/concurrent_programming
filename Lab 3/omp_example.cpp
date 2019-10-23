#include<iostream>
#include <omp.h>

using namespace std;

int main(int argc, char *argv[])
{
  int nthreads, tid;
  int count =0;

  int a[100000];

  int temp;

   srand (time(NULL));

  for(int i=0;i<10000;i++)
  {
    a[i]=rand()%100000;
//    cout<<" "<<a[i];
  }

  cout<<endl;






  #pragma omp parallel private(tid)
  {


    tid = omp_get_thread_num();
    //cout <<tid<<endl;



    for(int i=0;i<10000;i++)
    {
      for(int j=0;j<i;j++)
      {
        if(a[i]<a[j])
        {
          temp = a[i];
          a[i] = a[j];
          a[j] = temp;
        }
      }
    }


    // for(int i=0;i<1000000000;i++)
    // {}
    // if(tid==0)
    // {
    //   nthreads = omp_get_num_threads();
    //   cout<<" Number of threads = "<<nthreads<<endl;
    // }
  }

  for(int i=0;i<10000;i++)
  {
    for(int j=0;j<i;j++)
    {
      if(a[i]<a[j])
      {
        cout<<"<<<NOT SORTED>>>"<<endl;
        return 1;
      }
    }
  }

  cout<<"<<<SORTED>>>"<<endl;

  // for(int i=0;i<1000;i++)
  // {
  //   cout<<" "<<a[i];
  // }

  cout<<endl;

  return 1;
}
