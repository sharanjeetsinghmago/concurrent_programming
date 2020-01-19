#include <string.h>
#include <time.h>

#include "trees_lock.h"

using namespace std;

extern ofstream out_file;

int num_threads = 10;

extern int num_iterations;

extern pthread_rwlock_t tree_lock;

extern Node *groot;

extern int r_min;

extern int r_max;

extern int MAX_KEY;

int cont = -1;

int main( int argc, char **argv)
{

  srand(time(NULL));

  clock_t t1,t2;

  if(argc == 1)
  {
    cout<<"\n Usage like this \n";
    return 0;
  }

  for(int i=0;i<argc;i++)
  {

    if(strcmp(argv[i],"--name") == 0)
    {
      cout<<"\n Sharanjeet Singh Mago\n";
      return 0;
    }

    if(i+1 != argc)
    {
      if(strcmp(argv[i],"-t") == 0)
      {
        num_threads = atoi(argv[i+1]);
        i++;
      }
    }

    if(i+1 != argc)
    {
      if(strcmp(argv[i],"-i") == 0)
      {
        num_iterations = atoi(argv[i+1]);
        i++;
      }
    }

    if(i+1 != argc && i+2 != argc)
    {
      if(strcmp(argv[i],"-r") == 0)
      {
        r_min = atoi(argv[i+1]);
        r_max = atoi(argv[i+2]);
        i=i+2;
      }
    }

    if(i+1 != argc)
    {
      if(strcmp(argv[i],"-c") == 0)
      {
        if(atoi(argv[i+1]) == 1)
        {
          cont = 1;
          MAX_KEY = 10;
        }
        else if(atoi(argv[i+1]) == 0)
        {
          cont = 0;
        }
        i++;
      }
    }

  }

  cout<<"\n Running Code with ";
  cout<<"\n Threads : "<<num_threads;
  cout<<"\n Iterations : "<<num_iterations;
  cout<<"\n Range : ["<<r_min<<":"<<r_max<<"]";
  //cout<<"\n MAX KEY : "<<MAX_KEY;



  groot = NULL;

  pthread_rwlock_init(&tree_lock,NULL);

  pthread_t threads[num_threads];

  if(cont != -1)
  {
    if(cont == 1)
    {
      cout<<"\n\n HIGH CONTENTION SETTINGS";
    }
    if(cont == 0)
    {
      cout<<"\n\n LOW CONTENTION SETTINGS";
    }

    cout<<"\n Making Tree for contention settings";
    for(int i=0;i<num_iterations;i++)
    {
      int a = rand()%100000;
      int b = rand()%1000000;

      put(groot, a, b);

    }

  }

  t1 = clock();

  for(int i=0;i<num_threads;i++)
  {
    pthread_create(&threads[i], NULL, thread_func, NULL);
  }

  for (int i=0; i<num_threads; i++)
  {
    pthread_join(threads[i], NULL);
  }

  t2 = clock();

  out_file.open("../output/out.txt");

  if(!out_file)
  {
    cout<<"file nhi bani";
  }

  out_file<<"\n\n Range => \n";

  //cout<<"\n\n Range => \n";

  range(groot, r_min, r_max);

  cout<<endl;

  cout << "\n\n Time taken total: " << (t2 - t1) /
      (double)CLOCKS_PER_SEC << " secs " << endl;

  return 0;
}
