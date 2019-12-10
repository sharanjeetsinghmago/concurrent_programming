#include <string.h>

#include "trees_lock.h"

using namespace std;

ofstream out_file;

int num_threads = 10;

int num_iterations = 10;

pthread_mutex_t tree_lock;

Node *groot;

int r_min = 0;

int r_max = MAX_KEY;

int main( int argc, char **argv)
{

  srand(time(NULL));

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

  }

  cout<<"\n Running Code with ";
  cout<<"\n Threads : "<<num_threads;
  cout<<"\n Iterations : "<<num_iterations;
  cout<<"\n Range : ["<<r_min<<":"<<r_max<<"]";


  pthread_mutex_init(&tree_lock,NULL);

  pthread_t threads[num_threads];

  for(int i=0;i<num_threads;i++)
  {
    pthread_create(&threads[i], NULL, thread_func, NULL);
  }

  for (int i=0; i<num_threads; i++)
  {
    pthread_join(threads[i], NULL);
  }

  out_file.open("../output/out.txt");

  if(!out_file)
  {
    cout<<"file nhi bani";
  }

  out_file<<"\n In order traversal of tree => ";

  // cout<<"\n In order traversal of tree => ";

  inOrder(groot);

  //range(groot, 0, MAX_KEY);

  cout<<endl;



  return 0;
}
