#include <iostream>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <mutex>
#include <omp.h>

# define MAX 500000

using namespace std;

//int part=0;

int a[MAX];

vector<multiset<int>> b;

int size = 40;

//int num_threads = 6;

mutex insert_lock;

mutex part_lock;

bool fj=1;
bool bucket=0;


int getMax(int a[], int n)
{
  int max = 0;
  for(int i=0;i<MAX;i++)
    if(a[i]>max)
      max = a[i];

  return max;
}



// merging two parts
void merge( int left, int mid, int right)
{

  int x = mid - left + 1;   // size of left part
  int y = right - mid;      // size of right part

  int L[x],R[y];


  // inserting values in left and right parts

  for(int i=0;i<x+y;i++)
  {
    if(i<x)
    {
      L[i] = a[left+i];

    }
    else
    {
      R[i-x] = a[left+i];

    }
  }


  int i=0,j=0,k=left;


  // merge left and right in ascending order
  while(i<x && j<y)
  {

    if(L[i]<R[j])
    {
      a[k] = L[i];
      i++;
    }
    else
    {
      a[k] = R[j];
      j++;
    }
    k++;
  }

  // insert remaining values from left
  while(i<x)
  {
    a[k] = L[i];
    i++;
    k++;
  }


  // inserting remaining values from right
  while(j<y)
  {
    a[k] = R[j];
    j++;
    k++;
  }
}



void merge_sort(int left, int right)
{

  if(left < right)
  {
    int mid = left + (right - left)/2;
    merge_sort(left, mid);
    merge_sort(mid+1, right);

    merge(left, mid, right);
  }
}


void merging(int num_threads)
{


    cout<<"\n num  = "<<num_threads;

    int thread_part = 0;

    for(int i=0;i<num_threads;i++)
    {

      int left = thread_part * (size/num_threads);
      int right = (thread_part + 1) * (size/num_threads) - 1;

      if(size%num_threads != 0 && i==(num_threads-1))
      {
        right= size-1;
      }

      merge_sort(left,right);
      thread_part++;
    }

    int a,b;

    for(int i=1;i<num_threads;i++)
    {

      a = (i*(size/num_threads))-1;

      b = ((i+1)*(size/num_threads))-1;


      if(size%num_threads != 0 && i==(num_threads-1))
      {
        b= size-1;
      }


      merge(0,a,b);
    }

}




int main(int argc, char **argv)
{

  int num = 0;
  int n = 0;
  int i = 0;
  char* out_file;

  clock_t t1,t2,t3,t4;


  // checking for inputs in arguments
  for (i = 0; i < argc; i++)
  {


    if(i+1 != argc)
    {
      if(strcmp(argv[i],"-o") == 0)
      {
        out_file = argv[i+1];
        i++;
      }
    }


    if(strcmp(argv[i],"--name") == 0)
    {
      //printf("Sharanjeet Singh Mago\n");
      cout<<"Sharanjeet Singh Mago\n";
      return 0;
    }

    if(strcmp(argv[i],"--alg=fj") == 0)
    {
      fj=1;
      bucket=0;

      cout<<"\n\n Merge Sort using Multithreading\n";
    }

    if(strcmp(argv[i],"--alg=bucket") == 0)
    {
      fj=0;
      bucket=1;

      cout<<"\n\n Bucket Sort using Multithreading\n";
    }
  }


  cout<<endl;

  FILE *f;

  char data[20];

  char* file_name = argv[1];

  f = fopen(file_name, "r");

  i=0;

  if(f == NULL)
  {
    cout<<" Failed to open the input file\n";
    return -1;
  }
  else
  {

    while( fgets( data, 10, f) != NULL )
    {
      if(isdigit(data[0]))
      {

        num = atoi(data);

        a[i] = num;
        i++;
        num = 0;
      }
    }
  }

  fclose(f);

  cout<<"\n i = "<<i;

  size = i;

  n=i-1;

  b.resize(n);


  t1 = clock();


  #pragma omp parallel
  {

     #pragma omp single
     {

       merging(omp_get_num_threads());
     }

  }



  t2 = clock();

  f = fopen(out_file, "w");

  char buffer[200000] = "";

  int x;


  if(f == NULL)
  {
    //printf(" Failed to open the output file\n");
    cout<<" Failed to open the output file\n";
    return -1;
  }
  else
  {

    for(int i=0;i<=n;i++)
    {
      x = snprintf(buffer,10, "%d\n", a[i]);
      fwrite(buffer,x,1,f);
    }


  }



  cout << "\n Time taken total: " << (t2 - t1) /
      (double)CLOCKS_PER_SEC << endl;



}
