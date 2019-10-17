#include <iostream>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <mutex>
#include <atomic>

using namespace std;

int part=0;

int a[200000];

vector<multiset<int>> b;

int size = 40;

int num_threads = 6;

int TEST_NUM = 0;

mutex insert_lock;

mutex part_lock;

atomic<bool> flag_tas;

atomic<int> next_num;
atomic<int> now_serving;

bool fj=1;
bool bucket=0;



class Node
{
public:
  atomic<Node*> next;
  atomic<bool> wait;
};

atomic<Node*> tail{NULL};

class MCSLock
{
public:


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
  Node* temp_node = myNode;
  if(tail.compare_exchange_strong(temp_node, NULL))
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

MCSLock mcs_lock;


// LOCKS

//tas
void tas_lock()
{
  bool expected, changed;
  do{
    expected = false;
    changed = true;
  }while(!flag_tas.compare_exchange_strong(expected,changed));
}

void tas_unlock()
{
  flag_tas.store(false);
}


//ttas
void ttas_lock()
{
  bool expected, changed;
  do{
    expected = false;
    changed = true;
  }while(flag_tas.load()==true || !flag_tas.compare_exchange_strong(expected,changed));
}

void ttas_unlock()
{
  flag_tas.store(false);
}

//ticket

void ticket_lock()
{
  int my_num = next_num.fetch_add(1);
  while(now_serving.load() != my_num)
  {}
}

void ticket_unlock()
{
  now_serving.fetch_add(1);
}

int getMax(int a[], int n)
{
  int max = 0;
  for(int i=0;i<n;i++)
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


void* merge_sort(void* arg)
{
  part_lock.lock();
  int thread_part = part++;
  part_lock.unlock();

  int left = thread_part * (size/num_threads);
  int right = (thread_part + 1) * (size/num_threads) - 1;

  if(thread_part == num_threads-1)
  {
    right = size-1;
  }

  int mid = left + (right - left) / 2;
  if(left < right)
  {
    merge_sort(left, mid);
    merge_sort(mid + 1, right);
    merge(left, mid, right);
  }

}

void bucket_sort(int start,int n)
{
  cout<<"pika";
  int max = getMax(a, n);

  int divider = ceil(float(max+1)/num_threads);

  for (int i=start;i<=n;i++)
  {

    int x = floor( a[i] / divider);

    if(TEST_NUM == 1)
    {
      tas_lock();
      b[x].insert(a[i]);
      tas_unlock();
    }
    else if(TEST_NUM == 2)
    {
      ttas_lock();
      b[x].insert(a[i]);
      ttas_unlock();
    }
    else if(TEST_NUM == 3)
    {
      ticket_lock();
      b[x].insert(a[i]);
      ticket_unlock();
    }
    else if(TEST_NUM == 4)
    {
      Node* myNode = new(Node);
      mcs_lock.acquire(myNode);
      b[x].insert(a[i]);
      mcs_lock.release(myNode);
      free(myNode);
    }
    else
    {
      insert_lock.lock();
      b[x].insert(a[i]);
      insert_lock.unlock();
    }

  }
}


void* bucket_sort(void* arg)
{
  int thread_part = part++;

  int left = thread_part * (size/num_threads);
  int right = (thread_part + 1) * (size/num_threads) - 1;

  if(thread_part == num_threads-1)
  {
    right = size-1;
  }


  bucket_sort(left,right);

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


    if(strcmp(argv[i],"--name") == 0)
    {
      //printf("Sharanjeet Singh Mago\n");
      cout<<"Sharanjeet Singh Mago\n";
      return 0;
    }

    if(i+1 != argc)
    {
      if(strcmp(argv[i],"-o") == 0)
      {
        out_file = argv[i+1];
        i++;
      }
    }

    if(i+1 != argc)
    {
      if(strcmp(argv[i],"-t") == 0)
      {
        num_threads = atoi(argv[i+1]);
        i++;
      }
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

    if(strcmp(argv[i],"--lock=tas") == 0)
    {
      TEST_NUM = 1;
    }

    if(strcmp(argv[i],"--lock=ttas") == 0)
    {
      TEST_NUM = 2;
    }

    if(strcmp(argv[i],"--lock=ticket") == 0)
    {
      TEST_NUM = 3;
    }

    if(strcmp(argv[i],"--lock=mcs") == 0)
    {
      TEST_NUM = 4;
    }

    if(strcmp(argv[i],"--lock=pthread") == 0)
    {
      TEST_NUM = 5;
    }

    if(strcmp(argv[i],"--bar=sense") == 0)
    {
      TEST_NUM = 6;
    }

    if(strcmp(argv[i],"--bar=pthread") == 0)
    {
      TEST_NUM = 7;
    }
  }



  cout<<endl;

  next_num = 0;
  now_serving = 0;

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

  // Sorting code starts here

  t1 = clock();
  pthread_t threads[num_threads];

  for(int i=0; i < num_threads; i++)
  {
    if(fj)
    {
      pthread_create(&threads[i], NULL, merge_sort, (void*)NULL);
    }
    else if(bucket)
    {
      pthread_create(&threads[i], NULL, bucket_sort, (void*)NULL);
    }
  }


  t2 = clock();

  for (int i=0; i<num_threads; i++)
  {
    pthread_join(threads[i], NULL);
  }

  t3 = clock();

  if(fj)
  {
    for(int i=0;i<num_threads-1;i++)
    {
      merge(0,(i*(size/num_threads))-1,((i+1)*(size/num_threads))-1);
    }

    i=num_threads-1;
    merge(0,(i*(size/num_threads))-1,size-1);
  }
  else if(bucket)
  {
    int index = 0;
    for(int i=0; i<n; i++)
    {
      for(multiset<int>::iterator j = b[i].begin(); j!= b[i].end(); j++)
      {
        a[index++] = *j;
      }
    }
  }

  t4 = clock();


  // Sorting code ends here

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


  cout << endl << endl << "Number of threads : " << num_threads << endl << endl;
  // time taken by merge sort in seconds
  cout << "Time taken for fork: " << (t2 - t1) /
      (double)CLOCKS_PER_SEC << endl;

  cout << "Time taken for joining: " << (t3 - t2) /
      (double)CLOCKS_PER_SEC << endl;

  cout << "Time taken for fork and join: " << (t3 - t1) /
      (double)CLOCKS_PER_SEC << endl;

  cout << "Time taken merging: " << (t4 - t3) /
      (double)CLOCKS_PER_SEC << endl;

  cout << "Time taken total: " << (t4 - t1) /
      (double)CLOCKS_PER_SEC << endl;



}
